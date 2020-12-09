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
 * @brief The SquareRegion class
 * Class that defined the functionality for a square region. Inherited from
 * the Region Class
 */
class SquareRegion : public Region {

private:

    /** The top left corner of the square region */
    Coordinate start;

    /** Integer that stores the exact height of the region */
    int height;

    /** A pointer to the screen element that is displaying this region */
    QGraphicsRectItem* rect;

public:

    /**
     * @brief SquareRegion: Constructor for the SquareRegion Class
     * @param numLocations: the number of locations in the region
     * @param start: the Coordinate of the top-left corner of the region
     * @param height: the side length of the region
     * @param color: the QColor with which the region will be drawn
     * @param name: the name of the region
     */
    SquareRegion(int numLocations, Coordinate start, int height,
                 QColor color, std::string name);

    /**
     * @brief renderRegion: Function to render the rectangular region that
     * represents the region on the screen
     * @return a QGraphicsItem* representing the rectangular region
     */
    virtual QGraphicsItem* renderRegion() override;

    /**
     * @brief renderText: Function to render the text name of the region onto
     * the canvas
     * @return a QGraphicsItem* representing the name of the region
     */
    virtual QGraphicsItem* renderName() override;

    /**
     * @brief getStart: Getter function for the starting Coordinate of the
     * square region
     * @return the starting coordinate of the square region
     */
    Coordinate getStart();

    /**
     * @brief getHeight: Getter function for the height of the region
     * @return the height of the region as an int
     */
    int getHeight();

    /**
     * @brief renderLocations
     * Function to render the randomly generated locations to the screen
     */
    std::vector<QGraphicsItem*> renderLocations() override;

    /** Destructor for the SquareRegion Class*/
    ~SquareRegion();

};

#endif // SQUAREREGION_H