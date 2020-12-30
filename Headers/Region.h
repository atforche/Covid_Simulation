#ifndef REGION_H
#define REGION_H

/**
 * @brief The Region class
 * Base class that contains a list of Locations within the region, used to
 * manage assigning Agents to Locations
 */

#include <vector>
#include <string>
#include "QColor"
#include "QGraphicsItem"
#include "LocationGenerator.h"
class Location;

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


public:

    /**
     * @brief Region
     * Constructor for the Region Class. A Region is a container class that
     * wraps around a set of locations. Locations in different regions are
     * subject to different rules and result in different agent behaviors. Each
     * Region has an associated shape that is rendered to the screen. Use a
     * LocationGenerator to generate Locations within the Region
     * @param numLocations: the number of Locations within this Region
     */
    Region(int numLocations, QColor color, std::string name);

    /**
     * @brief getLocations
     * Getter functions for the vector of Locations within a region. Returns
     * the Locations within the Region as a vector
     * @return a vector<Location*> contains all the Locations within the Region
     */
    std::vector<Location*>& getLocations();

    /**
     * @brief setLocations
     * Setter function for the vector of Locations within a region. Pass in a
     * vector of Locations to set all Locations in a Region at once
     * @param locations: the vector of generated locations for the region
     */
    void setLocations(std::vector<Location*>& locations);

    /**
     * @brief getNumLocations
     * Getter function for the number of Locations within a Region. Returns
     * the number of Locations in a Region as an int
     * @return the number of locations in the region as an int
     */
    int getNumLocations();

    /**
     * @brief getColor
     * Getter function for the color assigned to the region. Returns the
     * color as a QColor
     * @return the QColor that the region will be drawn in
     */
    QColor getColor();

    /**
     * @brief getName
     * Getter function for the name assigned to the region. Returns the name
     * as a std::string
     * @return the name of the region
     */
    std::string getName();

    /**
     * @brief generateLocations
     * Function to randomly generation a set of Locations within the region.
     * Locations are added to the Region using the setLocations function. Can
     * be retrieved using the getLocations function. Utilizes a LocationGenerator
     * to randomly sample and select Locations that lie within the Regions
     * borders
     * @param generator: pointer to a location generator while will randomly
     * sample and generate the locations
     */
    void generateLocations(LocationGenerator* generator);

    /**
     * @brief renderRegion: Function to return a QGraphicsItem* corresponding
     * to the shape of the Region. This QGraphicsItem* must be added to a
     * simulation in order for it to appear. Region can not be updated or
     * re-rendered once created. Must be overridden by derived classes
     * @return a QGraphicsItem* representing the shape of the region
     */
    virtual QGraphicsItem* renderRegion() = 0;

    /**
     * @brief renderName
     * Function to render the name of the region onto the
     * canvas. Exactly rendering mechanism depends on the shape of the Region.
     * Must be overridden by derived classes
     * @return a QGraphicsItem* containing the name of the region
     */
    virtual QGraphicsItem* renderName() = 0;

    /**
     * @brief renderLocations:
     * Function to return a vector of QGraphicsItems* corresponding to the
     * Locations within the Region. Locations must be added to a Simulation
     * in order to appear
     * @return a vector of QGraphicsItem* that render all the locations
     */
    virtual std::vector<QGraphicsItem*> renderLocations() = 0;

};

#endif // REGION_H
