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
#include "QtTest/QTest"

class Region;


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
    SimpleSimulation(int numAgents, Ui::MainWindow* ui);

    /**
     * @brief init \n
     * Function to initialize the simulation. Initializes the Regions and Locations.
     * Renders all the Regions and Locations and add all of them to the
     * QGraphicsScene. Also generates the agents in the Sim, assigns each of
     * them to a set of Locations, and renders each of them to the QGraphicsScene
     */
    virtual void init() override;

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
     * @brief test \n
     * Test Function that allows for functionalities of the simulation
     * to be tested. Function that can be modified as needed to test various
     * functionalities of the simulation.
     */
    virtual void test() override;

    /**
     * @brief generateAgents \n
     * Function to generate agents and assign them to behaviors and locations.
     * Currently, randomly assign agents with ages, behaviors, and locations.
     */
    virtual void generateAgents() override;

    /**
     * @brief ~SimpleSimulation \n
     * Destructor function for the Simple Simulation Class. Clears all dynamic
     * memory used within the Simulation by clearing the scene (which clears
     * all memory used by the scene)
     */
    virtual ~SimpleSimulation() override;

};

#endif // SIMPLESIMULATION_H
