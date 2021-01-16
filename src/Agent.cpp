#include <Headers/Agent.h>

Agent::Agent(int age, Coordinate* position) {
    this->age = age;
    this->position = position;
    this->destination = new Coordinate(-1,-1); // Default destination for now
    this->rect = new QGraphicsRectItem(this->position->getCoord(Coordinate::X),
                                       this->position->getCoord(Coordinate::Y),
                                       AGENT_WIDTH, AGENT_WIDTH);

    // Default initialize the Locations vector
    this->locations = std::vector<Location*>(Agent::LOCATIONS_MAX);

    this->behavior = 1; // Static as there is only one behavior; Change Later
}


QGraphicsRectItem* Agent::getGraphicsObject() {
    return rect;
}


void Agent::updateGraphicsObject() {
    rect->setRect(position->getCoord(Coordinate::X),
                  position->getCoord(Coordinate::Y),
                  AGENT_WIDTH, AGENT_WIDTH);
}


void Agent::setDestination(Location *newLocation) {
    this->destination = newLocation->getPosition();
}


void Agent::setLocation(Location *location, LOCATIONS which) {
    this->locations[static_cast<int>(which)] = location;
    if (which == HOME) {
        this->destination = location->getPosition(); // Start agents out heading toward home. Change Later
    }
}


Location* Agent::getLocation(LOCATIONS which) {
    return this->locations[static_cast<int>(which)];
}


void Agent::takeTimeStep() {
    // If the agent is within MAX_CREEP pixels of its destination, just move
    // around randomly in the area. Otherwise, move toward the destination
    // in a straight line
    if (getPosition()->distBetween(this->destination) < MAX_CREEP) {
        position->setCoord(position->getCoord(Coordinate::X) + (rand() % 3 - 1),
                           Coordinate::X);
        position->setCoord(position->getCoord(Coordinate::Y) + (rand() % 3 - 1),
                           Coordinate::Y);
    } else {
        Coordinate* heading = getPosition()->headingBetween(this->destination);
        position->setCoord(position->getCoord(Coordinate::X) +
                       heading->getCoord(Coordinate::X) * SPEED_CONSTANT,
                           Coordinate::X);
        position->setCoord(position->getCoord(Coordinate::Y) +
                       heading->getCoord(Coordinate::Y) * SPEED_CONSTANT,
                           Coordinate::Y);
    }
}


Coordinate* Agent::getPosition() {
    return this->position;
}


int Agent::getBehavior() {
    return this->behavior;
}

Agent::~Agent() {
    delete this->position;
    delete this->rect;

    this->position = nullptr;
    this->rect = nullptr;
}
