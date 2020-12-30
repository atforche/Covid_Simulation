#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Headers/Simulation.h>
#include "ui_mainwindow.h"
#include "QGraphicsRectItem"
#include "QtTest/QTest"
#include <vector>
#include <string>
#include <Headers/SimpleSimulation.h>
#include "Headers/ThreadExecution.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the MainWindow of the application
     * Creates the UI design for the main window. Initializes the drawing scene
     * and all individual UI elements. Primarily handled by Qt
     * @param parent: the parent widget to which this window belongs
     */
    MainWindow(QWidget *parent = nullptr);

    /** Destructor for the MainWindow object, clears all memory*/
    ~MainWindow();

signals:

    /** Signal to halt simulation execution on other threads. Connect all
      Simulation Controllers to this signal and halt their associated Workers
      when it is emitted
    */
    void dropSimulation(QString result);


private slots:
    /**
     * @brief on_runSimulation_clicked()
     * Starts the simulation when the runSimulation button is clicked. Disables
     * UI buttons, creates a new Simulation, creates a new Simulation Controller,
     * and signals to the Simulation Controller to begin execution
     */
    void on_runSimulation_clicked();

    /**
     * @brief on_resetSimulation_clicked()
     * Resets the simulation to its initial conditions when the resetSimulation
     * button is pressed. Re-enables the UI buttons, resets the current
     * simulation that is rendered on the screen, signals to the Simulation
     * Worker to drop the simulation and halt execution to prevent errors
     */
    void on_resetSimulation_clicked();

    /**
     * @brief on_numLocations_valueChanged
     * On Value Changed listener for numLocations slider. Updates the text box
     * with the current value of the numLocations slider so the user can see
     * the exact value of the slider
     * @param value: the new value of the slider
     */
    void on_numLocations_valueChanged(int value);

    /**
     * @brief on_numAgents_valueChanged
     * Slider Listener for numAgents slider. Updates the text box with the
     * current value of the numAgents slider so the user can see the exact
     * value of the slider
     * @param value: the new value of the slider
     */
    void on_numAgents_valueChanged(int value);

    /**
     * @brief on_numLocationsLabel_valueChanged
     * Value listener for the numLocationsLabel Spin box. Enables slider value
     * to change in response to a change in the Spin Box value
     * @param arg1: the new value of the spin box
     */
    void on_numLocationsLabel_valueChanged(int arg1);

    /**
     * @brief on_numAgentsLabel_valueChanged
     * Value listener for the numAgentsLabel Spin box. Enables slider value
     * to change in response to a change in the Spin Box value
     * @param arg1
     */
    void on_numAgentsLabel_valueChanged(int arg1);

private:
    /** Pointer to store and access the ui components of the window*/
    Ui::MainWindow *ui;

    /** Pointer to store and access the Simulation that is running*/
    Simulation* sim;

    /** Controller that executes the Simulation in a separate thread */
    SimulationController* controller;

    /** Maximum numbers for agents and locations */
    static const int MAX_AGENTS = 100;
    static const int MAX_LOCATIONS = 50;

};
#endif // MAINWINDOW_H
