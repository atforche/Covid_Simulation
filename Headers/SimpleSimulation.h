#ifndef SIMPLESIMULATION_H
#define SIMPLESIMULATION_H
/**
 * @brief The SimpleSimulation class
 * Class that manages the progression of the Simple Simulation type
 */

#include <vector>
#include <Headers/mainwindow.h>
#include <ui_mainwindow.h>
#include <Headers/Simulation.h>
#include <QGraphicsRectItem>
#include <Headers/Agent.h>
#include <Headers/SquareRegion.h>
#include <Headers/AgentGenerator.h>
#include "QtTest/QTest"

class Region;


class SimpleSimulation : public Simulation {

private:

    /** Pointers to the four different regions in the simulation */
    Region* workRegion;
    Region* homeRegion;
    Region* leisureRegion;
    Region* schoolRegion;

    /** Generator to handle generation and assignment of Agents to locations */
    AgentGenerator* agentGenerator;

public:

    /**
     * @brief SimpleSimulation
     * The constructor for the SimpleSimulation Class, sets up all controls
     * necessary for the running of this simulation type. Simple Simulation is
     * comprised of four SquareRegions that are home, work, school, and leisure.
     * It will implement agent assignment, motion, and behavior charts, however
     * it will lack any more advanced features like infectivity or economy.
     * @param numAgents: the specified number of agents in the simulation
     */
    SimpleSimulation(int numAgents, Ui::MainWindow* ui);

    /**
     * @brief init
     * Function to initialize the simulation. Initializes the Regions and Locations.
     * Renders all the Regions and Locations and add all of them to the
     * QGraphicsScene. Also generates the agents in the Sim, assigns each of
     * them to a set of Locations, and renders each of them to the QGraphicsScene
     */
    virtual void init() override;

    /**
     * @brief execute
     * Function to execute a simple timestep of the simulation. Loops through
     * every agent in the simulation and calls the update function on them.
     * Simulation::renderAgentUpdate() is required for updates to appear on
     * the screen (may need to be overridden)
     */
    virtual void execute() override;

    /**
     * @brief reset
     * Function to reset the simulation. Resets the Simulation by clearing the
     * Scene and deleting all agents from the Simulation. Redraws the Regions to
     * the screen to the QGraphicsScene does not appear blank to the user.
     */
    virtual void reset() override;

    /**
     * @brief test
     * Test Function that allows for functionalities of the simulation
     * to be tested. Function that can be modified as needed to test various
     * functionalities of the simulation.
     */
    virtual void test() override;

    /**
     * @brief ~SimpleSimulation
     * Destructor function for the Simple Simulation Class. Clears all dynamic
     * memory used within the Simulation by clearing the scene (which clears
     * all memory used by the scene)
     */
    virtual ~SimpleSimulation() override;

    /**
     * @brief generateAgents
     * Function that generates Agents to be added to the simulation. Utilizes
     * the AgentGenerator to randomly generate a set of agents for the
     * simulation and assign them to locations within each Region
     */
    void generateAgents();

};

#endif // SIMPLESIMULATION_H
