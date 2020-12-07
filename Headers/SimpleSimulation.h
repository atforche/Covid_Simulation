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
     * @brief SimpleSimulation
     * The constructor for the SimpleSimulation Class, sets up all controls
     * necessary for the running of this simulation type
     * @param numAgents: the specified number of agents in the simulation
     */
    SimpleSimulation(int numAgents, Ui::MainWindow* ui);

    /**
     * @brief init
     * Function to initialize the simulation. More documentation to come
     */
    virtual void init() override;

    /**
     * @brief reset
     * Function to reset the simulation. More documentation to come
     */
    virtual void reset() override;

    /**
     * @brief test
     * Test Function that allows for functionalities of the simulation
     * to be tested
     */
    virtual void test() override;

    /**
     * @brief ~SimpleSimulation
     * Destructor function for the Simple Simulation Class. Clears all dynamic
     * memory used within the simulation
     */
    virtual ~SimpleSimulation();

    /**
     * @brief generateAgents
     * Function that generates Agents to be added to the simulation
     */
    void generateAgents(Agent* agent);

};

#endif // SIMPLESIMULATION_H
