#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <random>
#include <map>
#include "ui_mainwindow.h"
#include "AgentController.h"
#include "Location.h"
#include "Region.h"

/**
 * @brief The Simulation class \n
 * The Abstract Simulation class from which all other simulations will be
 * derived. Provides a common interface through which each simulation can be
 * executed in the same screen space with the same driver.
 */
class Simulation : public QObject {

private:
    /** a vector of every agent in the simulation */
    std::vector<Agent*> agents;

    /** Map of various debug information to be passed into the Simulation*/
    std::map<std::string, bool> debug;

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

public:

    /**
     * @brief Simulation:  \n
     * Constructor for the Base Simulation Object. Takes
     * in all parameters that are universal to all simulation types
     * @param numAgents: the number of agents in the simulation
     * @param ui: the main window in which the simulation will runs
     * @param debug: a dict that contains the boolean values of various debug
     *               values;
     */
    Simulation(int numAgents, Ui::MainWindow* ui, std::map<std::string, bool> debug);

    /**
     * @brief init \n
     * Function to initialize the simulation and screen components for the
     * specific simulation type. Needs to be overridden by derived classes
     */
    virtual void init() = 0;

    /**
     * @brief execute \n
     * Function to execute a single timestep in the simulation. Function should
     * be called in a loop by the driver, not called within the class. Needs to
     * be overridden by derived classes
     */
    virtual void execute() = 0;

    /**
     * @brief reset \n
     * Function to reset the simulation and its screen to an initial state.
     * Init needs to be called again before simulation can be run. Needs to
     * be overridden by derived classes
     */
    virtual void reset() = 0;

    /**
     * @brief test \n
     * Function to test each simulation type. Exact purpose of function can
     * change depending on what needs to be tested. Needs to be overridden by
     * derived classes
     */
    virtual void test() = 0;

    /**
     * @brief generateAgents \n
     * Function to generate the agents and assign them to behaviors and locations.
     * Should be overridden by derived classes to customize assignment
     * process to the rules of the Simulation type
     */
    virtual void generateAgents() = 0;

    /**
     * @brief addAgent \n
     * Function to add a new agent to the simulation. Will not
     * add new agent if simulation is already at max number of agents (May
     * need to change in future)
     * @param agent: the agent to be added to the simualtion, as a pointer
     */
    void addAgent(Agent* agent);

    /**
     * @brief getAgents \n
     * Getter function for the vector of all agents in the simulation. Returns
     * a vector of all Agents
     * @return a std::vector containing pointers to every agent in the
     * simulation
     */
    std::vector<Agent*>& getAgents();

    /**
     * @brief addToScreen \n
     * Function to add a QGraphicsItem to the scene on which the simulation
     * is being drawn. Can add any QGraphicsItem. Use this function to add
     * all objects that should be displayed to the simulation
     * @param item: a QGraphicsItem to be added
     */
    void addToScreen(QGraphicsItem* item);

    /**
     * @brief advanceTime \n
     * Function that should be called on every call to Simulation::execute().
     * Advances the time in the simulation. Advances the hours after
     * FRAMES_PER_HOUR number of functions calls. Advances the days after
     * 24 hours have occurred. Advanced the year after 365 years have occurred.
     */
    void advanceTime();

    /**
     * @brief clearScreen \n
     * Calls clear() on the drawing scene on which the simulation is being
     * displayed. Deletes the dynamic memory associated with every object
     * in the scene.
     */
    void clearScreen();

    /**
     * @brief clearAgents \n
     * Calls clear() on the std::vector of Agents to eliminate all of the
     * agents from the simulation
     */
    void clearAgents();

    /**
     * @brief generateLocations \n
     * Function that generates a set of Locations within the specified Region.
     * Generates the number of locations specified in the UI. Ensures all
     * locations generated sit within the bounds of the Region. Generated
     * Locations will be automatically added into the Region
     * @param region: the region to generate Locations for
     */
    void generateLocations(Region* region, int num);

    /**
     * @brief getSimHeight \n
     * Getter function for the height of the QGraphicsScene window. Returns the
     * height as an int
     * @return an int representing the height of the sim
     */
    int getSimHeight();

    /**
     * @brief getSimWidth \n
     * Getter function for the width of the QGraphicsScene window. Returns the
     * width as an int
     * @return an int representing the width of the sim
     */
    int getSimWidth();

    /**
     * @brief getNumAgents \n
     * Getter function for the number of agents in the sim
     * @return the number of agents in the sim
     */
    int getNumAgents();

    /**
     * @brief getYear \n
     * Getter function for the current year in the Sim
     * @return the year as an int
     */
    int getYear();

    /**
     * @brief getDay \n
     * Getter function for the current day in the Sim
     * @return the day as an int
     */
    int getDay();

    /**
     * @brief getHour \n
     * Getter function for the current hour in the Sim
     * @return the hour as an int
     */
    int getHour();

    /**
     * @brief getController \n
     * Getter function for the AgentController driving the Agent's behavior
     * @return the AgentController for the Simulation
     */
    AgentController* getController();

    /**
     * @brief checkDebug \n
     * Checks the debug dict passed into the Simulation for the truth value
     * of the specified string. If the value is true, return true. If the value
     * is false or does not appear, return false.
     * @param val: the debug value to be queried
     * @return true if the valid is true, false otherwise
     */
    bool checkDebug(std::string val);

    /** Destructor for the Simulation Class*/
    virtual ~Simulation();

public slots:
    /**
     * @brief renderAgentUpdate \n
     * Function to render the agents in their new positions following an update.
     * Called by the SimulationController when the SimulationWorker has
     * completed an iteration of the Simulation's execution
     */
    void renderAgentUpdate();

};

#endif // SIMULATION_H
