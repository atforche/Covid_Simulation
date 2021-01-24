#include "Headers/mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Setup the UI for the mainWindow
    ui->setupUi(this);
    QGraphicsView* view = ui->mainCanvas;
    QGraphicsScene* scene = new QGraphicsScene(this);
    view->setScene(scene);

    // Changes the maximum agents and locations based on constant values
    ui->numAgents->setMaximum(MAX_AGENTS);
    ui->numAgentsLabel->setMaximum(MAX_AGENTS);
    ui->numLocations->setMaximum(MAX_LOCATIONS);
    ui->numLocationsLabel->setMaximum(MAX_LOCATIONS);

    // Set the icons of the speed buttons and set the speed to default
    ui->slowSim->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pauseSim->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->fastSim->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    currentSpeed = "Normal";
    paused = true;

    // Create a SimpleSimulation and connect it to the UI
    SimpleSimulation* simulation = new SimpleSimulation(50, ui, checkDebugInfo());
    this->sim = simulation;

    // Define the values for the simulationTypes dropdown
    QStringList simulationTypes;
    simulationTypes.append("Simple Simulation");
    ui->simulationType->addItems(simulationTypes);

    // Disable the UI buttons that should be disabled to begin
    enableUI();

    // Print any needed debug information
    qDebug() << ui->simulationType->currentText();
}


void MainWindow::disableUI() {
    ui->simulationType->setEnabled(false);
    ui->runSimulation->setEnabled(false);
    ui->numLocations->setEnabled(false);
    ui->numLocationsLabel->setEnabled(false);
    ui->numAgents->setEnabled(false);
    ui->numAgentsLabel->setEnabled(false);

    ui->resetSimulation->setEnabled(true);
    ui->slowSim->setEnabled(true);
    ui->pauseSim->setEnabled(true);
    ui->fastSim->setEnabled(true);
}


void MainWindow::enableUI() {
    ui->simulationType->setEnabled(true);
    ui->runSimulation->setEnabled(true);
    ui->numLocations->setEnabled(true);
    ui->numLocationsLabel->setEnabled(true);
    ui->numAgents->setEnabled(true);
    ui->numAgentsLabel->setEnabled(true);

    ui->resetSimulation->setEnabled(false);
    ui->slowSim->setEnabled(false);
    ui->pauseSim->setEnabled(false);
    ui->fastSim->setEnabled(false);

    ui->year->setText("0");
    ui->day->setText("0");
    ui->hour->setText("0");
    ui->speed->setText("Normal");
}


std::map<std::string, bool> MainWindow::checkDebugInfo() {
    std::map<std::string, bool> debug;
    debug["visualize behaviors"] = (ui->agentBehaviors->checkState() ==
                                    Qt::CheckState::Checked);
    return debug;
}


MainWindow::~MainWindow()
{
    delete sim;
    delete ui;
}



void MainWindow::on_runSimulation_clicked()
{
    disableUI();
    sim->reset();

    delete sim;
    sim = new SimpleSimulation(ui->numAgents->value(), ui, checkDebugInfo());
    sim->init();

    // Create a SimulationController and connect it to the Simulation
    SimulationController* control = new SimulationController(this->sim,
                                                             SimulationWorker::NORMAL);
    this->paused = false;
    this->controller = control;
    this->controller->startSimulation();
}


void MainWindow::on_resetSimulation_clicked()
{
    this->controller->pauseSimulation(); // Call to simulation controller to end execution
    ui->mainCanvas->scene()->clear();
    enableUI();
    ui->pauseSim->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    sim->reset();
    currentSpeed = "Normal";
}


void MainWindow::on_numLocations_valueChanged(int value)
{
    ui->numLocationsLabel->setValue(value);
}


void MainWindow::on_numAgents_valueChanged(int value)
{
    ui->numAgentsLabel->setValue(value);
}


void MainWindow::on_numLocationsLabel_valueChanged(int arg1)
{
    ui->numLocations->setValue(arg1);
}



void MainWindow::on_numAgentsLabel_valueChanged(int arg1)
{
    ui->numAgents->setValue(arg1);
}


void MainWindow::on_slowSim_clicked()
{
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

void MainWindow::on_pauseSim_clicked()
{
    if (this->paused) {
        this->controller->startSimulation();
        ui->pauseSim->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        ui->speed->setText(currentSpeed);
        this->paused = false;

        // Re-enable speed buttons when the simulation is played
        ui->slowSim->setEnabled(currentSpeed != "Slow");
        ui->fastSim->setEnabled(currentSpeed != "Unlimited");
    } else {
        this->controller->pauseSimulation();
        ui->pauseSim->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        ui->speed->setText("Paused");
        this->paused = true;

        // Disable speed buttons when the simulation is paused
        ui->slowSim->setEnabled(false);
        ui->fastSim->setEnabled(false);
    }
}

void MainWindow::on_fastSim_clicked()
{
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
