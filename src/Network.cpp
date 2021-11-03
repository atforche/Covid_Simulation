#include "Headers/Network.h"
#include "Headers/TrainingExecution.h"
#include "Headers/DualSimulation.h"
#include "istream"

// Constants for the Networks
const int inputSize = 12;
const int outputSize = 17;


//******************************************************************************


PolicyNetworkImpl::PolicyNetworkImpl(int networkSize) :
    linear1(torch::nn::Linear(inputSize, outputSize))
{
    // Initialize the weights to normally distributed weights
    torch::nn::init::sparse_(linear1->weight, 0.5);
    torch::nn::init::normal_(linear1->bias);

    // Register the submodules
    register_module("linear1", linear1);
}


//******************************************************************************


torch::Tensor PolicyNetworkImpl::forward(torch::Tensor x) {
    x = linear1(x);
    return x;
}


//******************************************************************************


void saveNetwork(PolicyNetwork& network, std::string filename, std::string dirname) {
    // Create the directories needed
    QDir dir(QString::fromStdString(dirname));
    dir.mkpath(QString::fromStdString(dirname));

    // Create the file to guarantee it can be written to
    QFile outfile(QString::fromStdString(filename));
    outfile.open(QIODevice::ReadWrite);
    outfile.close();

    // Write the network out to the file
    torch::save(network, filename);
}


//******************************************************************************


PolicyNetwork loadNetwork(std::string filename) {
    int networkSize = 50;
    PolicyNetwork storedNetwork(networkSize);
    torch::load(storedNetwork, filename);
    return storedNetwork;
}


//******************************************************************************


void discountRewards(std::vector<double>& rewards, double gamma) {

    // Discount and sum the rewards for the next 24 hours
    for (int i = 0; i < static_cast<int>(rewards.size()); ++i) {
        // Sum and discount the next 24 rewards
        std::vector<double> next24(24, 0.0);
        for (int j = 0; j < 24; ++j) {
            if (j + i >= rewards.size()) {
                break;
            }
            next24[j] = rewards[i + j] * pow(gamma, j);
        }
        rewards[i] = std::accumulate(next24.begin(), next24.end(), 0.0);
    }
}


//******************************************************************************


double calculateReward(torch::Tensor state, QString policy) {

    // Indices into the State tensor
    const int last24CasesIndex = 1;
    const int last24DeathsIndex = 3;
    const int homelessnessRate = 9;
    const int unemploymentRate = 10;
    const int percentageValChange = 11;

    double last24Cases = state[last24CasesIndex].item<double>();
    double last24Deaths = state[last24DeathsIndex].item<double>();
    double pandemicReward = 2.0;

    if (last24Cases > 0 && last24Deaths > 0) {
        pandemicReward = (1 / last24Cases) + (1 / pow(last24Deaths, 2));
    } else if (last24Cases > 0) {
        pandemicReward = (1 / last24Cases) + 1;
    } else if (last24Deaths > 0) {
        pandemicReward = 1 + (1 / pow(last24Deaths, 2));
    }

    double unemployment = state[unemploymentRate].item<double>();
    double homelessness = state[homelessnessRate].item<double>();
    double economicReward = 100 * state[percentageValChange].item<double>();
    if (economicReward < 0) {
        economicReward = economicReward * (1 + unemployment) * (1 + homelessness);
    } else {
        economicReward = economicReward * (1 - unemployment) * (1 - homelessness);
    }

    if (policy == "Pandemic") {
        return pandemicReward;
    } else if (policy == "Economic") {
        return economicReward;
    }

    return pandemicReward + economicReward;
}


//******************************************************************************


torch::Tensor calculateLoss(PolicyNetwork network, torch::Tensor states,
                            torch::Tensor actions, torch::Tensor rewards) {

    // Convert the rewards into a vector for discounting
    std::vector<double> discountedRewards;
    for (int i = 0; i < rewards.size(0); ++i) {
        discountedRewards.push_back(rewards[i].item<double>());
    }

    // Calculate the discounted rewards and return the result to a Tensor
    discountRewards(discountedRewards);
    torch::Tensor newRewards = torch::reshape(torch::tensor(discountedRewards),
                                              {states.size(0), 1}).to(TrainingController::tensorDevice);

    // Forward propogate through the network
    torch::Tensor probabilities = network->forward(states);

    // Calculate the average probability of the policies taken at each timestep
    torch::Tensor aggregatedProbabilities = torch::sum(torch::mul(probabilities, actions), 1, true);
    torch::Tensor policiesTaken = torch::sum(actions, 1, true);
    policiesTaken = torch::fmax(policiesTaken, torch::ones({policiesTaken.size(0),
                                                          policiesTaken.size(1)},
                                                          TrainingController::tensorDevice));

    torch::Tensor averageProbabilities = torch::div(aggregatedProbabilities, policiesTaken);

    // Multiply the rewards by the probabilities and take the log
    torch::Tensor loss = torch::mean(torch::mul(newRewards, averageProbabilities));

    return -1.0 * loss;
}


//******************************************************************************


torch::Tensor applyPolicies(torch::Tensor probabilities,
                            std::unordered_map<int, QCheckBox*> &indexPolicyMapping) {
    torch::Tensor policies = torch::zeros(probabilities.size(0));
    for (int i = 0; i < probabilities.size(0); ++i) {
        double probability = 1000 * probabilities[i].item<double>();
        policies[i] = static_cast<double>((rand() % 1000) < probability);

        if (policies[i].item<double>()) {
            indexPolicyMapping[i]->setCheckState(Qt::Checked);
        } else {
            indexPolicyMapping[i]->setCheckState(Qt::Unchecked);
        }
    }

    return policies;
}


//******************************************************************************


std::string tensorToString(torch::Tensor tensor) {
    std::ostringstream stream;
    stream << tensor;
    return(stream.str());
}


//******************************************************************************


void evaluateNetwork(PolicyNetwork network, Ui::MainWindow* ui, int episodeLength,
                     int numEpisodes) {

    // Mapping of Policies to their CheckBoxes
    std::unordered_map<int, QCheckBox*> indexPolicyMapping({
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
                                                           });

    // Create tensors to store the number of Cases, number of Deaths, Economic Value,
    // average Homelessness Rate, and average Unemployment Rate
    torch::Tensor cases = torch::zeros(numEpisodes);
    torch::Tensor deaths = torch::zeros(numEpisodes);
    torch::Tensor value = torch::zeros(numEpisodes);
    torch::Tensor homelessness = torch::zeros(numEpisodes);
    torch::Tensor unemployment = torch::zeros(numEpisodes);

    for (int i = 0; i < numEpisodes; ++i) {
        // Create the Simulation to run the Network on
        DualSimulation* sim = new DualSimulation(ui->lagPeriod->value(),
                                 ui->initialInfected->value(),
                                 ui->initialValue->value(),
                                 ui->numAgents->value(),
                                 ui,
                                 MainWindow::checkDebugInfo(ui));

        sim->init("Dual");
        sim->mapChartViews();

        // Create vectors to store the Unemployment and Homelessness rates
        std::vector<double> homelessnessRates;
        std::vector<double> unemploymentRates;

        // Run the Simulation through an Entire Episode
        bool finished = false;
        int currentStep = 0;
        while (!finished && currentStep < episodeLength * 24) {

            // Get the current state from the Simulation
            torch::Tensor state = sim->getState().to(TrainingController::tensorDevice);

            // Evaluate the policies the network should enfore
            torch::Tensor probabilities = network->forward(state);

            torch::Tensor policies = applyPolicies(probabilities, indexPolicyMapping);

            // Advance the Simulation
            finished = sim->takeStep(episodeLength);

            // Record the homelessness and unemployment rates
            homelessnessRates.push_back(static_cast<double>(sim->getNumHomelessAgents()) / sim->getCurrentNumAgents());
            unemploymentRates.push_back(static_cast<double>(sim->getNumUnemployedAgents()) / sim->getCurrentNumAgents());

            // Increment the current step
            currentStep++;
        }

        // Record some statistics about the Simulation
        cases[i] = sim->getTotalCases();
        deaths[i] = sim->getTotalDeaths();
        value[i] = sim->getTotalEconomicValue();
        homelessness[i] = std::accumulate(homelessnessRates.begin(), homelessnessRates.end(), 0.0) / homelessnessRates.size();
        unemployment[i] = std::accumulate(unemploymentRates.begin(), unemploymentRates.end(), 0.0) / unemploymentRates.size();

        // Delete the Simulation
        delete sim;
    }

    // Report out the statistics from all runs
    qDebug() << QString("%1: Average of %2 with std dev of %3").arg("Cases",
                                                                     QString::number(torch::mean(cases).item<double>()),
                                                                     QString::number(torch::std(cases).item<double>()));

    qDebug() << QString("%1: Average of %2 with std dev of %3").arg("Deaths",
                                                                     QString::number(torch::mean(deaths).item<double>()),
                                                                     QString::number(torch::std(deaths).item<double>()));

    qDebug() << QString("%1: Average of %2 with std dev of %3").arg("Economic Value",
                                                                     QString::number(torch::mean(value).item<double>()),
                                                                     QString::number(torch::std(value).item<double>()));

    qDebug() << QString("%1: Average of %2 with std dev of %3").arg("Homelessness",
                                                                     QString::number(torch::mean(homelessness).item<double>()),
                                                                     QString::number(torch::std(homelessness).item<double>()));

    qDebug() << QString("%1: Average of %2 with std dev of %3").arg("Unemployment",
                                                                     QString::number(torch::mean(unemployment).item<double>()),
                                                                     QString::number(torch::std(unemployment).item<double>()));


}


//******************************************************************************
