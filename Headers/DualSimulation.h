#ifndef DUALSIMULATION_H
#define DUALSIMULATION_H

#include "EconomicSimulation.h"
#include "PandemicSimulation.h"
#include "DualController.h"


/**
 * @brief The DualSimulation class \n
 * Class that incorporates both the EconomicSimulation class and the Pandemic
 * Simulation class. Overlaps the functionality between both classes to
 * enable a Pandemic to spread through a population that also has an economy.
 */
class DualSimulation : public PandemicSimulation {

private:


public:

    /**
     * @brief DualSimulation \n
     * Constructor for the DualSimulation class. Initializes a
     * SimpleSimulation. Please update as more things are added
     * @param lagPeriod: period of time until virus is introduced to the population
     * @param initialInfected: number of people who will initially be infected
     * @param initialValue: the initial amount of economic value in the simulation
     * @param numAgents: the initial number of agents in the Simulation
     * @param ui: the MainWindow::ui containing information on all the user inputs
     * @param debug: a map of debug info about the Simulation
     */
    DualSimulation(int lagPeriod, int initialInfected, int initialValue, int numAgents, Ui::MainWindow* ui,
                       std::map<std::string, bool> debug);

    /**
     * @brief execute \n
     * Function to execute a simple timestep of the simulation. Loops through
     * every agent in the simulation and calls the update function on them.
     * Simulation::renderAgentUpdate() is required for updates to appear on
     * the screen.
     */
    virtual void execute() override;

    /**
     * @brief init \n
     * Function to initialize the simulation. Initializes the Regions and Locations.
     * Renders all the Regions and Locations and add all of them to the
     * QGraphicsScene. Also generates the agents in the Sim, assigns each of
     * them to a set of Locations, and renders each of them to the QGraphicsScene
     * @param type: the type of Location to generate, either Simple, Economic, or Pandemic
     */
    virtual void init(std::string type) override;

    /**
     * @brief generateAgents \n
     * Function to generate agents and assign them to behaviors and locations.
     * Currently, randomly assign agents with ages, behaviors, and locations.
     * @param num: number of agents to generate
     */
    virtual void generateAgents(int num, bool birth = false, std::string type = "Dual") override;

    /**
     * @brief renderChart \n
     * Dispatch function to update the necessary Chart on the UI
     * @param which: specific chart that needs to be updated
     * @param newChartView: whether the chart is being moved to a new view
     */
    virtual void renderChartUpdates(QString which, bool newChartView) override;


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

#endif // DUALSIMULATION_H
