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
    // Offset by (LOCATION_WIDTH/2) is necessary to allow locations to be
    // rendered so they are centered around their position, instead of the
    // position being the top left coordinate of the Location
    return new QGraphicsEllipseItem(position->getX() - (LOCATION_WIDTH/2),
                                    position->getY() - (LOCATION_WIDTH/2),
                                    LOCATION_WIDTH, LOCATION_WIDTH);
}


Coordinate* Location::getPosition() {
    return this->position;
}


Location::~Location() {
    delete position;
    position = nullptr;
}
