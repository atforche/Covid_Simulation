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

    // Create a SimpleSimulation and connect it to the UI
    SimpleSimulation* simulation = new SimpleSimulation(50, ui);
    this->sim = simulation;

    // Define the values for the simulationTypes dropdown
    QStringList simulationTypes;
    simulationTypes.append("Simple Simulation");
    ui->simulationType->addItems(simulationTypes);

    // Disable the UI buttons that should be disabled to begin
    ui->resetSimulation->setEnabled(false);

    // Print any needed debug information
    qDebug() << ui->simulationType->currentText();
}


MainWindow::~MainWindow()
{
    delete sim;
    delete ui;
}



void MainWindow::on_runSimulation_clicked()
{
    ui->simulationType->setEnabled(false);
    ui->runSimulation->setEnabled(false);
    ui->resetSimulation->setEnabled(true);
    ui->numLocations->setEnabled(false);
    ui->numAgents->setEnabled(false);
    ui->visualize->setEnabled(false);
    sim->reset();

    delete sim;
    sim = new SimpleSimulation(ui->numAgents->value(), ui);
    sim->init();

    // Create a SimulationController and connect it to the Simulation
    SimulationController* control = new SimulationController(this->sim,
                                                             SimulationWorker::SLOW);
    this->controller = control;
    this->controller->startSimulation();
}


void MainWindow::on_resetSimulation_clicked()
{
    this->controller->endSimulation();
    ui->simulationType->setEnabled(true);
    ui->mainCanvas->scene()->clear();
    ui->runSimulation->setEnabled(true);
    ui->resetSimulation->setEnabled(false);
    ui->numLocations->setEnabled(true);
    ui->numAgents->setEnabled(true);
    ui->visualize->setEnabled(true);
    sim->reset();
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
