#include <Headers/Coordinate.h>
#include <math.h>

Coordinate::Coordinate() {
    this->x = 0;
    this->y = 0;
}

Coordinate::Coordinate(double x, double y) {
    this->x = x;
    this->y = y;
}


double Coordinate::getCoord(COORDINATES which) {
    if (which == COORDINATES::X) {
        return x;
    } else if (which == COORDINATES::Y) {
        return y;
    } else {
        return -1; // Default value if invalid Coordinate is selected
    }
}


void Coordinate::setCoord(double newVal, COORDINATES which) {
    if (which == COORDINATES::X) {
        x = newVal;
    } else if (which == COORDINATES::Y) {
        y = newVal;
    }
}


double Coordinate::distBetween(Coordinate& other) {
    double xDiff = getCoord(Coordinate::X) - other.getCoord(Coordinate::X);
    double yDiff = getCoord(Coordinate::Y) - other.getCoord(Coordinate::Y);
    return sqrt((xDiff * xDiff) + (yDiff * yDiff));
}


Coordinate Coordinate::headingBetween(Coordinate& other) {
    double xdiff = other.getCoord(Coordinate::X) - getCoord(Coordinate::X);
    double ydiff = other.getCoord(Coordinate::Y) - getCoord(Coordinate::Y);
    // Prevent divide by zero errors
    if (xdiff == 0 && ydiff == 0) {
        return Coordinate(0,0);
    }
    double magnitude = sqrt((xdiff * xdiff) + (ydiff * ydiff));
    return Coordinate(xdiff / magnitude, ydiff / magnitude);
}
