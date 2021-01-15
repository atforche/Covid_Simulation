#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <assert.h>
#include "ui_mainwindow.h"
#include "AgentController.h"
#include "LocationGenerator.h"

/**
 * @brief The Simulation class
 * The Abstract Simulation class from which all other simulations will be
 * derived. Provides a common interface through which each simulation can be
 * executed in the same screen space with the same driver.
 */
class Simulation : public QObject {

private:
    /** a vector of every agent in the simulation */
    std::vector<Agent*> agents;

    /** AgentController to dynamically control agent behavior */
    AgentController* agentController;

    /** Integer containing the specified number of agents in the simulation */
    int numAgents;

    /** ui element that allows simulation to inferface with the screen */
    Ui::MainWindow* ui;

    /** Integer that stores the width of the Sims canvas*/
    int simWidth;

    /** Integer that stores the height of the Sims canvas*/
    int simHeight;

    /** LocationGenerator to create locations for each region */
    LocationGenerator* locationGenerator;

    /** Int corresponding to the number of years that have passed in the Sim*/
    int year;

    /** Int corresponding to the number of days that have passed in the
    current year in the Sim*/
    int day;

    /** Int corresponding to the number of hours that have passed in the
    current day in the Sim*/
    int hour;

    /** Number of frames that occur before a new hour begins */
    static const int FRAMES_PER_HOUR = 60;

    /** Number of unique behaviors the simulation is configured for*/
    static const int NUM_BEHAVIORS = 1;

public:

    /**
     * @brief Simulation: Constructor for the Base Simulation Object. Takes
     * in all parameters that are universal to all simulation types
     * @param numAgents: the number of agents in the simulation
     * @param ui: the main window in which the simulation will runs
     */
    Simulation(int numAgents, Ui::MainWindow* ui);

    /**
     * @brief init
     * Function to initialize the simulation and screen components for the
     * specific simulation type. Needs to be overridden by derived classes
     */
    virtual void init() = 0;

    /**
     * @brief execute
     * Function to execute a single timestep in the simulation. Function should
     * be called in a loop by the driver, not called within the class. Needs to
     * be overridden by derived classes
     */
    virtual void execute() = 0;

    /**
     * @brief reset
     * Function to reset the simulation and its screen to an initial state.
     * Init needs to be called again before simulation can be run. Needs to
     * be overridden by derived classes
     */
    virtual void reset() = 0;

    /**
     * @brief test
     * Function to test each simulation type. Exact purpose of function can
     * change depending on what needs to be tested. Needs to be overridden by
     * derived classes
     */
    virtual void test() = 0;

    /**
     * @brief pause
     * Function to pause the simulation. May be handled in driver?
     */
//    virtual void pause() = 0;

    /**
     * @brief resume
     * Function to resume the simulation. May be handled in driver?
     */
//    virtual void resume() = 0;


    /**
     * @brief addAgent: Function to add a new agent to the simulation. Will not
     * add new agent if simulation is already at max number of agents
     * @param agent: the agent to be added to the simualtion, as a pointer
     */
    void addAgent(Agent* agent);

    /**
     * @brief getAgents
     * Getter function for the vector of all agents in the simulation. Returns
     * a vector of all Agents
     * @return a std::vector containing pointers to every agent in the
     * simulation
     */
    std::vector<Agent*>& getAgents();

    /**
     * @brief addToScene
     * Function to add a QGraphicsItem to the scene on which the simulation
     * is being drawn. Can add any QGraphicsItem. Use this function to add
     * all objects that should be displayed to the simulation
     * @param item: a QGraphicsItem to be added
     */
    void addToScene(QGraphicsItem* item);

    /**
     * @brief advanceTime
     * Function that should be called on every call to Simulation::execute().
     * Advances the time in the simulation. Advances the hours after
     * FRAMES_PER_HOUR number of functions calls. Advances the days after
     * 24 hours have occurred. Advanced the year after 365 years have occurred.
     */
    void advanceTime();

    /**
     * @brief clearScene
     * Calls clear() on the drawing scene on which the simulation is being
     * displayed. Deletes the dynamic memory associated with every object
     * in the scene.
     */
    void clearScene();

    /**
     * @brief clearAgents: Calls clear() on the std::vector of Agents
     */
    void clearAgents();

    /**
     * @brief getSimHeight
     * Getter function for the height of the QGraphicsScene window. Returns the
     * height as an int
     * @return an int representing the height of the sim
     */
    int getSimHeight();

    /**
     * @brief getSimWidth
     * Getter function for the width of the QGraphicsScene window. Returns the
     * width as an int
     * @return an int representing the width of the sim
     */
    int getSimWidth();

    /**
     * @brief getGenerator
     * Getter function for the Location Generator within the simulation. Allows
     * Regions to access the LocationGenerator within the Simulation. Returns
     * the LocationGenerator as a pointer
     * @return a pointer to the LocationGenerator
     */
    LocationGenerator* getLocationGenerator();

    /**
     * @brief getNumAgents: Getter function for the number of agents in the sim
     * @return the number of agents in the sim
     */
    int getNumAgents();

    /**
     * @brief getYear
     * Getter function for the current year in the Sim
     * @return the year as an int
     */
    int getYear();

    /**
     * @brief getDay
     * Getter function for the current day in the Sim
     * @return the day as an int
     */
    int getDay();

    /**
     * @brief getHour
     * Getter function for the current hour in the Sim
     * @return the hour as an int
     */
    int getHour();

    /** Destructor for the Simulation Class*/
    virtual ~Simulation();

    /**
     * @brief visualizeChecked
     * Function to check if the visualize check box is checked and return a
     * truth value. Allows Simulation and Agents to know whether the
     * visualization lines need to be drawn.
     * @return
     */
    bool visualizeChecked();

public slots:
    /**
     * @brief renderAgentUpdate
     * Function to render the agents in their new positions following an update.
     * Called by the SimulationController when the SimulationWorker has
     * completed an iteration of the Simulation's execution
     */
    void renderAgentUpdate();

};

#endif // SIMULATION_H
