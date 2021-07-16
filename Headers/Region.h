#ifndef REGION_H
#define REGION_H

#include <vector>
#include <string>
#include <random>

#include "QColor"
#include "QGraphicsItem"
#include "Agent.h"

// Forward declaration of Location class
class Location;

/**
 * @brief The Region class
 * Base class that contains a list of Locations within the region, used to
 * manage assigning Agents to Locations
 */
class Region {

private:

    /** a vector of all Locations within the Region */
    std::vector<Location*> locations;

    /** an integer representing the number of Locations in the Region */
    int numLocations;

    /** a QColor representing the color with which the region will be drawn*/
    QColor color;

    /** a string representing the name of the region */
    std::string name;

    /** the type of Locations that appear in this region */
    Agent::LOCATIONS type;


public:

    /**
     * @brief Region \n
     * Constructor for the Region Class. A Region is a container class that
     * wraps around a set of locations. Locations in different regions are
     * subject to different rules and result in different agent behaviors. Each
     * Region has an associated shape that is rendered to the screen. Use a
     * LocationGenerator to generate Locations within the Region
     */
    Region(QColor color, std::string name);

    /** Virtual destructor for the Region Class */
    virtual ~Region() {};

    /**
     * @brief getLocations \n
     * Getter functions for the vector of Locations within a region. Returns
     * the Locations within the Region as a vector
     * @return a vector<Location*> contains all the Locations within the Region
     */
    std::vector<Location*>& getLocations();

    /**
     * @brief generateLocations \n
     * Generates a fixed number of locations within the bounds of the current
     * Region. Picks coordinates from a random distribution and ensures they
     * lie within the bounding rectangle of the region before creating a
     * location at that coordinate.
     * @param num: number of locations to generate
     * @param type: type of locations to generate, either Simple or Economic
     */
    void generateLocations(int num, std::string type = "Simple");

    /**
     * @brief getNumLocations \n
     * Getter function for the number of Locations within a Region. Returns
     * the number of Locations in a Region as an int
     * @return the number of locations in the region as an int
     */
    int getNumLocations();

    /**
     * @brief getColor \n
     * Getter function for the color assigned to the region. Returns the
     * color as a QColor
     * @return the QColor that the region will be drawn in
     */
    QColor getColor();

    /**
     * @brief getName \n
     * Getter function for the name assigned to the region. Returns the name
     * as a std::string
     * @return the name of the region
     */
    std::string getName();

    /**
     * @brief getGraphicsObject \n
     * Function to return a QGraphicsItem* corresponding
     * to the shape of the Region. This QGraphicsItem* must be added to a
     * simulation in order for it to appear. Region can not be updated or
     * re-rendered once created. Must be overridden by derived classes
     * @return a QGraphicsItem* representing the shape of the region
     */
    virtual QGraphicsItem* getGraphicsObject() = 0;

    /**
     * @brief getNameGraphicsObject \n
     * Function to render the name of the region onto the
     * canvas. Exactly rendering mechanism depends on the shape of the Region.
     * Must be overridden by derived classes
     * @return a QGraphicsItem* containing the name of the region
     */
    virtual QGraphicsItem* getNameGraphicsObject() = 0;

    /**
     * @brief renderLocations \n
     * Function to return a vector of QGraphicsItems* corresponding to the
     * Locations within the Region. Locations must be added to a Simulation
     * in order to appear
     * @return a vector of QGraphicsItem* that render all the locations
     */
    virtual std::vector<QGraphicsItem*> getLocationsGraphicsObject() = 0;

};

// Rear includes to avoid circular dependency
#include "Location.h"
#include "EconomicLocation.h"

#endif // REGION_H
