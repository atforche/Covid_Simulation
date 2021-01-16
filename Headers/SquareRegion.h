#ifndef SQUAREREGION_H
#define SQUAREREGION_H

#include "Headers/Region.h"
#include "Headers/Coordinate.h"
#include "Headers/Location.h"
#include "QGraphicsRectItem"
#include "QColor"
#include "QPen"
#include "QBrush"
#include <string>
#include <random>

/**
 * @brief The SquareRegion class \n
 * Class that defined the functionality for a square region. Inherited from
 * the Region Class
 */
class SquareRegion : public Region {

private:

    /** The top left corner of the square region */
    Coordinate start;

    /** Integer that stores the exact height of the region */
    int height;

public:

    /**
     * @brief SquareRegion \n
     * Constructor for the SquareRegion Class. A Square
     * Region is a derived region that is represented by a QGraphicsRectItem.
     * @param numLocations: the number of locations in the region
     * @param start: the Coordinate of the top-left corner of the region
     * @param height: the side length of the region
     * @param color: the QColor with which the region will be drawn
     * @param name: the name of the region
     */
    SquareRegion(int numLocations, Coordinate start, int height,
                 QColor color, std::string name);

    /**
     * @brief getGraphicsObject \n
     * Function that returns a QGraphicsRectItem* used to render the region
     * on the QGraphicsScene. Returns QGraphicsItem needs to be added to
     * the Simulation for it to appear.
     * @return a QGraphicsItem* representing the rectangular region
     */
    virtual QGraphicsItem* getGraphicsObject() override;

    /**
     * @brief getNameGraphicsObject \n
     * Function to render the text name of the region onto the canvas. Renders
     * the name as a QGraphicsTextItem and places it in the top left corner
     * of the SquareRegion.
     * @return a QGraphicsItem* representing the name of the region
     */
    virtual QGraphicsItem* getNameGraphicsObject() override;

    /**
     * @brief getStartingCoordinate \n
     * Getter function for the starting Coordinate of the square region (top
     * left corner). Returns the Coordinate as a pointer
     * @return the starting coordinate of the square region
     */
    Coordinate* getStartingCoordinate();

    /**
     * @brief getHeight \n
     * Getter function for the height of the region. Returns the height of the
     * Region as an int. getWidth function is not necessary because these
     * are all square regions
     * @return the height of the region as an int
     */
    int getHeight();

    /**
     * @brief renderLocations \n
     * Function to render the randomly generated locations to the screen.
     * Returns a vector of QGraphicsItems that are the objects to render
     * the Locations on the QGraphicsScene. The returned objects need to
     * be added to a Simulation in order to appear
     */
    std::vector<QGraphicsItem*> getLocationsGraphicsObject() override;

    /** Destructor for the SquareRegion Class*/
    ~SquareRegion();
};

#endif // SQUAREREGION_H
