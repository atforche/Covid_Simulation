#ifndef ECONOMICLOCATION_H
#define ECONOMICLOCATION_H

#include "Location.h"
#include "Agent.h"


/**
 * @brief The EconomicLocation class
 * Class that inherits from the Location class. Extends the Location class to
 * enable Locations to have an associated value and cost.
 */
class EconomicLocation : virtual public Location {

private:

    /** The current economic value of the Location, only relevant for Work Locations */
    double value;

    /** Track how much the Locations value has changed by today */
    double dailyValueChange;

    /** Track how much the Locations value changed by yesterday */
    double yesterdayValueChange;

    /** Track how many hours have passed since the business was created */
    int totalDays;

    /** Boolean to indicate whether this business has hired an Agent today */
    bool dailyHire;

    /** Pointer to this Location's sibling location, only relevant for Work and
    Leisure locations */
    Location* sibling;

    /** The type of this specific location */
    Agent::LOCATIONS type;

    /** Cost associated with this specific location. Value an agent loses or gains at this location */
    double cost;

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
    double getValue();

    /**
     * @brief incrementValue \n
     * Increment the economic value of the Location by a certain amount.
     * Will not allow the value of a Location to go below zero.
     * @return the new value of the Location
     */
    double incrementValue(double amount);

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
     * @brief makeHire \n
     * Function that determines whether or not a Business location is able
     * to hire a new agent
     * @return whether the Business can hire a new agent
     */
    bool makeHire();

    /**
     * @brief getCost \n
     * Getter function for the cost of an Economic Location. The cost is how much
     * value an agent gains or loses by spending an hour there.
     * @return the Location's cost as an int
     */
    double getCost();

    /**
     * @brief getYesterdayValueChange \n
     * Getter function for the overall value change the Location experienced
     * Yesterday. Used to gain an estimate of how much economic value the
     * location is gaining or losing every day.
     * @return the total value change of this location from yesterday as an int
     */
    double getYesterdayValueChange();

    /**
     * @brief startNewDay \n
     * Resets the daily value change of the location
     */
    void startNewDay();

    /**
     * @brief getDaysOpen \n
     * Returns the number of days that the Location has been open
     * @return
     */
    int getDaysOpen();

};

#endif // ECONOMICLOCATION_H
