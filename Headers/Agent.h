#ifndef AGENT_H
#define AGENT_H

/**
 * @brief The Agent Class
 * Class that specifies the functionality of the Agents in the simulation
 * Contains all information required for the agent to act in the environment
 */

#include <Headers/Coordinate.h>
#include <QPen>
#include <QGraphicsRectItem>
#include "Location.h"

class Agent {

public:

    /** Enum that specifies the possible locations for the Agent to be */
    enum LOCATIONS {HOME, SCHOOL, WORK, LEISURE};

    /**
     * @brief Agent
     * Constructor for the Agent Class. Constructs an Agent object with a
     * certain age, position, and visualization. Agent must be added to a
     * simulation, through which it will be controlled
     * @param age: the current age of the Agent
     */
    Agent(int age, Coordinate* position, bool visualize = false);

    /**
     * @brief renderAgent
     * Function to generate the initial QGraphicsRectItem that represents this
     * specific agent. Return value should be stored within Simulation
     * and added to UI element within simulation. Updates to the agent's
     * position should be made using the update() function, and the rendering
     * can be updated through the renderUpdate() function.
     */
    QGraphicsRectItem* renderAgent();

    /**
     * @brief renderUpdate
     * Function that renders the agent and its associated
     * components in their updated positions. Must be called from main thread
     * after update() has been called on the agent. Allows the agents position
     * to be updated in a separate thread and the rendering to be updated in
     * the main thread.
     */
    void renderUpdate();

    /**
     * @brief update
     * Function to update the position of the Agent. The function renderUpdate()
     * must be called in the main thread for the update to appear. Can be
     * called in a secondary thread to prevent blocking of the GUI thread.
     */
    void update();

    /**
     * @brief setDestination
     * Sets the destination of this Agent to the new Location specified
     * @param newLocation: a pointer to the new Location to set the agents
     *                     destination to
     */
    void setDestination(Location* newLocation);

    /**
     * @brief setLocation
     * Function to set the value of a Location assigned to the agent to a
     * specific location
     * @param location: a pointer to the new location
     * @param which: specifies which of the Agent's locations to overwrite
     * (Home, School, Leisure, Work)
     */
    void setLocation(Location* location, LOCATIONS which);

    /**
     * @brief getLocation
     * Function to get a pointer to one of the Agents assigned locations
     * @param which: specifies which of the Agent's locations to return (Home,
     * School, Leisure, Work)
     * @return the specified location as a pointer
     */
    Location* getLocation(LOCATIONS which);

    /**
     * @brief setHome
     * Setter function for the agent's home Location. Changes the home location
     * to which the agent is assigned to a new location
     * @param home: the home Location the agent is assigned to
     */
    void setHome(Location* home);

    /**
     * @brief getHome
     * Getter function for the agent's home Location. Returns the home location
     * of the agent
     * @return the agent's home Location
     */
    Location* getHome();

    /**
     * @brief setSchool
     * Setter function for the agent's school Location. Changes the school
     * location to which the agent is assigned to a new location
     * @param school: the school Location the agent is assigned to
     */
    void setSchool(Location* school);

    /**
     * @brief getSchool
     * Getter function for the agent's school Location. Returns the school
     * location of the agent
     * @return the agent's school Location
     */
    Location* getSchool();

    /**
     * @brief setWork:
     * Setter function for the agent's work Location. Changes the work location
     * to which the agent is assigned to a new location
     * @param work: the work Location the agent is assigned to
     */
    void setWork(Location* work);

    /**
     * @brief getWork:
     * Getter function for the agent's work Location. Returns the work location
     * of the agent
     * @return the agent's work Location
     */
    Location* getWork();

    /**
     * @brief setLeisure:
     * Setter function for the agent's leisure Location. Changes the leisure
     * location to which the agent is assigned to a new location
     * @param leisure: the leisure Location the agent is assigned to
     */
    void setLeisure(Location* leisure);

    /**
     * @brief getLeisure:
     * Getter function for the agent's leisure Location. Returns the leisure
     * location of the agent
     * @return the agent's leisure location
     */
    Location* getLeisure();

    /**
     * @brief getPosition:
     * Getter function for the position of the agent. Returns the current
     * position of the agent as a Coordinate*
     * @return the position of the agent as a Coordinate
     */
    Coordinate* getPosition();

    /**
     * @brief getVisualize:
     * Getter function for whether to visualize the agent's
     * assignments to other points. Visualizing is a debugging tool that
     * displays the agents connections to different locations in the sim
     * @return this->visualize
     */
    bool getVisualize();

    /**
     * @brief initVisualize
     * Function to initialize the visualizing lines that visualize to assignment
     * of the agent to its locations; Must be called after the agent is
     * assigned to its locations. Updates to these lines as the agent moves are
     * handled automatically through the updateRender() function. Lines must
     * be added to the UI by calling the renderVisualize() function
     */
    void initVisualize();

    /**
     * @brief renderVisualize
     * Function that returns the visualization lines that need to be rendered
     * to the screen. Must be called after initVisualize().
     * @return a vector of QGraphicsItem* that contains the four lines that
     * visualize assignment to locations
     */
    std::vector<QGraphicsItem*> renderVisualize();

    /**
     * @brief allRenderedObject
     * Function that returns a vector of all objects rendered to the screen
     * that are owned by this agent. This function should be called whenever
     * the simulation goes to remove an agent. QGraphicsScene::removeItem()
     * should be called on all items in the returned vector. The destructor
     * for this agent will then clean up the pointers
     * @return a std::vector containing all QGraphicsItems rendered by the agent
     */
    std::vector<QGraphicsItem*> allRenderedObject();

    /**
     * @brief currentLocationPosition
     * Function that returns the position of the Agent's current location as
     * a Coordinate*. Reads the currentLocation enum and returns the position
     * associated with that location
     * @return
     */
    Coordinate* currentLocationPosition();

    /**
     * @brief getBehavior
     * Getter function for the behavior assignment of this agent. Returns
     * the behavior assignment as an integer
     * @return the behavior assignment as an integer
     */
    int getBehavior();

    /**
     * @brief ~Agent
     * Destructor to free memory from the Agent class
     */
    ~Agent();

private:

    /** The current location of the Agent as a Coordinate*/
    Coordinate* position;

    /** The current age of the Agent as an Integer */
    int age;

    /** Int representing how wide the agents should be rendered as */
    static const int AGENT_WIDTH = 2;

    /** Int representing how far an agent is allowed to "creep" away
    from its current location through it's random movements */
    static const int MAX_CREEP = 7;

    /** Int representing the speed constant of the agent*/
    static const int SPEED_CONSTANT = 5;

    /** Int representing the current behavioral plan of the agent*/
    int behavior;

    /** Visualizes connection between agent to assignments if true */
    bool visualize;

    /** Rectangle corresponding to this rendered agent */
    QGraphicsRectItem* rect;

    /** Pointer to the agent's home location */
    Location* home;

    /** Line used to visualize assignment to home Location*/
    QGraphicsLineItem* homeLine;

    /** Pointer to the agent's school location, may be nullptr*/
    Location* school;

    /** Line used to visualize assignment to school Location */
    QGraphicsLineItem* schoolLine;

    /** Pointer to the agent's work location, may be nullptr*/
    Location* work;

    /** Line used to visualize assignment to work Location*/
    QGraphicsLineItem* workLine;

    /** Pointer to the agent's leisure location */
    Location* leisure;

    /** Line used to visualize assignment to leisure Location */
    QGraphicsLineItem* leisureLine;

    /** Coordinate that determines where the agent is trying to head. A
    Coordinate of (-1,-1) indicates no destination*/
    Coordinate* destination;

    /** Coordinate that represents the heading between the agent and its
    destination */
    Coordinate* heading;

    /** currentLocation enum that specifies where the Agent is at or heading */
    currentLocation currentPlace;
};

#endif // AGENT_H
