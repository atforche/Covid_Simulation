#ifndef COORDINATE_H
#define COORDINATE_H

/**
 * @brief The Coordinate class
 * A Class that represents positiions within the simulation. Also
 * supports calculations between Coordinates
 */


class Coordinate {

private:
    /** Doubles representing the x and y positions of the Coordinate*/
    double x;
    double y;

public:

    /**
     * @brief Coordinate
     * Constructor for the Coordinate Clas
     * @param x: the x position of the Coordinate
     * @param y: the y position of the Coordinate
     */
    Coordinate(double x, double y);

    /**
     * @brief getX
     * Getter function for the X position of the Coordinate
     * @return the X position of the Coordinate
     */
    double getX();

    /**
     * @brief getY
     * Getter function for the Y position of the Coordinate
     * @return the Y position of the Coordinate
     */
    double getY();

    /**
     * @brief setX
     * Setter function for the X position of the Coordinate
     * @param x: the new X position of the coordinate
     */
    void setX(double x);

    /**
     * @brief setY
     * Setter function for the Y position of the Coordinate
     * @param y: the new Y position of the coordinate
     */
    void setY(double y);

    /**
     * @brief distBetween
     * Function to calculate the distance between two coordinates
     * @param other
     * @return the distance between the two coordinates as a double
     */
    double distBetween(Coordinate& other);

};

#endif // COORDINATE_H
