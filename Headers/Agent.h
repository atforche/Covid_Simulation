#ifndef AGENT_H
#define AGENT_H

#include <QPen>
#include <QGraphicsRectItem>

#include "Headers/Coordinate.h"
#include "Headers/Location.h"

/**
 * @brief The Agent Class \n
 * Class that specifies the functionality of the Agents in the simulation
 * Contains all information required for the agent to act in the environment
 */
class Agent {

private:

    /** The current location of the Agent as a Coordinate*/
    Coordinate position;

    /** The current age of the Agent as an Integer */
    int age;

    /** Current speed of the agent. Can differ from the base speed.*/
    double speed;

    /** Int representing the current behavioral plan of the agent*/
    int behavior;

    /** Rectangle corresponding to this rendered agent */
    QGraphicsRectItem* rect;

    /** Vector to hold the Locations the agent is assigned to. The index
    of each location corresponds to its location in the LOCATIONS enum */
    std::vector<Location*> locations;

    /** Coordinate that determines where the agent is trying to head. A
    Coordinate of (-1,-1) indicates no destination*/
    Coordinate destination;

    /** QString that stored the current destination assignment as a string */
    QString destinationString;

    /** Int representing how wide the agents should be rendered as */
    static const int AGENT_WIDTH = 2;

    /** Int representing how far an agent is allowed to "creep" away
    from its current location through it's random movements */
    static const int MAX_CREEP = 7;

    /** Int representing the speed constant of the agent*/
    static const int BASE_SPEED = 5;

public:

    /** Enum that specifies the possible locations for the Agent to be */
    enum LOCATIONS {HOME, SCHOOL, WORK, LEISURE, LOCATIONS_MAX = 4};

    /**
     * @brief Agent \n
     * Constructor for the Agent Class. Constructs an Agent object with a
     * certain age, position, and behavior chart.
     * @param age: the current age of the Agent
     * @param startingLocation: the initial location to create this agent at
     * @param startingLocationString: string that represents the type of starting location
     * @param behavior: the behavior assignment of this agent
     */
    Agent(int age, Location* startingLocation, QString startingLocationString,
          int behavior);

    /**
     * @brief getGraphicsObject \n
     * Function that returns the QGraphics object that represents the agent
     * on the screen. Returned object should be added to the Simulation
     * in order for it to appear.
     */
    QGraphicsRectItem* getGraphicsObject();

    /**
     * @brief updateGraphicsObject \n
     * Function that updates the position of the QGraphics object that represents
     * the agent on the screen. Signals to the screen to redraw the agent. Can
     * only be called from the main thread.
     */
    void updateGraphicsObject();

    /**
     * @brief takeTimeStep \n
     * Function that causes the agent to take a step forward in time. Currently,
     * only updates the position of the Agent. Agent::updateGraphicsObject should
     * be called in order to update the agent on the screen
     */
    void takeTimeStep();

    /**
     * @brief setDestination \n
     * Sets the destination of this Agent to the new Location specified
     * @param newLocation: a pointer to the new Location to set the agents
     *                     destination toward
     * @param destinationType: QString representing the type of destination
     */
    void setDestination(Location& newLocation, QString destinationType);

    /**
     * @brief getDestinationString \n
     * Getter function for the string representation of the current Destination
     * of the Agent
     * @return the current Destination of the Agent as a QString
     */
    QString getDestinationString();

    /**
     * @brief setLocation \n
     * Function to set the value of an Agents assigned location to a new value
     * @param location: a pointer to the new location
     * @param which: specifies which of the Agent's locations to overwrite
     * (Home, School, Leisure, Work)
     */
    void setLocation(Location* location, LOCATIONS which);

    /**
     * @brief getLocation \n
     * Function to get a pointer to one of the Agents assigned locations
     * @param which: specifies which of the Agent's locations to return (Home,
     * School, Leisure, Work)
     * @return the specified location as a pointer
     */
    Location* getLocation(LOCATIONS which);

    /**
     * @brief getCurrentLocation \n
     * Getter function for the current location the Agent is assigned to.
     * @return a pointer to the Agent's current location
     */
    Location* getCurrentLocation();

    /**
     * @brief getPosition: \n
     * Getter function for the position of the agent. Returns the current
     * position of the agent as a Coordinate*
     * @return the position of the agent as a Coordinate
     */
    Coordinate getPosition();

    /**
     * @brief getBehavior \n
     * Getter function for the behavior assignment of this agent. Returns
     * the behavior assignment as an integer
     * @return the behavior assignment as an integer
     */
    int getBehavior();

    /**
     * @brief setBehavior \n
     * Updates the behaviorChart of the agent to a new behavior.
     * @param newBehavior: the new behavior chart for the agent
     */
    void setBehavior(int newBehavior);

    /**
     * @brief isAdult \n
     * Returns whether or not this agent is an adult. An agent is considered
     * an adult if they are 18 years of age.
     * @return whether the agent is an adult
     */
    bool isAdult();

    /**
     * @brief getAge \n
     * Getter function for the Age of the agent
     * @return the age of the agent as an int
     */
    int getAge();

    /**
     * @brief incrementAge
     * Increases the age of the agent by one,
     * @return the new agent of the agent after the increment
     */
    int incrementAge();

    /**
     * @brief setColor \n
     * Setter function for the color of the Agent. Updates the color of the Agent
     * to the provided color.
     * @param color: the new color for the agent
     */
    void setColor(QColor color);

    /**
     * @brief ~Agent \n
     * Destructor to free memory from the Agent class
     */
    virtual ~Agent();

};


#endif // AGENT_H
