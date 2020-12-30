#include <Headers/Location.h>

Location::Location(double x, double y) {
    position = new Coordinate(x, y);
}


void Location::addAgent(Agent &agent) {
    agents.insert(&agent);
}


void Location::removeAgent(Agent &agent) {
    if(agents.find(&agent) != agents.end()) {
        agents.erase(&agent);
    }
}


QGraphicsEllipseItem* Location::renderLocation() {
    return new QGraphicsEllipseItem(position->getX(), position->getY(),
                                    LOCATION_WIDTH, LOCATION_WIDTH);
}


Coordinate* Location::getPosition() {
    return this->position;
}


Location::~Location() {
    delete position;
    position = nullptr;
}
