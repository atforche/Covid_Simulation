#include <Headers/Agent.h>

Agent::Agent(int age, Location* startingLocation, QString startingLocationString,
             int behavior) {
    // Initialize the Agent's age
    this->age = age;

    // Set the Agent's position and place them at their starting location
    this->position = startingLocation->getPosition();
    setDestination(*startingLocation, startingLocationString);

    // Initialize a QRect to render the agent
    this->rect = new QGraphicsRectItem(this->position.getCoord(Coordinate::X),
                                       this->position.getCoord(Coordinate::Y),
                                       AGENT_WIDTH, AGENT_WIDTH);

    // Default initialize the Locations vector
    this->locations = std::vector<Location*>(Agent::LOCATIONS_MAX);

    // Give the agent the default speed
    this->speed = BASE_SPEED;

    // Assign the agent a specific behavior
    this->behavior = behavior;
}


//******************************************************************************


QGraphicsRectItem* Agent::getGraphicsObject() {
    return rect;
}


//******************************************************************************


void Agent::updateGraphicsObject() {
    rect->setRect(position.getCoord(Coordinate::X),
                  position.getCoord(Coordinate::Y),
                  AGENT_WIDTH, AGENT_WIDTH);
}


//******************************************************************************


void Agent::takeTimeStep() {

    // If the agent is close to it's destination, just move randomly
    if (position.distBetween(this->destination) < MAX_CREEP) {

        // Reset the agents speed
        this->speed = BASE_SPEED;

        // Update the agents position randomly
        position.setCoord(position.getCoord(Coordinate::X) + (rand() % 3 - 1),
                           Coordinate::X);
        position.setCoord(position.getCoord(Coordinate::Y) + (rand() % 3 - 1),
                           Coordinate::Y);
    } else {
        // Else, move in a straight line toward the destination
        Coordinate heading = position.headingBetween(this->destination);
        position.setCoord(position.getCoord(Coordinate::X) +
                       heading.getCoord(Coordinate::X) * speed,
                           Coordinate::X);
        position.setCoord(position.getCoord(Coordinate::Y) +
                       heading.getCoord(Coordinate::Y) * speed,
                           Coordinate::Y);
    }
}


//******************************************************************************


void Agent::setDestination(Location &newLocation, QString destinationType) {

    // Update the destination of the agent
    this->destination = newLocation.getPosition();
    this->destinationString = destinationType;

    // Ensure the agent arrives in no more than 30 frames
    double dist = this->position.distBetween(this->destination);
    this->speed = std::max(static_cast<double>(BASE_SPEED), dist/30);
}


//******************************************************************************


QString Agent::getDestinationString() {
    return destinationString;
}


//******************************************************************************


void Agent::setLocation(Location *location, LOCATIONS which) {
    this->locations[static_cast<int>(which)] = location;
}


//******************************************************************************


Location* Agent::getLocation(LOCATIONS which) {
    return this->locations[static_cast<int>(which)];
}


//******************************************************************************


Location* Agent::getCurrentLocation() {
    if (destinationString == "Home") {
        return getLocation(Agent::HOME);
    } else if (destinationString == "Work") {
        return getLocation(Agent::WORK);
    } else if (destinationString == "Leisure") {
        return getLocation(Agent::LEISURE);
    } else if (destinationString == "School") {
        return getLocation(Agent::SCHOOL);
    }
    return nullptr;
}


//******************************************************************************


Coordinate Agent::getPosition() {
    return this->position;
}


//******************************************************************************


int Agent::getBehavior() {
    return this->behavior;
}


//******************************************************************************


void Agent::setBehavior(int newBehavior) {
    this->behavior = newBehavior;
}


//******************************************************************************


bool Agent::isAdult() {
    return (this->age >= 18);
}


//******************************************************************************


int Agent::getAge() {
    return this->age;
}


//******************************************************************************


int Agent::incrementAge() {
    this->age++;
    return this->age;
}


//******************************************************************************


void Agent::setColor(QColor color) {
    rect->setPen(color);
}


//******************************************************************************


Agent::~Agent() {}
