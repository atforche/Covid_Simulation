#ifndef PANDEMICSIMULATION_H
#define PANDEMICSIMULATION_H

#include "ui_mainwindow.h"

#include "Simulation.h"
#include "EconomicSimulation.h"
#include "PandemicController.h"

/**
 * @brief The PandemicSimulation class
 * Class that inherits from the Simple Simulation class. Extends the Simple
 * Simulation to include pandemic rules. The pandemic rules are based on the
 * SEIR infectivity model.
 */
class PandemicSimulation : public EconomicSimulation {

private:

    /** Number of agents that are Susceptible in the Simulation */
    int numSusceptible;

    /** Number of agents that are Exposed in the Simulation */
    int numExposed;

    /** Number of agents that are Infected in the Simulation */
    int numInfected;

    /** Number of agents that are Recovered in the Simulation */
    int numRecovered;

    /** Number of days before an infected individual will appear in the Simulation */
    int lagPeriod;

    /** Number of agents to become infected in the initial infection */
    int initialInfected;

    /** Controller class to handle enforcing all Pandemic rules */
//    PandemicController* controller;

    /** Pointer to helper for rendering SEIR Line Chart */
    SEIRChartHelper* SEIRHelper;

    /** Pointer to helper for rendering DailyTracker Line Chart*/
    DailyTrackerChartHelper* dailyTrackerHelper;

    // Status colors for each Pandemic Status
    static constexpr QColor SUSCEPTIBLECOLOR = QColor(235, 220, 52);
    static constexpr QColor EXPOSEDCOLOR = QColor(235, 143, 52);
    static constexpr QColor INFECTEDCOLOR = QColor(255, 0, 0);
    static constexpr QColor RECOVEREDCOLOR = QColor(27, 163, 3);


public:

    /**
     * @brief PandemicSimulation \n
     * Constructor for the PandemicSimulation class. Initializes a
     * SimpleSimulation. Please update as more things are added
     * @param lagPeriod: the number of days to pass before infecting agents
     * @param initialInfected: the number of agents that will become infected in the initial infection
     * @param numAgents: the initial number of agents in the Simulation
     * @param ui: the MainWindow::ui containing information on all the user inputs
     * @param debug: a map of debug info about the Simulation
     */
    PandemicSimulation(int lagPeriod, int initialInfected, int initialValue, int numAgents,
                       Ui::MainWindow* ui, std::map<std::string, bool> debug);

    /**
     * @brief execute \n
     * Function to execute a simple timestep of the simulation. Loops through
     * every agent in the simulation and calls the update function on them.
     * Simulation::renderAgentUpdate() is required for updates to appear on
     * the screen. Additionally calculates changes in economic value for
     * each agent.
     */
    virtual void execute() override;

    /**
     * @brief init \n
     * Function to initialize the simulation. Initializes the Regions and Locations.
     * Renders all the Regions and Locations and add all of them to the
     * QGraphicsScene. Also generates the agents in the Sim, assigns each of
     * them to a set of Locations, and renders each of them to the QGraphicsScene
     * @param type: the type of Location to generate, either Simple or Economic
     */
    virtual void init(std::string type) override;

    /**
     * @brief generateAgents \n
     * Function to generate agents and assign them to behaviors and locations.
     * Currently, randomly assign agents with ages, behaviors, and locations.
     * @param num: number of agents to generate
     */
    virtual void generateAgents(int num, bool birth = false, std::string type = "Simple") override;

    /**
     * @brief renderChartUpdates \n
     * Dispatch function to update the necessary Chart on the UI
     * @param which: specific chart that needs to be updated
     * @param newChartView: whether the chart is being moved to a new view
     */
    virtual void renderChartUpdates(QString which, bool newChartView) override;

    /**
     * @brief getNumSEIR \n
     * Getter function for the number of agents that are Suseptible, Exposed,
     * Infected, and Recovered
     * @return the number of SEIR agents as a vector
     */
    std::vector<int> getNumSEIR();

    /**
     * @brief getLagPeriod \n
     * Getter function for the Lag Period, or the number of Simulation days that
     * will pass before the initial infection
     * @return the lag period as an int
     */
    int getLagPeriod();

    /**
     * @brief getInitialInfected \n
     * Getter function for the number of Agents that should become infected as a
     * part of the initial infection.
     * @return the number of initial infected as an int
     */
    int getInitialInfected();

    /**
     * @brief updatePandemicCharts \n
     * Wrapper function to automatically update all charts that display
     * all Pandemic information
     * @param newDay: whether or not a new day has passed in the Simulation
     */
    void updatePandemicCharts(bool newDay);

    /**
     * @brief getTotalCases \n
     * Returns the total number of cases that have occurred in the Simulation
     * @return
     */
    int getTotalCases();

    /**
     * @brief getTotalDeaths \n
     * Returns the total number of deaths that have occurred in the Simulation
     * @return
     */
    int getTotalDeaths();

    /** Destructor for the Pandemic Simulation class */
    ~PandemicSimulation();

public slots:

    /**
     * @brief renderAgentUpdate \n
     * Function to render the agents in their new positions following an update.
     * Called by the SimulationController when the SimulationWorker has
     * completed an iteration of the Simulation's execution
     */
    virtual void renderAgentUpdate() override;

    /**
     * @brief renderCharts \n
     * Renders each of the relevant chart to the screen
     * @param which: QString that specifies which of the charts should be rendered
     */
    virtual void renderCharts(const QString &which, bool newChartView) override;

};


#endif // PANDEMICSIMULATION_H
