#include <Headers/Agent.h>

Agent::Agent(int age, Location* startingLocation, int behavior) {
    this->age = age;
    this->position = startingLocation->getPosition();
    setDestination(startingLocation);
    this->rect = new QGraphicsRectItem(this->position.getCoord(Coordinate::X),
                                       this->position.getCoord(Coordinate::Y),
                                       AGENT_WIDTH, AGENT_WIDTH);

    // Default initialize the Locations vector
    this->locations = std::vector<Location*>(Agent::LOCATIONS_MAX);

    // Give the agent the default speed
    this->speed = BASE_SPEED;

    this->behavior = behavior;
}


QGraphicsRectItem* Agent::getGraphicsObject() {
    return rect;
}


void Agent::updateGraphicsObject() {
    rect->setRect(position.getCoord(Coordinate::X),
                  position.getCoord(Coordinate::Y),
                  AGENT_WIDTH, AGENT_WIDTH);
}


void Agent::setDestination(Location *newLocation) {
    this->destination = newLocation->getPosition();
    // Update the speed of the Agent so it arrives in no more than
    // 30 minutes (30 frames)
    double dist = this->position.distBetween(this->destination);
    this->speed = std::max(static_cast<double>(BASE_SPEED), dist/30);
}


void Agent::setLocation(Location *location, LOCATIONS which) {
    this->locations[static_cast<int>(which)] = location;
}


Location* Agent::getLocation(LOCATIONS which) {
    return this->locations[static_cast<int>(which)];
}


void Agent::takeTimeStep() {
    // If the agent is within MAX_CREEP pixels of its destination, just move
    // around randomly in the area. Otherwise, move toward the destination
    // in a straight line
    if (position.distBetween(this->destination) < MAX_CREEP) {
        // Reset the agents speed
        this->speed = BASE_SPEED;

        // Update the agents position randomly
        position.setCoord(position.getCoord(Coordinate::X) + (rand() % 3 - 1),
                           Coordinate::X);
        position.setCoord(position.getCoord(Coordinate::Y) + (rand() % 3 - 1),
                           Coordinate::Y);
    } else {
        Coordinate heading = position.headingBetween(this->destination);
        position.setCoord(position.getCoord(Coordinate::X) +
                       heading.getCoord(Coordinate::X) * speed,
                           Coordinate::X);
        position.setCoord(position.getCoord(Coordinate::Y) +
                       heading.getCoord(Coordinate::Y) * speed,
                           Coordinate::Y);
    }
}


Coordinate Agent::getPosition() {
    return this->position;
}


int Agent::getBehavior() {
    return this->behavior;
}


bool Agent::isAdult() {
    return (this->age >= 18);
}


Agent::~Agent() {
    delete this->rect;
    this->rect = nullptr;
}
