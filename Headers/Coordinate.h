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
     * Constructor for the Coordinate Class. A Coordinate is an object that
     * has an X and Y position and can represent a position in the Simulation
     * space.
     * @param x: the x position of the Coordinate
     * @param y: the y position of the Coordinate
     */
    Coordinate(double x, double y);

    /**
     * @brief getX
     * Getter function for the X position of the Coordinate. Returns the X
     * position of this Coordinate as a double
     * @return the X position of the Coordinate
     */
    double getX();

    /**
     * @brief getY
     * Getter function for the Y position of the Coordinate. Returns the Y
     * position of this Coordinate as a double
     * @return the Y position of the Coordinate
     */
    double getY();

    /**
     * @brief setX
     * Setter function for the X position of the Coordinate. Sets the X
     * position of this Coordinate to a new double
     * @param x: the new X position of the coordinate
     */
    void setX(double x);

    /**
     * @brief setY
     * Setter function for the Y position of the Coordinate. Sets the Y
     * position of this Coordinate to a new double
     * @param y: the new Y position of the coordinate
     */
    void setY(double y);

    /**
     * @brief distBetween
     * Function to calculate the distance between two coordinates. Calculates
     * the Euclidean distance between this Coordinate and a second Coordinate
     * @param other: the second coordinate to calculate the distance to
     * @return the distance between the two coordinates as a double
     */
    double distBetween(Coordinate *other);

    /**
     * @brief headingBetween
     * Function to calculate the heading vector that connects the current point
     * to the point other. The heading vector is a straight line unit vector
     * that connects the two points in as little distance as possible.
     * @param other: the second point that the first point is trying to reach
     * @return a Coordinate corresponding to the shortest 2D vector between
     *         the two points
     */
    Coordinate* headingBetween(Coordinate *other);

    /**
     * @brief isValid
     * Function to determine if a Coordinate is valid. A Coordinate is valid
     * if it has two positive components. An invalid coordinate can be used to
     * prevent an agent from pursuing something. Function may be expanded to
     * check scene bounds in the future.
     * @return
     */
    bool isValid();

};

#endif // COORDINATE_H
