#include "Headers/TrainingExecution.h"
#include "Headers/DualSimulation.h"

// Constants for the Networks
const int inputSize = 12;
const int outputSize = 17;

// Give the network a default device
torch::Device TrainingController::tensorDevice = torch::kCPU;

//******************************************************************************


TrainingWorker::TrainingWorker(Ui::MainWindow* ui) :
    indexPolicyMapping({
                       {0, ui->quarantineWhenInfected},
                       {1, ui->totalLockdown},
                       {2, ui->weak},
                       {3, ui->moderate},
                       {4, ui->strong},
                       {5, ui->weakLockdown},
                       {6, ui->moderateLockdown},
                       {7, ui->strongLockdown},
                       {8, ui->weakAssistance},
                       {9, ui->moderateAssistance},
                       {10, ui->strongAssistance},
                       {11, ui->weakGuidelines},
                       {12, ui->moderateGuidelines},
                       {13, ui->strongGuidelines},
                       {14, ui->weakCommerce},
                       {15, ui->moderateCommerce},
                       {16, ui->strongCommerce}
                   }) {}


//******************************************************************************


void TrainingWorker::evaluateEpisode(int episodeLength, DualSimulation* sim,
                                     PolicyNetwork* network, QString policy,
                                     bool visualize) {

    // Create vectors to store the state, actions, and rewards
    torch::Tensor states = torch::zeros({episodeLength * 24, inputSize}, TrainingController::tensorDevice);
    torch::Tensor actions = torch::zeros({episodeLength * 24, outputSize}, TrainingController::tensorDevice);
    torch::Tensor output = torch::zeros({episodeLength * 24, outputSize}, TrainingController::tensorDevice);
    torch::Tensor rewards = torch::zeros({episodeLength * 24, 1}, TrainingController::tensorDevice);

    // Run the episode and record the states, actions, and rewards
    bool finished = false;
    int currentStep = 0;
    while (!finished && currentStep < episodeLength * 24) {

        // Get the current state from the Simulation
        torch::Tensor state = sim->getState().to(TrainingController::tensorDevice);
        states[currentStep] = state;

        // Evaluate the policies the network should enfore
        torch::Tensor probabilities = (*network)->forward(state);

        output[currentStep] = probabilities; // Store the raw output from the network
        torch::Tensor policies = applyPolicies(probabilities, this->indexPolicyMapping);
        actions[currentStep] = policies;

        // Advance the Simulation
        finished = sim->takeStep(episodeLength);

        // Calculate the reward from the previous action
        rewards[currentStep] = (calculateReward(sim->getState(), policy));

        // Increment the current step
        currentStep++;

        if (visualize) {
            sim->updateChart("ALL", false);
        }
    }

    // Indicate to the TrainingController that the episode has finished
    emit episodeFinished(states, output, actions, rewards);

}


//******************************************************************************


TrainingController::TrainingController(Ui::MainWindow* ui, MainWindow* window,
                                       int episodeLength,
                                       int numEpisodes, int networkSize) :
    network(PolicyNetwork(networkSize)),
    optimalNetwork(PolicyNetwork(networkSize)),
    optimizer(torch::optim::Adam(network->parameters())) {

    // Create the simulation
    this->sim = new DualSimulation(ui->lagPeriod->value(),
                                   ui->initialInfected->value(),
                                   ui->initialValue->value(),
                                   ui->numAgents->value(),
                                   ui,
                                   MainWindow::checkDebugInfo(ui));

    // Create a new simulation worker
    worker = new TrainingWorker(ui);
    this->ui = ui;
    ui->headlessMode->setCheckState(Qt::CheckState::Checked);

    // Move the worker object onto a second thread
    worker->moveToThread(&workerThread);

    // Enable Tensors to be passed across signals and slots
    qRegisterMetaType<torch::Tensor>("torch::Tensor");

    // Connect all the necessary signals and slots
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(sim, &Simulation::updateChart, sim, &DualSimulation::renderCharts, Qt::QueuedConnection);
    connect(worker, &TrainingWorker::printMessage, &MainWindow::printMessage);
    connect(this, &TrainingController::printMessage, &MainWindow::printMessage);

    connect(this, &TrainingController::evaluateEpisode, worker,
            &TrainingWorker::evaluateEpisode);
    connect(worker, &TrainingWorker::episodeFinished, this,
            &TrainingController::processEpisodeResults);

    // Enable the worker to force the screen to update
    connect(worker, &TrainingWorker::forceUpdate, window, &MainWindow::forceUpdate);

    // Initialize all variables needed for training
    this->episodeLength = episodeLength;
    this->numEpisodes = numEpisodes;
    this->policy = "Pandemic";
    currentEpisode = 0;
    episodeLosses = std::vector<double>(numEpisodes, 0.0);

//    optimizer.defaults().set_lr(1e-8);
    optimizer.defaults().set_lr(1e-2);
    optimalLoss = DBL_MAX;

    // Put the network on the correct device
    network->to(tensorDevice);

    // Start the worker thread, will wait
    workerThread.start();
}


//******************************************************************************


void TrainingController::trainNetwork(bool visualize) {

    // Once training has finished, signal and return
    if (currentEpisode >= numEpisodes) {
        emit trainingFinished();
        emit printMessage("Training Finished");
        return;
    }

    // Reset the Simulation
    delete sim;
    sim = new DualSimulation(ui->lagPeriod->value(),
                             ui->initialInfected->value(),
                             ui->initialValue->value(),
                             ui->numAgents->value(),
                             ui,
                             MainWindow::checkDebugInfo(ui));

    // Reconnect the signals and Slots
    if (visualize) {
        connect(sim, &Simulation::updateChart, sim, &Simulation::renderCharts);
    }

    // Update the training parameters
    this->episodeLength = ui->episodeLength->value();
    this->numEpisodes = ui->numEpisodes->value();
    this->policy = ui->policy->currentText();

    // Initialize the Simulation
    sim->init("Dual");
    sim->mapChartViews();
    if (visualize) {
        emit sim->updateChart("ALL", true);
    }

    // Signal to the worker to begin the Episode
    emit evaluateEpisode(episodeLength, sim, &network, this->policy);
}


//******************************************************************************


void TrainingController::loadNetwork(PolicyNetwork network, int currentStep) {
    this->currentEpisode = currentStep;
    this->network = network;
    this->network->to(tensorDevice);
}


//******************************************************************************


void TrainingController::processEpisodeResults(torch::Tensor states, torch::Tensor output,
                                               torch::Tensor policies, torch::Tensor rewards) {

    // Zero out the gradients in the optimizer
    optimizer.zero_grad();

    torch::autograd::DetectAnomalyGuard guard;
    if (torch::isinf(states).any().item<bool>()) {
        emit printMessage(QString::fromStdString(tensorToString(states.index({
                                                                                 torch::indexing::Slice(torch::indexing::None),
                                                                                 11
                                                                             }))));
    }

    // Calculate the loss
    torch::Tensor loss = calculateLoss(network, states, policies, rewards);

    emit printMessage(QString::fromStdString(tensorToString(loss)));
    sim->getUI()->loss->setText(QString::fromStdString(tensorToString(loss)));

    // Print out some debugging information
    if (policy == "Pandemic") {
        emit printMessage(QString::number(sim->getTotalCases()));
        emit printMessage(QString::number(sim->getTotalDeaths()));
    } else if (policy == "Economic") {
        emit printMessage(QString::number(sim->getCurrentValue()));
    } else {
        emit printMessage(QString::number(sim->getTotalCases()));
        emit printMessage(QString::number(sim->getTotalDeaths()));
        emit printMessage(QString::number(sim->getCurrentValue()));
    }

    // If the loss is better than the current best, save the network weights
    if (loss.item<double>() < optimalLoss) {
        optimalLoss = loss.item<double>();

        // Save the network to a file and then read it in to make a copy
        std::string tempFilename = QString("%1/temp/temp.net").arg(QDir::currentPath()).toStdString();
        std::string tempDirname = QString("%1/temp").arg(QDir::currentPath()).toStdString();
        saveNetwork(network, tempFilename, tempDirname);

        torch::load(optimalNetwork, tempFilename);
    }

    // Backpropogate the Loss through the network
    loss.backward();

    // Update the network according to the optimization chosen
    optimizer.step();

    // Store the loss
    episodeLosses[currentEpisode] = loss.item<double>();

    // Every ten episodes, save the state of the network
    QString complianceString = "/";
    if (ui->strongCompliance->checkState() == Qt::CheckState::Checked) {
        complianceString = "/strong_non_compliance/";
    } else if (ui->moderateCompliance->checkState() == Qt::CheckState::Checked) {
        complianceString = "/moderate_non_compliance/";
    } else if (ui->weakCompliance->checkState() == Qt::CheckState::Checked) {
        complianceString = "/weak_non_compliance/";
    }
    if (currentEpisode % 10 == 0) {
        saveNetwork(network, QString("%1/networks/%2%3%4.net").arg(QDir::currentPath()).arg(policy).arg(complianceString).arg(currentEpisode).toStdString(),
                    QString("%1/networks/%2%3").arg(QDir::currentPath()).arg(policy).arg(complianceString).toStdString());
    }

    // Increment the current episode and continue training
    currentEpisode++;
    trainNetwork(false);
}


//******************************************************************************
