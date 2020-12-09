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
     * Constructor for the Region Class
     * @param numLocations: the number of Locations within this Region
     */
    Region(int numLocations, QColor color, std::string name);

    /**
     * @brief getLocations
     * Getter functions for the vector of Locations within a region
     * @return a vector<Location*> contains all the Locations within the Region
     */
    std::vector<Location*>& getLocations();

    /**
     * @brief setLocations
     * Setter function for the vector of Locations within a region
     * @param locations: the vector of generated locations for the region
     */
    void setLocations(std::vector<Location*>& locations);

    /**
     * @brief getNumLocations
     * Getter function for the number of Locations within a Region
     * @return the number of locations in the region as an int
     */
    int getNumLocations();

    /**
     * @brief getColor: Getter function for the color assigned to the region
     * @return the QColor that the region will be drawn in
     */
    QColor getColor();

    /**
     * @brief getName: Getter function for the name assigned to the region
     * @return the name of the region
     */
    std::string getName();

    /**
     * @brief generateLocations
     * Function to randomly generation a set of Locations within the region.
     * Locations are added to the Region using the setLocations function. Can
     * be retrieved using the getLocations function
     * @param generator: pointer to a location generator while will randomly
     * sample and generate the locations
     */
    void generateLocations(LocationGenerator* generator);

    /**
     * @brief renderRegion: Function to render the shape of the region onto
     * the canvas
     * @return a QGraphicsItem* representing the shape of the region
     */
    virtual QGraphicsItem* renderRegion() = 0;

    /**
     * @brief renderName: Function to render the name of the region onto the
     * canvas
     * @return a QGraphicsItem* containing the name of the region
     */
    virtual QGraphicsItem* renderName() = 0;

    /**
     * @brief renderLocations: Function to render each of the randomly generated
     * Locations to the screen
     */
    virtual std::vector<QGraphicsItem*> renderLocations() = 0;

};

#endif // REGION_H
