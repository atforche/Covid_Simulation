#ifndef SIMPLESIMULATION_H
#define SIMPLESIMULATION_H

#include <vector>

#include <ui_mainwindow.h>
#include <QGraphicsRectItem>
#include <QtTest/QTest>

#include <Headers/Agent.h>
#include <Headers/SquareRegion.h>
#include <Headers/mainwindow.h>
#include <Headers/Simulation.h>

// Forward declaration of the Region Class
class Region;


/**
 * @brief The SimpleSimulation class
 * Class that manages the progression of the Simple Simulation type
 */
class SimpleSimulation : public Simulation {

private:

    /** Pointers to the four different regions in the simulation */
    Region* workRegion;
    Region* homeRegion;
    Region* leisureRegion;
    Region* schoolRegion;

public:

    /**
     * @brief SimpleSimulation \n
     * The constructor for the SimpleSimulation Class, sets up all controls
     * necessary for the running of this simulation type. Simple Simulation is
     * comprised of four SquareRegions that are home, work, school, and leisure.
     * It will implement agent assignment, motion, and behavior charts, however
     * it will lack any more advanced features like infectivity or economy.
     * @param numAgents: the specified number of agents in the simulation
     */
    SimpleSimulation(int numAgents, Ui::MainWindow* ui,
                     std::map<std::string, bool> debug);

    /**
     * @brief ~SimpleSimulation \n
     * Destructor function for the Simple Simulation Class. Clears all dynamic
     * memory used within the Simulation by clearing the scene (which clears
     * all memory used by the scene)
     */
    virtual ~SimpleSimulation() override;

    /**
     * @brief init \n
     * Function to initialize the simulation. Initializes the Regions and Locations.
     * Renders all the Regions and Locations and add all of them to the
     * QGraphicsScene. Also generates the agents in the Sim, assigns each of
     * them to a set of Locations, and renders each of them to the QGraphicsScene
     * @param type: the type of Location to generate, either Simple or Economic
     */
    virtual void init(std::string type = "Simple") override;

    /**
     * @brief execute \n
     * Function to execute a simple timestep of the simulation. Loops through
     * every agent in the simulation and calls the update function on them.
     * Simulation::renderAgentUpdate() is required for updates to appear on
     * the screen (may need to be overridden)
     */
    virtual void execute() override;

    /**
     * @brief reset \n
     * Function to reset the simulation. Resets the Simulation by clearing the
     * Scene and deleting all agents from the Simulation. Redraws the Regions to
     * the screen to the QGraphicsScene does not appear blank to the user.
     */
    virtual void reset() override;

    /**
     * @brief generateAgents \n
     * Function to generate agents and assign them to behaviors and locations.
     * Currently, randomly assign agents with ages, behaviors, and locations.
     * @param num: number of agents to generate
     * @param type: the type of agent to generate, either "Simple" or "Economic"
     */
    virtual void generateAgents(int num, bool birth = false, std::string type = "Simple") override;

    /**
     * @brief getRandomLocation \n
     * Returns a pointer to a random location which type is determines by the
     * which parameter
     * @param which: the type of location pointer to return
     * @return a pointer to a random location
     */
    virtual Location* getRandomLocation(Agent::LOCATIONS which) override;

    /**
     * @brief getRegion \n
     * Virtual method for getting a specific region from the Simulation. Must be
     * overridden by inheriting classes
     * @param which: which Region to return
     * @return a pointer to the specified region
     */
    virtual Region* getRegion(Agent::LOCATIONS which) override;

    /**
     * @brief getAllLocations \n
     * Getter function that returns a vector containing all Locations in the
     * entire Simulation
     * @return a vector of Location*
     */
    std::vector<Location*> getAllLocations();

public slots:

    /**
     * @brief renderCharts \n
     * Renders each of the appriopriate graph types to the screen in their
     * designated chartViews
     * @param which: QString representing which graph type to render
     * @param newChartView: whether the Chart is being moved to a new view or just updated
     */
    virtual void renderCharts(const QString &which, bool newChartView) override;

};

#endif // SIMPLESIMULATION_H
