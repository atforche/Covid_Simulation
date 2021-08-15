#include "Headers/mainwindow.h"
#include "Headers/SimpleSimulation.h"
#include "Headers/EconomicSimulation.h"
#include "Headers/PandemicSimulation.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    // Setup the main UI components for the mainWindow
    ui->setupUi(this);
    QGraphicsView* view = ui->mainCanvas;
    QGraphicsScene* scene = new QGraphicsScene(this);
    view->setScene(scene);

    // Sets the maximum agents and locations based on constant values
    ui->numAgents->setMaximum(MAX_AGENTS);
    ui->numAgentsLabel->setMaximum(MAX_AGENTS);
    ui->numLocations->setMaximum(MAX_LOCATIONS);
    ui->numLocationsLabel->setMaximum(MAX_LOCATIONS);

    // Set up the FRAMES_PER_HOUR slider and spin box
    ui->framesPerHour->setValue(Simulation::FRAMES_PER_HOUR);
    ui->framesPerHourSlider->setValue(Simulation::FRAMES_PER_HOUR);

    // Set the icons of the speed buttons and set the speed to default
    ui->slowSim->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pauseSim->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->fastSim->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    currentSpeed = "Normal";
    paused = true;

    // Create a SimpleSimulation and connect it to the UI
    SimpleSimulation* simulation = new SimpleSimulation(50, ui, checkDebugInfo());
    controller = nullptr;
    this->sim = simulation;

    // Initialize each of the Main Window's Combo Boxes
    initializeComboBoxes();

    // Enable the UI buttons that should be enabled to begin
    enableUI();
    showEconomicOptions(false);
    showPandemicOptions(false);

    // Update the maximum value of the initial infected to match the number of agents
    ui->initialInfected->setMaximum(ui->numAgents->value());
    ui->initialInfectedSlider->setMaximum(ui->numAgents->value());
    ui->hospitalCapacity->setMaximum(ui->numAgents->value());
    ui->hospitalCapacitySlider->setMaximum(ui->numAgents->value());
}


//******************************************************************************


MainWindow::~MainWindow() {
    if (controller) {
        controller->pauseSimulation();
    }
    delete sim;
    delete ui->graphView1->chart();
    delete ui->graphView2->chart();
    delete controller;
    delete ui;
}


//******************************************************************************


void MainWindow::disableUI() {

    // Disable all buttons that modify the state of the simulation
    ui->simulationType->setEnabled(false);
    ui->graph1Selection->setEnabled(false);
    ui->graph2Selection->setEnabled(false);
    ui->graph3Selection->setEnabled(false);
    ui->runSimulation->setEnabled(false);
    ui->numLocations->setEnabled(false);
    ui->numLocationsLabel->setEnabled(false);
    ui->numAgents->setEnabled(false);
    ui->numAgentsLabel->setEnabled(false);
    ui->agentBehaviors->setEnabled(false);
    ui->headlessMode->setEnabled(false);
    ui->framesPerHour->setEnabled(false);
    ui->framesPerHourSlider->setEnabled(false);

    // Enable all buttons that control the flow of the simulation
    ui->resetSimulation->setEnabled(true);
    ui->slowSim->setEnabled(true);
    ui->pauseSim->setEnabled(true);
    ui->fastSim->setEnabled(true);

    // Handle simulation specific options
    if (ui->simulationType->currentText() == "Economic Simulation") {
        enableEconomicOptions(false);
    }
}


//******************************************************************************


void MainWindow::enableUI() {

    // Enable all buttons that modify the state of the simulation
    ui->simulationType->setEnabled(true);
    ui->graph1Selection->setEnabled(true);
    ui->graph2Selection->setEnabled(true);
    ui->graph3Selection->setEnabled(true);
    ui->runSimulation->setEnabled(true);
    ui->numLocations->setEnabled(true);
    ui->numLocationsLabel->setEnabled(true);
    ui->numAgents->setEnabled(true);
    ui->numAgentsLabel->setEnabled(true);
    ui->agentBehaviors->setEnabled(true);
    ui->headlessMode->setEnabled(true);
    ui->framesPerHour->setEnabled(true);
    ui->framesPerHourSlider->setEnabled(true);

    // Disable all buttons that modify the flow of the simulation
    ui->resetSimulation->setEnabled(false);
    ui->slowSim->setEnabled(false);
    ui->pauseSim->setEnabled(false);
    ui->fastSim->setEnabled(false);

    // Reset all the dynamic text in the simulation
    ui->year->setText("0");
    ui->day->setText("0");
    ui->hour->setText("0");
    ui->speed->setText("Normal");

    // Handle simulation specific options
    if (ui->simulationType->currentText() == "Economic Simulation") {
        enableEconomicOptions(true);
    } else if (ui->simulationType->currentText() == "Pandemic Simulation") {
        enablePandemicOptions(true);
    }
}


//******************************************************************************


void MainWindow::showEconomicOptions(bool show) {
    enableEconomicOptions(false);
    ui->economicOptions->setVisible(show);
    ui->initialValue->setVisible(show);
    ui->initialValueLabel->setVisible(show);
    ui->initialValueSlider->setVisible(show);
    ui->currentValue->setVisible(show);
    ui->currentValueLabel->setVisible(show);
}


//******************************************************************************


void MainWindow::enableEconomicOptions(bool enabled) {
    ui->economicOptions->setEnabled(enabled);
    ui->initialValue->setEnabled(enabled);
    ui->initialValueLabel->setEnabled(enabled);
    ui->initialValueSlider->setEnabled(enabled);
}


//******************************************************************************


void MainWindow::enableEconomicCharts(bool enabled) {

    // Block signals while working on the ComboBoxes
    ui->graph1Selection->blockSignals(true);
    ui->graph2Selection->blockSignals(true);
    ui->graph3Selection->blockSignals(true);

    if (enabled) {
        // Add the Economic Charts to each Combo Box
        ui->graph1Selection->addItem("Agent Value");
        ui->graph1Selection->addItem("Agent Economic Status");
        ui->graph1Selection->addItem("Business Value");
        ui->graph1Selection->addItem("Total Value");

        ui->graph2Selection->addItem("Agent Value");
        ui->graph2Selection->addItem("Agent Economic Status");
        ui->graph2Selection->addItem("Business Value");
        ui->graph2Selection->addItem("Total Value");

        ui->graph3Selection->addItem("Agent Value");
        ui->graph3Selection->addItem("Agent Economic Status");
        ui->graph3Selection->addItem("Business Value");
        ui->graph3Selection->addItem("Total Value");

        // Apply the default Chart config for the Economic Simulation
        ui->graph1Selection->setCurrentIndex(6);
        ui->graph2Selection->setCurrentIndex(4);
        ui->graph3Selection->setCurrentIndex(5);

    } else {
        // Avoid removing items if items don't need to be removed
        if (ui->graph1Selection->count() == 7) {
            // Remove the Economic Charts to each Combo Box
            ui->graph1Selection->removeItem(6);
            ui->graph1Selection->removeItem(5);
            ui->graph1Selection->removeItem(4);
            ui->graph1Selection->removeItem(3);

            ui->graph2Selection->removeItem(6);
            ui->graph2Selection->removeItem(5);
            ui->graph2Selection->removeItem(4);
            ui->graph2Selection->removeItem(3);

            ui->graph3Selection->removeItem(6);
            ui->graph3Selection->removeItem(5);
            ui->graph3Selection->removeItem(4);
            ui->graph3Selection->removeItem(3);

            // Apply the default Chart config for the Economic Simulation
            ui->graph1Selection->setCurrentIndex(0);
            ui->graph2Selection->setCurrentIndex(1);
            ui->graph3Selection->setCurrentIndex(2);
        }

    }

    // Re-enable signals while working on the ComboBoxes
    ui->graph1Selection->blockSignals(false);
    ui->graph2Selection->blockSignals(false);
    ui->graph3Selection->blockSignals(false);

    // Update which options are enabled and disabled
    disableComboBoxOptions();
}


//******************************************************************************


void MainWindow::showPandemicOptions(bool show) {
    enablePandemicOptions(false);
    ui->pandemicOptions->setVisible(show);
    ui->initialInfected->setVisible(show);
    ui->initialInfectedLabel->setVisible(show);
    ui->initialInfectedSlider->setVisible(show);

    ui->lagPeriod->setVisible(show);
    ui->lagPeriodLabel->setVisible(show);
    ui->lagPeriodSlider->setVisible(show);

    ui->hospitalCapacity->setVisible(show);
    ui->hospitalCapacityLabel->setVisible(show);
    ui->hospitalCapacitySlider->setVisible(show);

    ui->totalCaseLabel->setVisible(show);
    ui->totalCases->setVisible(show);
    ui->totalDeathsLabel->setVisible(show);
    ui->totalDeaths->setVisible(show);

    ui->groupBox->setVisible(show);
}


//******************************************************************************


void MainWindow::enablePandemicOptions(bool enabled) {
    ui->initialInfected->setEnabled(enabled);
    ui->initialInfectedSlider->setEnabled(enabled);
    ui->lagPeriod->setEnabled(enabled);
    ui->lagPeriodSlider->setEnabled(enabled);

    ui->hospitalCapacity->setEnabled(enabled);
    ui->hospitalCapacitySlider->setEnabled(enabled);

    ui->groupBox->setEnabled(enabled);
}


//******************************************************************************


void MainWindow::enablePandemicCharts(bool enabled) {
    // Block signals while working on the ComboBoxes
    ui->graph1Selection->blockSignals(true);
    ui->graph2Selection->blockSignals(true);
    ui->graph3Selection->blockSignals(true);

    if (enabled) {
        // Add the Economic Charts to each Combo Box
        ui->graph1Selection->addItem("SEIR Chart");
        ui->graph1Selection->addItem("Daily Case/Death Tracker");

        ui->graph2Selection->addItem("SEIR Chart");
        ui->graph2Selection->addItem("Daily Case/Death Tracker");

        ui->graph3Selection->addItem("SEIR Chart");
        ui->graph3Selection->addItem("Daily Case/Death Tracker");

        // Apply the default Chart config for the Economic Simulation
        ui->graph1Selection->setCurrentIndex(3);
        ui->graph2Selection->setCurrentIndex(4);
        ui->graph3Selection->setCurrentIndex(2);

    } else {
        // Avoid removing items if items don't need to be removed
        if (ui->graph1Selection->count() == 5) {
            // Remove the Economic Charts to each Combo Box
            ui->graph1Selection->removeItem(4);
            ui->graph1Selection->removeItem(3);

            ui->graph2Selection->removeItem(4);
            ui->graph2Selection->removeItem(3);

            ui->graph3Selection->removeItem(4);
            ui->graph3Selection->removeItem(3);

            // Apply the default Chart config for the Economic Simulation
            ui->graph1Selection->setCurrentIndex(0);
            ui->graph2Selection->setCurrentIndex(1);
            ui->graph3Selection->setCurrentIndex(2);
        }

    }

    // Re-enable signals while working on the ComboBoxes
    ui->graph1Selection->blockSignals(false);
    ui->graph2Selection->blockSignals(false);
    ui->graph3Selection->blockSignals(false);

    // Update which options are enabled and disabled
    disableComboBoxOptions();
}


//******************************************************************************


void MainWindow::initializeComboBoxes() {

    // Define the values for the Graph1Selection dropdowns
    QStringList chartTypes(graphViews.begin(), graphViews.end());

    // Block the combo boxes from firing signal while they're being initialized
    ui->graph1Selection->blockSignals(true);
    ui->graph2Selection->blockSignals(true);
    ui->graph3Selection->blockSignals(true);

    ui->graph1Selection->addItems(chartTypes);
    ui->graph1Selection->setCurrentIndex(0);

    ui->graph2Selection->addItems(chartTypes);
    ui->graph2Selection->setCurrentIndex(1);

    ui->graph3Selection->addItems(chartTypes);
    ui->graph3Selection->setCurrentIndex(2);

    // Renable signals from the combo boxes
    ui->graph1Selection->blockSignals(false);
    ui->graph2Selection->blockSignals(false);
    ui->graph3Selection->blockSignals(false);

    // Define the values for the simulationTypes dropdown
    QStringList simulationTypes;
    simulationTypes.append("Simple Simulation");
    simulationTypes.append("Economic Simulation");
    simulationTypes.append("Pandemic Simulation");
    ui->simulationType->addItems(simulationTypes);

    // Disable the current selections in the other dropdowns
    disableComboBoxOptions();
}


//******************************************************************************


void MainWindow::disableComboBoxOptions() {

    // Block signals while modifying the combo boxes
    ui->graph1Selection->blockSignals(true);
    ui->graph2Selection->blockSignals(true);
    ui->graph3Selection->blockSignals(true);

    // Create a model to control the ComboBox options
    QStandardItemModel* model1 = qobject_cast<QStandardItemModel*>(
                ui->graph1Selection->model());
    QStandardItemModel* model2 = qobject_cast<QStandardItemModel*>(
                ui->graph2Selection->model());
    QStandardItemModel* model3 = qobject_cast<QStandardItemModel*>(
                ui->graph3Selection->model());

    // Enable each of the options to begin with
    for (int i = 0; i < static_cast<int>(graphViews.size()); ++i) {
        model1->item(i)->setEnabled(true);
        model1->item(i)->setSelectable(true);

        model2->item(i)->setEnabled(true);
        model2->item(i)->setSelectable(true);

        model3->item(i)->setEnabled(true);
        model3->item(i)->setSelectable(true);
    }

    // Disable the first's index in the remaining lists
    int currentIndex = ui->graph1Selection->currentIndex();
    model2->item(currentIndex)->setEnabled(false);
    model2->item(currentIndex)->setEnabled(false);
    model3->item(currentIndex)->setEnabled(false);
    model3->item(currentIndex)->setEnabled(false);

    // Disable the second's index in the remaining lists
    currentIndex = ui->graph2Selection->currentIndex();
    model1->item(currentIndex)->setEnabled(false);
    model1->item(currentIndex)->setSelectable(false);
    model3->item(currentIndex)->setEnabled(false);
    model3->item(currentIndex)->setEnabled(false);

    // Disable the third's index in the remaining lists
    currentIndex = ui->graph3Selection->currentIndex();
    model1->item(currentIndex)->setEnabled(false);
    model1->item(currentIndex)->setSelectable(false);
    model2->item(currentIndex)->setEnabled(false);
    model2->item(currentIndex)->setEnabled(false);

    // Allow signals once modifications are made
    ui->graph1Selection->blockSignals(false);
    ui->graph2Selection->blockSignals(false);
    ui->graph3Selection->blockSignals(false);
}


//******************************************************************************


std::map<std::string, bool> MainWindow::checkDebugInfo() {
    // Create map containing all debug fields
    std::map<std::string, bool> debug;
    debug["visualize behaviors"] = (ui->agentBehaviors->checkState() ==
                                    Qt::CheckState::Checked);
    debug["headless mode"] = (ui->headlessMode->checkState() ==
                              Qt::CheckState::Checked);

    // Add debug information for the Pandemic Options
    debug["quarantine when infected"] = (ui->quarantineWhenInfected->checkState() ==
                                         Qt::CheckState::Checked);
    debug["weak contact tracing"] = (ui->weak->checkState() ==
                                     Qt::CheckState::Checked);
    debug["moderate contact tracing"] = (ui->moderate->checkState() ==
                                     Qt::CheckState::Checked);
    debug["strong contact tracing"] = (ui->strong->checkState() ==
                                     Qt::CheckState::Checked);

    debug["weak lockdown"] = (ui->weakLockdown->checkState() ==
                                     Qt::CheckState::Checked);
    debug["moderate lockdown"] = (ui->moderateLockdown->checkState() ==
                                     Qt::CheckState::Checked);
    debug["strong lockdown"] = (ui->strongLockdown->checkState() ==
                                     Qt::CheckState::Checked);

    debug["weak non-compliance"] = (ui->weakCompliance->checkState() ==
                                     Qt::CheckState::Checked);
    debug["moderate non-compliance"] = (ui->moderateCompliance->checkState() ==
                                     Qt::CheckState::Checked);
    debug["strong non-compliance"] = (ui->strongCompliance->checkState() ==
                                     Qt::CheckState::Checked);

    debug["weak guidelines"] = (ui->weakGuidelines->checkState() ==
                                     Qt::CheckState::Checked);
    debug["moderate guidelines"] = (ui->moderateGuidelines->checkState() ==
                                     Qt::CheckState::Checked);
    debug["strong guidelines"] = (ui->strongGuidelines->checkState() ==
                                     Qt::CheckState::Checked);

    debug["weak e-commerce"] = (ui->weakCommerce->checkState() ==
                                     Qt::CheckState::Checked);
    debug["moderate e-commerce"] = (ui->moderateCommerce->checkState() ==
                                     Qt::CheckState::Checked);
    debug["strong e-commerce"] = (ui->strongCommerce->checkState() ==
                                     Qt::CheckState::Checked);

    return debug;
}


//******************************************************************************


void MainWindow::on_runSimulation_clicked() {
    // Disable the UI and reset the simulation
    disableUI();
    sim->reset();

    // Delete and reinitialize the simulation so the changes take effect
    delete sim;
    if (ui->simulationType->currentText() == "Simple Simulation") {
        sim = new SimpleSimulation(ui->numAgents->value(), ui, checkDebugInfo());
    } else if (ui->simulationType->currentText() == "Economic Simulation") {
        sim = new EconomicSimulation(ui->initialValueSlider->value(),
                                     ui->numAgents->value(), ui,
                                     checkDebugInfo());
    } else if (ui->simulationType->currentText() == "Pandemic Simulation") {
        sim = new PandemicSimulation(ui->lagPeriod->value(),
                                     ui->initialInfected->value(),
                                     ui->numAgents->value(),
                                     ui, checkDebugInfo());
    }
    sim->init();

    // Update the charts on the main window (non-dynamic for now)
    sim->mapChartViews();
    sim->renderCharts("ALL", true);

    // Create a SimulationController and connect it to the Simulation
    SimulationController* control = new SimulationController(this->sim,
                                                             SimulationWorker::NORMAL);

    // Connect the controller to the window and start the simulation
    this->paused = false;
    this->controller = control;
    this->controller->startSimulation();
}


//******************************************************************************


void MainWindow::on_resetSimulation_clicked() {

    // Lock the screen to prevent any rendering updates from happening
    QMutexLocker lock(sim->getQueueLock());

    // Have the controller stop the simulation
    this->controller->pauseSimulation();

    // Update the UI as needed
    ui->mainCanvas->scene()->clear();
    enableUI();
    ui->pauseSim->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    sim->reset();

    // Update the current speed of the simulation
    currentSpeed = "Normal";
}


//******************************************************************************


void MainWindow::on_numLocations_valueChanged(int value) {
    ui->numLocationsLabel->setValue(value);
}


//******************************************************************************


void MainWindow::on_numAgents_valueChanged(int value) {
    ui->numAgentsLabel->setValue(value);

    // Update the maximum number of initially infected agents
    ui->initialInfected->setMaximum(value);
    ui->initialInfectedSlider->setMaximum(value);

    // Update the maximum number of hospitalized agents
    ui->hospitalCapacity->setMaximum(value);
    ui->hospitalCapacitySlider->setMaximum(value);
}


//******************************************************************************


void MainWindow::on_numLocationsLabel_valueChanged(int arg1) {
    ui->numLocations->setValue(arg1);
}


//******************************************************************************


void MainWindow::on_numAgentsLabel_valueChanged(int arg1) {
    ui->numAgents->setValue(arg1);

    // Update the maximum number of initially infected agents
    ui->initialInfected->setMaximum(arg1);
    ui->initialInfectedSlider->setMaximum(arg1);

    // Update the maximum number of hospitalized agents
    ui->hospitalCapacity->setMaximum(arg1);
    ui->hospitalCapacitySlider->setMaximum(arg1);
}


//******************************************************************************


void MainWindow::on_slowSim_clicked() {
    // Slow down the simulation based on the current state
    if(currentSpeed == "Unlimited") {
        this->controller->changeSpeed(SimulationWorker::FAST);
        this->currentSpeed = "Fast";
        ui->fastSim->setEnabled(true); //Make user able to speed up sim again
    } else if (currentSpeed == "Fast") {
        this->controller->changeSpeed(SimulationWorker::NORMAL);
        this->currentSpeed = "Normal";
    } else if (currentSpeed == "Normal") {
        this->controller->changeSpeed(SimulationWorker::SLOW);
        this->currentSpeed = "Slow";
        ui->slowSim->setEnabled(false); //Can't slow the Sim past slow
    }
    ui->speed->setText(currentSpeed);
}


//******************************************************************************


void MainWindow::on_pauseSim_clicked() {
    if (this->paused) {
        // Restart the simulation and update the UI
        this->controller->startSimulation();
        ui->pauseSim->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        ui->speed->setText(currentSpeed);
        this->paused = false;

        // Re-enable speed buttons when the simulation is played
        ui->slowSim->setEnabled(currentSpeed != "Slow");
        ui->fastSim->setEnabled(currentSpeed != "Unlimited");
    } else {
        // Stop the simulation and update the UI
        this->controller->pauseSimulation();
        ui->pauseSim->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        ui->speed->setText("Paused");
        this->paused = true;

        // Disable speed buttons when the simulation is paused
        ui->slowSim->setEnabled(false);
        ui->fastSim->setEnabled(false);
    }
}


//******************************************************************************


void MainWindow::on_fastSim_clicked() {
    // Speed up the simulation based on the current state
    if (currentSpeed == "Slow") {
        this->controller->changeSpeed(SimulationWorker::NORMAL);
        this->currentSpeed = "Normal";
        ui->slowSim->setEnabled(true); //Allow user to slow down sim again
    } else if (currentSpeed == "Normal") {
        this->controller->changeSpeed(SimulationWorker::FAST);
        this->currentSpeed = "Fast";
    } else if (currentSpeed == "Fast") {
        this->controller->changeSpeed(SimulationWorker::UNLIMITED);
        this->currentSpeed = "Unlimited";
        ui->fastSim->setEnabled(false);
    }
    ui->speed->setText(currentSpeed);
}


//******************************************************************************


void MainWindow::on_graph1Selection_currentTextChanged(const QString &) {
    sim->mapChartViews();
    disableComboBoxOptions();
    sim->renderCharts("ALL", true);
}


//******************************************************************************


void MainWindow::on_graph2Selection_currentTextChanged(const QString &) {
    sim->mapChartViews();
    disableComboBoxOptions();
    sim->renderCharts("ALL", true);
}


//******************************************************************************


void MainWindow::on_graph3Selection_currentTextChanged(const QString &) {
    sim->mapChartViews();
    disableComboBoxOptions();
    sim->renderCharts("ALL", true);
}


//******************************************************************************


void MainWindow::on_framesPerHourSlider_valueChanged(int value) {
    ui->framesPerHour->setValue(value);
    Simulation::FRAMES_PER_HOUR = value;
}


//******************************************************************************


void MainWindow::on_framesPerHour_valueChanged(int arg1) {
    ui->framesPerHourSlider->setValue(arg1);
    Simulation::FRAMES_PER_HOUR = arg1;
}


//******************************************************************************


void MainWindow::on_simulationType_currentTextChanged(const QString &arg1) {
    if (arg1 == "Simple Simulation") {
        delete sim;
        this->sim = new SimpleSimulation(0, ui, std::map<std::string, bool>());

        // Ensure Economic options are disabled
        enableEconomicCharts(false);
        showEconomicOptions(false);

        // Ensure Pandemic options are disabled
        enablePandemicCharts(false);
        showPandemicOptions(false);

    } else if (arg1 == "Economic Simulation") {
        delete sim;
        this->sim = new EconomicSimulation(0, 0, ui, std::map<std::string, bool>());

        // Ensure the Pandemic options are disabled
        enablePandemicCharts(false);
        showPandemicOptions(false);

        // Enable the Economic options
        enableEconomicCharts(true);
        showEconomicOptions(true);

    } else if (arg1 == "Pandemic Simulation") {
        delete sim;
        this->sim = new PandemicSimulation(0, 0, 0, ui, std::map<std::string, bool>());

        // Ensure economic options are disabled
        enableEconomicCharts(false);
        showEconomicOptions(false);

        // Enable the Pandemic options
        enablePandemicCharts(true);
        showPandemicOptions(true);
    }
    enableUI();
}


//******************************************************************************


void MainWindow::on_initialValue_valueChanged(int arg1) {
    ui->initialValueSlider->setValue(arg1);
}


//*****************************************************************************


void MainWindow::on_initialValueSlider_valueChanged(int arg1) {
    ui->initialValue->setValue(arg1);
}


//******************************************************************************


void MainWindow::on_lagPeriod_valueChanged(int arg1) {
    ui->lagPeriodSlider->setValue(arg1);
}


//******************************************************************************


void MainWindow::on_lagPeriodSlider_valueChanged(int value) {
    ui->lagPeriod->setValue(value);
}


//******************************************************************************


void MainWindow::on_initialInfected_valueChanged(int arg1) {
    ui->initialInfectedSlider->setValue(arg1);
}


//******************************************************************************


void MainWindow::on_initialInfectedSlider_valueChanged(int value) {
    ui->initialInfected->setValue(value);
}


//******************************************************************************


void MainWindow::on_hospitalCapacity_valueChanged(int arg1) {
    ui->hospitalCapacitySlider->setValue(arg1);
}


//******************************************************************************


void MainWindow::on_hospitalCapacitySlider_valueChanged(int value) {
    ui->hospitalCapacity->setValue(value);
}


//******************************************************************************
