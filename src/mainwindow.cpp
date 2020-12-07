#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "QGraphicsRectItem"
#include "QtTest/QTest"
#include <vector>
#include <string>
#include <Headers/SimpleSimulation.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Setup the UI for the mainWindow
    ui->setupUi(this);
    QGraphicsView* view = ui->mainCanvas;
    QGraphicsScene* scene = new QGraphicsScene(this);
    view->setScene(scene);

    // Create a SimpleSimulation and connect it to the UI
    SimpleSimulation* simulation = new SimpleSimulation(1, ui);
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
    sim->init();
}


void MainWindow::on_resetSimulation_clicked()
{
    ui->simulationType->setEnabled(true);
    ui->mainCanvas->scene()->clear();
    ui->runSimulation->setEnabled(true);
    ui->resetSimulation->setEnabled(false);
    sim->reset();

    // Reset and recreate the sim
    delete sim;
    sim = new SimpleSimulation(1, ui);
}
