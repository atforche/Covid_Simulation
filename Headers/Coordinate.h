#ifndef COORDINATE_H
#define COORDINATE_H

/**
 * @brief The Coordinate class \n
 * A Class that represents positiions within the simulation. Also
 * supports calculations between Coordinates
 */


class Coordinate {

private:
    /** Doubles representing the x and y positions of the Coordinate*/
    double x;
    double y;

public:

    /** Enum specifying the different coordinate values stored in the class*/
    enum COORDINATES {X, Y};

    /**
     * @brief Coordinate
     * Default constructor for the Coordinate class. Creates a Coordinate
     * located at (0,0)
     */
    Coordinate();

    /**
     * @brief Coordinate \n
     * Constructor for the Coordinate Class. A Coordinate is an object that
     * has an X and Y position and can represent a position in the Simulation
     * space.
     * @param x: the x position of the Coordinate
     * @param y: the y position of the Coordinate
     */
    Coordinate(double x, double y);

    /**
     * @brief getCoord \n
     * Function to return one of the Coordinate values stored within the
     * Coordinate Class
     * @param which: enum specifying which Coordinate value to return
     * @return the specified coordinate value as a double
     */
    double getCoord(COORDINATES which);

    /**
     * @brief setCoord \n
     * Function to set one of the Coordinate values stored within the
     * Coordinate class to a new value
     * @param newVal: the new value of the coordinate
     * @param which: enum specifying which Coordinate value to change
     */
    void setCoord(double newVal, COORDINATES which);

    /**
     * @brief distBetween \n
     * Function to calculate the distance between two coordinates. Calculates
     * the Euclidean distance between this Coordinate and another Coordinate
     * @param other: the second coordinate to calculate the distance to
     * @return the distance between the two coordinates as a double
     */
    double distBetween(Coordinate& other);

    /**
     * @brief headingBetween \n
     * Function to calculate the heading vector that connects the current point
     * to the point other. The heading vector is a straight line unit vector
     * that connects the two points in as little distance as possible.
     * @param other: the second point that the first point is trying to reach
     * @return a Coordinate corresponding to the shortest 2D vector between
     *         the two points
     */
    Coordinate headingBetween(Coordinate& other);

    /**
     * @brief operator == \n
     * Overloaded equality operator for the Coordinate Class. Enables Coordinates
     * to be compared using "==". Returns true of the Coordinates X and Y
     * members are equal
     * @param lhs: first Coordinate to compare
     * @param rhs: second Coordinate to compare
     * @return whether the two Coordinates are equivalent
     */
    friend bool operator==(const Coordinate& lhs, const Coordinate& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    };

};

#endif // COORDINATE_H
