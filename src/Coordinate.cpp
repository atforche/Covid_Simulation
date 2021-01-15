#include <Headers/Coordinate.h>
#include <math.h>

Coordinate::Coordinate(double x, double y) {
    this->x = x;
    this->y = y;
}


double Coordinate::getX() {
    return this->x;
}


double Coordinate::getY() {
    return this->y;
}


void Coordinate::setX(double x) {
    this->x = x;
}


void Coordinate::setY(double y) {
    this->y = y;
}


double Coordinate::distBetween(Coordinate *other) {
    double xDiff = getX() - other->getX();
    double yDiff = getY() - other->getY();
    return sqrt((xDiff * xDiff) + (yDiff * yDiff));
}


Coordinate* Coordinate::headingBetween(Coordinate *other) {
    double xdiff = other->getX() - getX();
    double ydiff = other->getY() - getY();
    // Prevent divide by zero errors
    if (xdiff == 0 && ydiff == 0) {
        return new Coordinate(0,0);
    }
    double magnitude = sqrt((xdiff * xdiff) + (ydiff * ydiff));
    return new Coordinate(xdiff / magnitude, ydiff / magnitude);
}


bool Coordinate::isValid() {
    return (x >= 0 && y >= 0);
}
