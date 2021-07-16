#include "Headers/EconomicAgent.h"

EconomicAgent::EconomicAgent(int initialValue, int age, Location* startingLocation,
                             QString startingLocationString, int behavior) :
    Agent(age, startingLocation, startingLocationString, behavior) {

    // Give the agent some initial economic value
    this->economicValue = initialValue;
    this->currentStatus = NORMAL;
}


//******************************************************************************


void EconomicAgent::economicUpdate() {
    this->decrementValue(100);
}


//******************************************************************************


int EconomicAgent::incrementValue(int amount) {
    this->economicValue += amount;
    return economicValue;
}


//******************************************************************************


int EconomicAgent::decrementValue(int amount) {
    this->economicValue = std::max(economicValue - amount, 0);
    return economicValue;
}


//******************************************************************************


int EconomicAgent::getValue() {
    return this->economicValue;
}

