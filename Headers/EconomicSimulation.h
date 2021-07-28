#ifndef ECONOMICSIMULATION_H
#define ECONOMICSIMULATION_H

#include "ui_mainwindow.h"

#include "Simulation.h"
#include "SimpleSimulation.h"
#include "EconomicAgent.h"
#include "EconomicController.h"


/**
 * @brief The EconomicSimulation class
 * Class that inherits from the Simple Simulation class. Extends the Simple
 * Simulation to include economic rules. Agents are able to gain and lose value.
 * Locations can gain value, lose value, and go out of business.
 */
class EconomicSimulation : public SimpleSimulation {

private:

    /** Double to track the total Economic Value in the Simulation */
    double totalEconomicValue;

    /** Double to track the total Economic Value held by businesses*/
    double businessEconomicValue;

    /** Double to track the total Economic Value held by agents */
    double agentEconomicValue;

    /** Pointer to a helper for rendering the Agent Value chart*/
    AgentValueChartHelper *agentValueHelper;

    /** Pointer to a helper for rendering the Work Value chart*/
    WorkValueChartHelper *workValueHelper;

    /** Pointer to a helper for rendering the Total Value chart */
    TotalValueChartHelper *totalValueHelper;

    /** Pointer to a controller to manage all economic aspects of simulation */
    EconomicController* economicController;

    /** Pointer to a helper for rendering the Economic Status chart*/
    EconomicStatusChartHelper* statusHelper;

public:
    
    /**
     * @brief EconomicSimulation \n
     * Constructor for the EconomicSimulation class. Initializes a
     * SimpleSimulation. Please update as more things are added
     * @param initialValue: the initial amount of economic value in the simulation
     * @param numAgents: the initial number of agents in the Simulation
     * @param ui: the MainWindow::ui containing information on all the user inputs
     * @param debug: a map of debug info about the Simulation
     */
    EconomicSimulation(int initialValue, int numAgents, Ui::MainWindow* ui,
                       std::map<std::string, bool> debug);

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
     * @brief getCurrentValue \n
     * Getter function for the current value in the Simulation
     * @return the current value in the simulation as an int
     */
    int getCurrentValue();

    /**
     * @brief renderChart \n
     * Dispatch function to update the necessary Chart on the UI
     * @param which: specific chart that needs to be updated
     * @param newChartView: whether the chart is being moved to a new view
     */
    virtual void renderChartUpdates(QString which, bool newChartView) override;

    /**
     * @brief updateEconomicCharts \n
     * Wrapper function to automatically update all charts that display
     * all economic information
     */
    void updateEconomicCharts();

    /**
     * @brief getBusinessThresholds \n
     * Getter function for the individual value thresholds that Businesses are
     * measured against. Theses thresholds help determine if the Business is
     * a Low, Medium, or High value business location.
     * @return a vector of value thresholds
     */
    std::vector<int>& getBusinessThresholds();

    /**
     * @brief getNumHomelessAgents \n
     * Getter function for the number of homeless agents that are currently
     * in the simulation
     * @return the number of homeless agents as an int
     */
    int getNumHomelessAgents();

    /**
     * @brief getHomelessAgents \n
     * Getter function for the set of homeless agents in the simulation
     * @return the homeless agents in the simulation as a vector
     */
    std::vector<Agent*> getHomelessAgents();

    /**
     * @brief getNumUnemployedAgents \n
     * Getter function for the number of unemployed agents that are currently
     * in the simulation
     * @return the number of unemployed agents as an int
     */
    int getNumUnemployedAgents();

    /**
     * @brief getUnemployedAgents \n
     * Getter function for the set of unemployed agents in the simulation
     * @return the unemployed agents in the simulation as a vector
     */
    std::vector<Agent*> getUnemployedAgents();

    /** Destructor for the Economic Simulation class */
    ~EconomicSimulation();

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

#endif // ECONOMICSIMULATION_H
