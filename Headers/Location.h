#ifndef LOCATION_H
#define LOCATION_H

/**
 * @brief The Location class
 * A base class that defines functionality of unique locations within the
 * Simulation
 */

#include <unordered_set>
#include "QGraphicsEllipseItem"
#include "Headers/Coordinate.h"
class Agent;

class Location {

private:

    /** Coordinate storing the exact position of the Location*/
    Coordinate* position;

    /** A vector keeping track of agents assigned to this location */
    std::unordered_set<Agent*> agents;

    /** Constant width of the locations ellipse on the screen */
    const int LOCATION_WIDTH = 10;

public:

    /**
     * @brief Location
     * Constructor for the Location class
     * @param x: the x position of the Location
     * @param y: the y position of the Location
     */
    Location(double x, double y);

    /**
     * @brief addAgent
     * Function to assign an Agent to this Location
     * @param agent: the agent to be assigned to this location
     */
    void addAgent(Agent &agent);

    /**
     * @brief removeAgent
     * Function to remove an Agent from this Location
     * @param agent: the agent to be removed from this location
     */
    void removeAgent(Agent &agent);

    /**
     * @brief renderLocation
     * Function that draws a QGraphicsEllipseItem to the screen where this
     * location appears
     * @return a QGraphicsEllipseItem at this Location
     */
    QGraphicsEllipseItem* renderLocation();

    /**
     * @brief getPosition: Getter function for the position of the Location
     * @return the Coordinate position of the Location
     */
    Coordinate* getPosition();

    /** Destructor for the location class */
    ~Location();

};

#endif // LOCATION_H
