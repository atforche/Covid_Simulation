#include "Headers/EconomicLocation.h"

EconomicLocation::EconomicLocation(double x, double y, Agent::LOCATIONS type) :
    Location(x, y) {

    this->value = 0;
    this->dailyValueChange = 0;
    this->yesterdayValueChange = 0;
    this->sibling = nullptr;
    this->type = type;
    this->totalDays = 0;
    this->dailyHire = false;

    // Home locations have a cost between [1, 3]
    if (type == Agent::HOME) {
//        this->cost = (rand() % 3) + 1;
        this->cost = 1;
    } else if (type == Agent::SCHOOL) {
        // School locations have a cost of 1
        this->cost = 1;
    } else if (type == Agent::WORK) {
        // Work locations have a cost of [5, 10]
        this->cost = (rand() % 2) + 5;
    } else {
        // Leisure locations have a cost of [3, 7]
        this->cost = (rand() % 5) + 3;
    }
}


//******************************************************************************


double EconomicLocation::getValue() {
    return this->value;
}


//******************************************************************************


double EconomicLocation::incrementValue(double amount) {
    double newValue = std::max(this->value + amount, 0.0);
    dailyValueChange += (newValue - this->value);
    this->value = newValue;
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


bool EconomicLocation::makeHire() {
    if (yesterdayValueChange > 2 * cost && !dailyHire) {
        dailyHire = true;
        return true;
    }
    return false;
}


//******************************************************************************


double EconomicLocation::getCost() {
    return this->cost;
}


//******************************************************************************


double EconomicLocation::getYesterdayValueChange() {
    return yesterdayValueChange;
}


//******************************************************************************


void EconomicLocation::startNewDay() {
    yesterdayValueChange = dailyValueChange;
    dailyValueChange = 0;
    this->dailyHire = false;
    this->totalDays++;
}


//******************************************************************************


int EconomicLocation::getDaysOpen() {
    return this->totalDays;
}


//******************************************************************************
