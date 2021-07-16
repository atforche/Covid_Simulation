#include "Headers/EconomicLocation.h"

EconomicLocation::EconomicLocation(double x, double y, Agent::LOCATIONS type) :
    Location(x, y) {

    this->value = 0;
    this->sibling = nullptr;
    this->type = type;

    // Give the location a random cost associated with it
    if (type == Agent::HOME) {
        this->cost = (rand() % 7) + 1;
    } else if (type == Agent::SCHOOL) {
        this->cost = 1;
    } else {
        this->cost = (rand() % 15) + 1;
    }
}


//******************************************************************************


int EconomicLocation::getValue() {
    return this->value;
}


//******************************************************************************


int EconomicLocation::incrementValue(int amount) {
    this->value = std::max(this->value + amount, 0);
    return this->value;
}


//******************************************************************************


void EconomicLocation::setSibling(EconomicLocation *other) {
    this->sibling = other;
}


//******************************************************************************


EconomicLocation* EconomicLocation::getSibling() {
    if (type == Agent::WORK || type == Agent::LEISURE) {
        return dynamic_cast<EconomicLocation*>(sibling);
    }
    return nullptr;
}


//******************************************************************************


Agent::LOCATIONS EconomicLocation::getType() {
    return type;
}


//******************************************************************************


int EconomicLocation::getCost() {
    return this->cost;
}

