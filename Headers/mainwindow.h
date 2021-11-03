#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

#include "QMainWindow"
#include "QGraphicsRectItem"
#include "QtTest/QTest"
#include "QJsonDocument"
#include "QJsonObject"

#include "ui_mainwindow.h"
#include "Headers/SimulationExecution.h"
#include "Headers/SimpleSimulation.h"

#include "Headers/Network.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Forward Declarations
class TrainingController;


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

    /** Controller that executes the Training in a separate thread */
    TrainingController* trainingController;

    /** Maximum numbers for agents and locations */
    static const int MAX_AGENTS = 1000;
    static const int MAX_LOCATIONS = 100;

    /** String specifying the current speed of the simualation */
    QString currentSpeed;

    /** Bool specifying if the simulation is paused */
    bool paused;

    /** List of possible graph views */
    std::vector<QString> graphViews = {"Age Distribution",
                                       "Behavior Chart Distribution",
                                       "Destination Assignments"};

    /** A network loaded from a file */
    PolicyNetwork network;

    /** Boolean to indicate if a network was loaded*/
    bool networkLoaded;

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
     * @brief showEconomicOptions \n
     * Function that makes the options relevant to the Economic Simulation type
     * appear or disappear from the screen. Disables them all immediately after
     * they are shown or hidden. Options can be enabled with a call to
     * enableEconomicOptions.
     * @param show: whether to show or hide the economic options
     */
    void showEconomicOptions(bool show);

    /**
     * @brief enableEconomicOptions \n
     * Function that enables and disables all of the options relevant to the
     * Economic Simulation type. Status the options will be set to is determined
     * by the function parameter.
     * @param enabled: whether to enable or disable to economic options
     */
    void enableEconomicOptions(bool enabled);

    /**
     * @brief showEconomicCharts \n
     * Function to enable or disable the economic charts. If the economic charts
     * are enabled, the options will appear in the ComboBoxes and the default
     * chart views for the Economic Simulation will be applied. If the economic
     * charts are disabled, the options will be removed in the Combo Boxes and
     * the default chart views for the Simple Simulation will be applied
     * @param enabled: whether to enable or disable to Economic charts
     */
    void enableEconomicCharts(bool enabled);

    /**
     * @brief showPandemicOptions \n
     * Function that makes the options relevant to the Pandemic Simulation type
     * appear or disappear from the screen. Disables them all immediately after
     * they are shown or hidden, Options can be enables with a call to
     * enablePandemicOptions.
     * @param show: whether to show or hide the economic options
     */
    void showPandemicOptions(bool show);

    /**
     * @brief enablePandemicOptions \n
     * Function that enables and disables all of the options relevent to the
     * Pandemic Simualtion type. Status the options will be set to is determined
     * by the function parameter
     * @param enabled: whether to enable is disable the Pandemic options
     */
    void enablePandemicOptions(bool enabled);

    /**
     * @brief enablePandemicCharts \n
     * Function to enable or disable the Pandemic Charts. If the pandemic charts,
     * are enabled, the options will appear in the ComboBoxes and the default
     * chart views for the Pandemic Simulation will be applied. If the Pandemic
     * charts are disabled, the options will be removed in the ComboBoxes and
     * the default chart views for the Simple Simualtion will be applied.
     * @param enabled: whether to enable or disable the Pandemic Charts
     */
    void enablePandemicCharts(bool enabled);

    /**
     * @brief showDualOptions \n
     * Function that makes the options relevant to the Dual Simulation type
     * appear or disappear from the screen. Disables them all immediately after
     * they are shown or hidden, Options can be enables with a call to
     * enablePandemicOptions.
     * @param show: whether to show or hide the economic options
     */
    void showDualOptions(bool show);

    /**
     * @brief enableDualOptions \n
     * Function that enables and disables all of the options relevent to the
     * Dual Simualtion type. Status the options will be set to is determined
     * by the function parameter
     * @param enabled: whether to enable is disable the Pandemic options
     */
    void enableDualOptions(bool enabled);

    /**
     * @brief enableDualCharts \n
     * Function to enable or disable the Dual Charts. If the dual charts,
     * are enabled, the options will appear in the ComboBoxes and the default
     * chart views for the Dual Simulation will be applied. If the Dual
     * charts are disabled, the options will be removed in the ComboBoxes and
     * the default chart views for the Simple Simualtion will be applied.
     * @param enabled: whether to enable or disable the Pandemic Charts
     */
    void enableDualCharts(bool enabled);

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
    static std::map<std::string, bool> checkDebugInfo(Ui::MainWindow* ui);

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
     * @brief on_graph3Selection_currentTextChanged \n
     * Event handler for when the current selection on the Graph2Selection
     * Combo Box is changed. Updates the mapping of Graphs to Chart Views and
     * re-renders the graphs so they update.
     * @param arg1: the new selected value
     */
    void on_graph3Selection_currentTextChanged(const QString &arg1);

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

    /**
     * @brief on_simulationType_currentTextChanged \n
     * Event handler for when the ComboBox value for the SimulationType Combobox
     * changes. Updates the current Simulation Type, creates a new Simulation,
     * and shows/hides the correct screen options.
     * @param arg1: the new Simulation Type
     */
    void on_simulationType_currentTextChanged(const QString &arg1);

    /**
     * @brief on_initialValue_valueChanged \n
     * Event handler for when the SpinBox for the Initial Value changes. Updates
     * the value in the slider to keep them in sync.
     * @param arg1: the new value of the SpinBox
     */
    void on_initialValue_valueChanged(int arg1);

    /**
     * @brief on_initialValueSlider_valueChanged \n
     * Event handler for when the Slider for the Initial Value changes. Updates
     * the value in the SpinBox to keep them in sync.
     * @param arg1: the new value of the slider
     */
    void on_initialValueSlider_valueChanged(int arg1);

    /**
     * @brief on_lagPeriod_valueChanged \n
     * Event handler for when the SpinBox for the Lag Period changes. Updates
     * the value in the slider to keep them in sync.
     * @param arg1: the new value of the SpinBox
     */
    void on_lagPeriod_valueChanged(int arg1);

    /**
     * @brief on_lagPeriodSlider_valueChanged \n
     * Event handler for when the Slider for the Lag Period changes. Updates
     * the value of the SpinBox to keep them in sync.
     * @param value: the new value of the Slider
     */
    void on_lagPeriodSlider_valueChanged(int value);

    /**
     * @brief on_initialInfected_valueChanged \n
     * Event handler for when the SpinBox for the Initial Infected changes.
     * Updates the value of the Slider to keep them in sync.
     * @param arg1: the new value of the SpinBox
     */
    void on_initialInfected_valueChanged(int arg1);

    /**
     * @brief on_initialInfectedSlider_valueChanged \n
     * Event handler for when the Slider for the Initial Infected changes.
     * Updates the valueof the SpinBox to keep them in sync.
     * @param value: the new value of the Slider
     */
    void on_initialInfectedSlider_valueChanged(int value);

    /**
     * @brief on_hospitalCapacity_valueChanged \n
     * Event handler for when the SpinBox for the Initial Infected changes.
     * Updates the value of the Slider to keep them in sync.
     * @param arg1: the new value of the SpinBox
     */
    void on_hospitalCapacity_valueChanged(int arg1);

    /**
     * @brief on_hospitalCapacitySlider_valueChanged \n
     * Event handler for when the Slider for the Initial Infected changes.
     * Updates the valueof the SpinBox to keep them in sync.
     * @param value: the new value of the Slider
     */
    void on_hospitalCapacitySlider_valueChanged(int value);

    /**
     * @brief on_trainNetwork_clicked \n
     * Function to handle the training of a Neural Network to control the
     * Simulation
     */
    void on_startTraining_clicked();

    /**
     * @brief on_loadNetwork_clicked \n
     * Function to handle the loading of a trained network;
     */
    void on_loadNetwork_clicked();

    /**
     * @brief on_evaluateNetwork_clicked \n
     * Function to handle the evaluation of a trained network;
     */
    void on_evaluateNetwork_clicked();

public slots:

    /**
     * @brief printMessage \n
     * Prints a message to the console
     * @param message
     */
    static void printMessage(const QString &message);

    /** Force the application to update the screen */
    void forceUpdate();

};

#endif // MAINWINDOW_H
