#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

#include "QMainWindow"
#include "ui_mainwindow.h"
#include "QGraphicsRectItem"
#include "QtTest/QTest"
#include "QJsonDocument"
#include "QJsonObject"

#include "Headers/SimpleSimulation.h"
#include "Headers/ThreadExecution.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


/**
 * @brief The MainWindow class
 * Main Window that drives the simulation. Contains all Slots that connect
 * to user inputs. Passes user input on to the Simulation. Contains a
 * SimulationController that manages the runtime speed of the Simulation.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    /** Pointer to store and access the ui components of the window*/
    Ui::MainWindow *ui;

    /** Pointer to store and access the Simulation that is running*/
    Simulation* sim;

    /** Controller that executes the Simulation in a separate thread */
    SimulationController* controller;

    /** Maximum numbers for agents and locations */
    static const int MAX_AGENTS = 1000;
    static const int MAX_LOCATIONS = 50;

    /** String specifying the current speed of the simualation */
    QString currentSpeed;

    /** Bool specifying if the simulation is paused */
    bool paused;

    /** List of possible graph views */
    std::vector<QString> graphViews = {"Age Distribution",
                                       "Behavior Chart Distribution",
                                       "Destination Assignments"};

public:
    /**
     * @brief MainWindow \n
     * Creates the UI design for the main window. Initializes the drawing scene
     * and all individual UI elements. Primarily handled by Qt
     * @param parent: the parent widget to which this window belongs
     */
    MainWindow(QWidget *parent = nullptr);

    /** Destructor for the MainWindow object, clears all memory*/
    ~MainWindow();

    /**
     * @brief disableUI \n
     * Function that disables all necessary UI elements when the Simulation
     * is started. Prevents user from changing constant values while the
     * simulation is in execution.
     */
    void disableUI();

    /**
     * @brief enableUI \n
     * Function that enables all the necessary UI elements when the Simulation
     * is ended. Allows the user to be able to change constant values in
     * between Simulation executions
     */
    void enableUI();

    /**
     * @brief initializeComboBoxes \n
     * Initialize each of the Combo Boxes with their initial values and
     * list of possible values.
     */
    void initializeComboBoxes();

    /**
     * @brief disableComboBoxOptions \n
     * Disables and enables different options in the ComboBoxes to ensure that
     * there are never duplicate options selected
     */
    void disableComboBoxOptions();

    /**
     * @brief checkDebugInfo \n
     * Function that checks the state of various UI components that specify
     * debug states and info. Returns a std::map that makes these pieces of
     * debug info to their respective truth value.
     * @return
     */
    std::map<std::string, bool> checkDebugInfo();

private slots:
    /**
     * @brief on_runSimulation_clicked \n
     * Starts the simulation when the runSimulation button is clicked. Disables
     * UI buttons, creates a new Simulation, creates a new Simulation Controller,
     * and signals to the Simulation Controller to begin execution
     */
    void on_runSimulation_clicked();

    /**
     * @brief on_resetSimulation_clicked \n
     * Resets the simulation to its initial conditions when the resetSimulation
     * button is pressed. Re-enables the UI buttons, resets the current
     * simulation that is rendered on the screen, signals to the Simulation
     * Worker to drop the simulation and halt execution to prevent errors
     */
    void on_resetSimulation_clicked();

    /**
     * @brief on_numLocations_valueChanged \n
     * On Value Changed listener for numLocations slider. Updates the text box
     * with the current value of the numLocations slider so the user can see
     * the exact value of the slider
     * @param value: the new value of the slider
     */
    void on_numLocations_valueChanged(int value);

    /**
     * @brief on_numAgents_valueChanged \n
     * Slider Listener for numAgents slider. Updates the text box with the
     * current value of the numAgents slider so the user can see the exact
     * value of the slider
     * @param value: the new value of the slider
     */
    void on_numAgents_valueChanged(int value);

    /**
     * @brief on_numLocationsLabel_valueChanged \n
     * Value listener for the numLocationsLabel Spin box. Enables slider value
     * to change in response to a change in the Spin Box value
     * @param arg1: the new value of the spin box
     */
    void on_numLocationsLabel_valueChanged(int arg1);

    /**
     * @brief on_numAgentsLabel_valueChanged \n
     * Value listener for the numAgentsLabel Spin box. Enables slider value
     * to change in response to a change in the Spin Box value
     * @param arg1
     */
    void on_numAgentsLabel_valueChanged(int arg1);

    /**
     * @brief on_slowSim_clicked \n
     * Click Listener for the slowSim button. Reduces the execution speed of
     * the simulation by one stage. Different stages are Slow, Normal, Fast
     * and Unlimited. Updates the speed label and has the simulation controller
     * update the worker with the new speed
     */
    void on_slowSim_clicked();

    /**
     * @brief on_pauseSim_clicked \n
     * Click Listener for the pauseSim button. Stops the execution of the
     * Simulation by the Simulation Controller. Updates the speed label on
     * the screen
     */
    void on_pauseSim_clicked();

    /**
     * @brief on_fastSim_clicked \n
     * Click Listener for the slowSim button. Reduces the execution speed of
     * the simulation by one stage. Different stages are Slow, Normal, Fast
     * and Unlimited. Updates the speed label and has the simulation controller
     * update the worker with the new speed
     */
    void on_fastSim_clicked();

    /**
     * @brief on_graph1Selection_currentTextChanged \n
     * Event handler for when the current selection on the Graph1Selection
     * Combo Box is changed. Updates the mapping of Graphs to Chart Views and
     * re-renders the graphs so they update.
     * @param arg1: the new selected value
     */
    void on_graph1Selection_currentTextChanged(const QString &arg1);

    /**
     * @brief on_graph2Selection_currentTextChanged \n
     * Event handler for when the current selection on the Graph2Selection
     * Combo Box is changed. Updates the mapping of Graphs to Chart Views and
     * re-renders the graphs so they update.
     * @param arg1: the new selected value
     */
    void on_graph2Selection_currentTextChanged(const QString &arg1);

    /**
     * @brief on_framesPerHourSlider_valueChanged \n
     * Event handler for when the slider value for the framesPerHour slider
     * changes. Updates the text in the associated SpinBox and updates the speed
     * of the Simulation
     * @param value: new value of the slider
     */
    void on_framesPerHourSlider_valueChanged(int value);

    /**
     * @brief on_framesPerHour_valueChanged \n
     * Event handler for when the SpinBox value for the framesPerHour SpinBox
     * changes. Updates the text for the associated slider and updates the speed
     * of the Simulation
     * @param arg1: new value of the SpinBox
     */
    void on_framesPerHour_valueChanged(int arg1);

};
#endif // MAINWINDOW_H
