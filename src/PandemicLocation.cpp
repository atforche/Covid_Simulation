#include "Headers/PandemicLocation.h"

PandemicLocation::PandemicLocation(double x, double y, Agent::LOCATIONS type) :
    Location(x, y) {

    // Initialize values
    locationType = type;
    pandemicStatus = NORMAL;
    numInfectedAgents = 0;
}


//******************************************************************************


Agent::LOCATIONS PandemicLocation::getType() {
    return this->locationType;
}


//******************************************************************************


void PandemicLocation::addInfectedAgent() {
    numInfectedAgents++;
    if (pandemicStatus != PandemicLocation::LOCKDOWN) {
        pandemicStatus = PandemicLocation::EXPOSURE;
    }
}


//******************************************************************************


void PandemicLocation::removeInfectedAgent() {
    numInfectedAgents--;
    if (numInfectedAgents == 0) {
        pandemicStatus = PandemicLocation::NORMAL;
    }
}


//******************************************************************************


int PandemicLocation::getNumInfectedAgents() {
    return numInfectedAgents;
}


//******************************************************************************


PandemicLocation::STATUS PandemicLocation::getStatus() {
    return this->pandemicStatus;
}


//******************************************************************************


void PandemicLocation::setStatus(PandemicLocation::STATUS newStatus) {
    this->pandemicStatus = newStatus;
}


//******************************************************************************


PandemicLocation::~PandemicLocation() {}


//******************************************************************************
