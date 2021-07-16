#ifndef ECONOMICCONTROLLER_H
#define ECONOMICCONTROLLER_H

#include "Simulation.h"

class EconomicController {

private:

    /** Pointer to the Simulation this Controller interacts with */
    Simulation* sim;

    /** Proportion of worker's wages the business actually loses */
    static constexpr double workLossProportion = 0.50;

    /** Proportion of agent's home expenses that go to businesses */
    static constexpr double homeLossProportion = 0.75;

    /** Constant overhead each business pays each hour during the day */
    static constexpr int workOverhead = 8;

    /** Int to store the total value in all the agents */
    int agentValue;

    /** Int to store the total value in all businesses */
    int businessValue;

public:

    /**
     * @brief EconomicController \n
     * Constructor for the Economic Controller class. Acts as the main
     * controller for Economic Agents and Economic Locations. Enables the
     * behavior of Agents and Locations to change dynamically based on their
     * economic value.
     * @param sim: pointer to the simulation to control
     */
    EconomicController(Simulation* sim);

    /**
     * @brief executeEconomicUpdate \n
     * Executes the flow of Economic value in the Simulation. Each hour each
     * agent either gains or losses value depending on their current location.
     * Agents lose value when they are at their home or leisure locations.
     * Agents gain value when they are at work or school
     * @returns: the total Economic value held by all agents
     */
    int* executeEconomicUpdate();

};

#endif // ECONOMICCONTROLLER_H
