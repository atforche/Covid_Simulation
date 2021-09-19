#ifndef PANDEMICLOCATION_H
#define PANDEMICLOCATION_H

#include "Location.h"
#include "Agent.h"


/**
 * @brief The PandemicLocation class
 * Class that inherits from the Location class. Extends the Location class to
 * enable Locations to have an associated Pandemic Status.
 */
class PandemicLocation : virtual public Location {

public:

    /** Possible pandemic status of a Location */
    enum STATUS {NORMAL, EXPOSURE, LOCKDOWN};

    /** Locations colors for each Pandemic Status */
    static constexpr QColor NORMALCOLOR = QColor(27, 163, 3);
    static constexpr QColor EXPOSEDCOLOR = QColor(235, 143, 52);
    static constexpr QColor LOCKDOWNCOLOR = QColor(255, 0, 0);

    /**
     * @brief PandemicLocation \n
     * Constructor for an Pandemic Location. A Pandemic Location contains all
     * the same members as an actual location, but also includes information
     * about it's type and pandmemic status
     * @param type: the type of location
     */
    PandemicLocation(double x, double y, Agent::LOCATIONS type);

    /**
     * @brief getType \n
     * Getter function for the Type of location this EconomicLocation is
     * @return an Agent::LOCATIONS enum representing a type of location
     */
    Agent::LOCATIONS getType();

    /**
     * @brief addInfectedAgent \n
     * Increments the number of infected agents associated with this Location.
     */
    void addInfectedAgent();

    /**
     * @brief removeInfectedAgent \n
     * Decrements the number of infected agents associated with this Location;
     */
    void removeInfectedAgent();

    /**
     * @brief getNumInfectedAgents \n
     * Getter function for the number of Infected agents associated with this
     * Locations
     * @return: the number of Infected agents at this location, as an int
     */
    int getNumInfectedAgents();

    /**
     * @brief getStatus \n
     * Getter function for the current Pandemic status of the Location
     * @return the current Pandemic status of the Location as an enum
     */
    PandemicLocation::STATUS getStatus();

    /**
     * @brief setStatus \n
     * Setter function for the current Pandemic status of the Location
     * @param newStatus: the new Status of the Location
     */
    void setStatus(PandemicLocation::STATUS newStatus);

    /** Destructor for the Pandemic Location class */
    ~PandemicLocation();

private:

    /** The type of Location associated with this Pandemic Location */
    Agent::LOCATIONS locationType;

    /** Current pandemic status of this Location */
    STATUS pandemicStatus;

    /** Current number of Infected individuals associated with this location */
    int numInfectedAgents;

};

#endif // PANDEMICLOCATION_H
