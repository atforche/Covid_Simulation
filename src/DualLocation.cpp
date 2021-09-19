#include "Headers/DualLocation.h"

DualLocation::DualLocation(double x, double y, Agent::LOCATIONS type) :
    Location(x, y),
    PandemicLocation(x, y, type),
    EconomicLocation(x, y, type) {
};


//******************************************************************************
