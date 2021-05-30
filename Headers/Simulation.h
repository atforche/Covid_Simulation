#ifndef SIMULATION_H
#define SIMULATION_H

#include "QObject"

#include <vector>
#include <random>
#include <map>
#include <unordered_map>

#include "ui_mainwindow.h"
#include "AgentController.h"
#include "Location.h"
#include "Region.h"
#include "ChartHelpers.h"


/**
 * @brief The Simulation class \n
 * The Abstract Simulation class from which all other simulations will be
 * derived. Provides a common interface through which each simulation can be
 * executed in the same screen space with the same driver.
 */
class Simulation : public QObject {
    Q_OBJECT

private:
    /** a vector of every agent in the simulation */
    std::vector<Agent*> agents;

    /** Map of various debug information to be passed into the Simulation*/
    std::map<std::string, bool> debug;

    /** AgentController to dynamically control agent behavior */
    AgentController* agentController;

    /** ui element that allows simulation to inferface with the screen */
    Ui::MainWindow* ui;

    /** Integer containing the specified number of agents in the simulation */
    int numAgents;

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

    /** Maps each distribution type to the Chart View(s) it appears in */
    std::unordered_map<QString, int>* chartViews;

    // Default QGraphs to populate the QChartViews
    static QtCharts::QChart* default1;

    /** Classes of all dynamic memory components for each graph */
    AgeChartHelper* ageHelper;
    BehaviorChartHelper* behaviorHelper;

    /**
     * @brief ageAgents \n
     * Increments the age of each agent in the Simulation by one. If the
     * agent has become an adult, select an Adult behavior chart for the
     * agent and update it.
     */
    void ageAgents();

    /**
     * @brief addChartToView \n
     * Takes the chart provided as input and updates the Chart for
     * the designated chartView in the UI to the provided chart
     * @param chart: the chart that should be visualized
     * @param num: the ID of the chartView to update with the new chart
     */
    void addChartToView(QChart* chart, int num);

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

    /** Destructor for the Simulation Class*/
    virtual ~Simulation();

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
     * Additionally, updates each agent's behavior chart after an hour has passed
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

    /**
     * @brief getChartView \n
     * Determines which graph types should appear in each chartView according
     * to the Combox Boxes on the MainWindow. Populates these mappings in the
     * Simulation::chartViews map.
     */
    void mapChartViews();

    /**
     * @brief getChartView \n
     * Returns the chartView number that the specified graph type will appear
     * in on the MainWindow. If the specified graph type will not appear,
     * returns -1
     * @param type: the specific graph type to display
     * @return an integer representing with chartView that graph type appears in
     */
    int getChartView(QString type);

    /**
     * @brief renderAgeChart \n
     * Renders the Age Distribution chart to the screen with the current
     * age distribution of the agents in the simulation
     * @param newChartView: whether the Chart is being moved to a new view
     */
    void renderAgeChart(bool newChartView);

    /**
     * @brief renderBehaviorChart \n
     * Renders the Behavior Chart visualization to the screen with the current
     * behavior assignments of the Agents.
     * @param newChartView: whether the Chart is being moved to a new view
     */
    void renderBehaviorChart(bool newChartView);



public slots:
    /**
     * @brief renderAgentUpdate \n
     * Function to render the agents in their new positions following an update.
     * Called by the SimulationController when the SimulationWorker has
     * completed an iteration of the Simulation's execution
     */
    void renderAgentUpdate();

    /**
     * @brief renderCharts \n
     * Renders each of the relevant chart to the screen
     * @param which: QString that specifies which of the charts should be rendered
     */
    virtual void renderCharts(const QString &which, bool newChartView) = 0;


signals:

    /**
     * @brief updateChart \n
     * SignaL emitted by the simulation when one of the charts needs to be updated.
     * Enables any of the charts to be updated dynamically.
     */
    void updateChart(const QString &which, bool newChartView);


};

#endif // SIMULATION_H
