#include "Headers/Region.h"

Region::Region(int numLocations, QColor color, std::string name) {
    this->numLocations = numLocations;
    this->color = color;
    this->name = name;
}


//******************************************************************************


std::vector<Location*>& Region::getLocations() {
    return this->locations;
}


//******************************************************************************


void Region::setLocations(std::vector<Location *> &locations) {
    this->locations = locations;
}


//******************************************************************************


int Region::getNumLocations() {
    return this->numLocations;
}


//******************************************************************************


QColor Region::getColor() {
    return this->color;
}


//******************************************************************************


std::string Region::getName() {
    return this->name;
}

