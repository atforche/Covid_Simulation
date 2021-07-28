#ifndef LOCATION_H
#define LOCATION_H

#include <unordered_set>

#include "QGraphicsEllipseItem"
#include "QDebug"
#include "ui_mainwindow.h"

#include "Headers/Coordinate.h"

// Forward declaration of the Agent Class
class Agent;

/**
 * @brief The Location class \n
 * A base class that defines functionality of unique locations within the
 * Simulation
 */
class Location {

private:

    /** Coordinate storing the exact position of the Location*/
    Coordinate position;

    /** A vector keeping track of agents assigned to this location */
    std::unordered_set<Agent*> agents;

    /** Constant width of the locations ellipse on the screen */
    const int LOCATION_WIDTH = 10;

    /** QGraphicsItem that represents this location on the screen */
    QGraphicsEllipseItem* ellipse;

    /** Constant border width for rendering all locations */
    const qreal locationBorderWidth = 1.5;

    /** Unique ID associated with each Location */
    int id;

public:

    /**
     * @brief Location \n
     * Constructor for the Location class. Each Location contains a position and
     * a vector of agents that are assigned to it. (May be useful to keep track
     * of what type of Location it is, problem for future Andy)
     * @param x: the x position of the Location
     * @param y: the y position of the Location
     */
    Location(double x, double y);

    /**
     * @brief addAgent \n
     * Function to assign an Agent to this Location. Adds the agent to the
     * agents set. Allows location to keep track of its associated agents
     * for quarantining purposes.
     * @param agent: the agent to be assigned to this location
     */
    void addAgent(Agent *agent);

    /**
     * @brief removeAgent \n
     * Function to remove an Agent from this Location. Removes this agent from
     * the agents set. This function will generally only be used in the case
     * of agent death, however it could be possible to introduce agent moving
     * locations
     * @param agent: the agent to be removed from this location
     */
    void removeAgent(Agent* agent);

    /**
     * @brief getGraphicsObject \n
     * Function that returns the QGraphicsEllipseItem to visulize this location.
     * The returned ellipse must be added to the simulation for it to appear in
     * the window. Currently, locations cannot be updated or moved once created.
     * @return a QGraphicsEllipseItem at this Location
     */
    QGraphicsEllipseItem* getGraphicsObject();

    /**
     * @brief getPosition: \n
     * Getter function for the position of the Location. Returns the position
     * of this location as a Coordinate
     * @return the Coordinate position of the Location
     */
    Coordinate getPosition();

    /**
     * @brief setColor \n
     * Update the color of the Location to a new color.
     * @param color: QColor for the Location to be displayed as
     */
    void setColor(QColor color);

    /**
     * @brief getAgents \n
     * Getter function for the set of Agents assigned to this location.
     * @return a pointer to the Agent set
     */
    std::unordered_set<Agent*> getAgents();

    /**
     * @brief getID \n
     * Getter function for the unique ID associated with this location
     * @return the ID as an int
     */
    int getID();

    /** Destructor for the location class */
    virtual ~Location();

};


/**
 * @brief The Location_Comparator class
 * Class that serves as a Unary comparator between Locations. Compares the
 * ID of each location to determine if they are the same
 */
class Location_Comparator {

private:

    /** Location that this Comparator is searching for */
    Location* desiredLocation;

public:

    /**
     * @brief Location_Comparator \n
     * Constructor for the Location_Comparator class. Initializes the desired
     * location of the Comparator instance
     * @param searchLocation: the Location to be found
     */
    Location_Comparator(Location* searchLocation);

    /**
     * @brief operator () \n
     * Overloaded function call operator for the Location_Comparator class.
     * Determines if the test location matches the desired location by comparing
     * their locations
     * @param test: Location to check
     * @return whether the test Locattion is the desired Location
     */
    bool operator() (Location* test);

};

#endif // LOCATION_H
