#ifndef ECONOMICLOCATION_H
#define ECONOMICLOCATION_H

#include "Location.h"
#include "Agent.h"

class EconomicLocation : public Location {

private:

    /** The current economic value of the Location, only relevant for Work Locations */
    int value;

    /** Pointer to this Location's sibling location, only relevant for Work and
    Leisure locations */
    Location* sibling;

    /** The type of this specific location */
    Agent::LOCATIONS type;

    /** Cost associated with this specific location. Value an agent loses or gains at this location */
    int cost;

public:

    /**
     * @brief EconomicLocation \n
     * Constructor for an Economic Location. An Economic Location contains all
     * the same members as an actual location, but also includes information
     * about it's type, current value, and connected location (if any)
     * @param x: the x coordinate of the Location
     * @param y: the y coordinate of the Location
     * @param type: the type of location
     */
    EconomicLocation(double x, double y, Agent::LOCATIONS type);

    /**
     * @brief getValue \n
     * Getter function for the current economic value of the Location. If a
     * Work Location's economic value falls to zero, it and its associated
     * Leisure Location will disappear.
     * @return the current economic value of the Location
     */
    int getValue();

    /**
     * @brief incrementValue \n
     * Increment the economic value of the Location by a certain amount.
     * Will not allow the value of a Location to go below zero.
     * @return the new value of the Location
     */
    int incrementValue(int amount);

    /**
     * @brief setSibling \n
     * Sets the sibling location of this Location. Sibling locations refer to
     * pairs of Leisure/Work Locations that share money in tandem. Leisure
     * locations bring in money for the Work location, and both locations will
     * close if the Work Location runs out of money.
     * @param other
     */
    void setSibling(EconomicLocation* other);

    /**
     * @brief getSibling \n
     * Getter function for the sibling location to this location.
     * @return a pointer to the sibling location
     */
    EconomicLocation* getSibling();

    /**
     * @brief getType \n
     * Getter function for the Type of location this EconomicLocation is
     * @return an Agent::LOCATIONS enum representing a type of location
     */
    Agent::LOCATIONS getType();

    /**
     * @brief getCost \n
     * Getter function for the cost of an Economic Location. The cost is how much
     * value an agent gains or loses by spending an hour there.
     * @return the Location's cost as an int
     */
    int getCost();

};

#endif // ECONOMICLOCATION_H
