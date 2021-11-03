#include "Headers/EconomicAgent.h"

EconomicAgent::EconomicAgent(int initialValue, int age, Location* startingLocation,
                             QString startingLocationString, int behavior) :
    Agent(age, startingLocation, startingLocationString, behavior) {

    // Give the agent some initial economic value
    this->economicValue = initialValue;
    this->currentStatus = NORMAL;
    this->hoursOfEmployment = 0;
    this->hoursOfUnemployment = 0;
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


//******************************************************************************


EconomicAgent::STATUS EconomicAgent::getStatus() {
    return this->currentStatus;
}


//******************************************************************************


void EconomicAgent::setStatus(STATUS newStatus) {
    this->currentStatus = newStatus;
}


//******************************************************************************


void EconomicAgent::incrementHoursOfEmployment() {
    this->hoursOfEmployment++;

    // Once an agent works for three days, they are eligible to receive unemployment again
    if (hoursOfEmployment > 3 * 24) {
        this->hoursOfUnemployment = 0;
        this->hoursOfEmployment = 0;
    }
}


//******************************************************************************


void EconomicAgent::incrementHoursOfUnemployment() {
    this->hoursOfUnemployment++;
}


//******************************************************************************


bool EconomicAgent::canReceiveUnemployment() {
    return this->hoursOfUnemployment < (7 * 24);
}


//******************************************************************************
