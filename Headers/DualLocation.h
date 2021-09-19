#ifndef DUALLOCATION_H
#define DUALLOCATION_H

#include "PandemicLocation.h"
#include "EconomicLocation.h"


class DualLocation : public PandemicLocation, public EconomicLocation {

private:

public:

    /**
     * @brief DualLocation \n
     * Constructor for a Dual Location. An Dual Location contains all
     * the same members as an actual location, but also includes information
     * about it's type, current value, current pandemic status, and connected location (if any)
     * @param x: the x coordinate of the Location
     * @param y: the y coordinate of the Location
     * @param type: the type of location
     */
    DualLocation(double x, double y, Agent::LOCATIONS type);

};


#endif // DUALLOCATION_H
