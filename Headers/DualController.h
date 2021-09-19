#ifndef DUALCONTROLLER_H
#define DUALCONTROLLER_H

#include "PandemicController.h"
#include "EconomicController.h"


/**
 * @brief The DualController class.
 * An extension of the AgentController class to include functionality
 * from both the Pandemic Controller class and the Economic Controller class.
 * Enables the Agents in the Simulation to be controlled according to both
 * the Pandemic rules and the Economic rules
 */
class DualController : public PandemicController, public EconomicController {

private:


public:

    /**
     * @brief DualController \n
     * Constructor for the Dual Controller class. Acts as the main
     * controller for Dual Agents and Dual Locations. Enables the
     * behavior of Agents and Locations to change dynamically based on their
     * economic value and Pandemic status.
     * @param sim: pointer to the simulation to control
     */
    DualController(Simulation* sim);

    /**
     * @brief updateAgentDestinations \n
     * Function that goes through each of the agents and assigns them to their
     * correct destinations. Should be called from Simulation::advanceTime() at
     * an hourly interval to save computations
     * @param agents: a vector of Agent* pointing to each agent in the Simulation
     * @param hour: the current hour of the simulation
     */
    virtual void updateAgentDestinations(std::vector<Agent*> &agents, int hour) override;

    /**
     * @brief updateSingleDestination \n
     * Updates the Destination for a single agent based on their Behavior chart
     * @param agent: pointer to the agent to be updated
     * @param hour: current hour in the Simulation
     * @param randomAllowed: whether Agent movement to random Locations is allowed
     */
    virtual void updateSingleDestination(Agent* agent, int hour, bool randomAllowed) override;

    /**
     * @brief lockdownLocations \n
     * Function that will move through every location and lock them down if
     * necessary, depending on the threshold specified in the UI.
     */
    virtual void lockdownLocations() override;

    /**
     * @brief businessEconomicUpdate \n
     * Perform the economic update for the Business locations. Each business
     * location loses a fixed amount of value during business hours, and a
     * business will go bankrupt if they run out of money.
     * @param hour: the current hour in the Simulation
     */
    virtual void businessEconomicUpdate(int hour) override;

    /**
     * @brief homeEconomicUpdate \n
     * Perform the economic updates associated with Home locations. When at home,
     * agents lose value to various expenses, and some of that value gets given
     * back to businesses. Agents can go homeless while at home.
     * @param agent: agent to be updated
     * @returns value lost by agent that should be distributed to businesses
     */
    virtual double homeEconomicUpdate(EconomicAgent* agent) override;

    /**
     * @brief workEconomicUpdate \n
     * Perform the economic updates associated with Work locations. When at work,
     * agents gain value from working, and some of the value gets taken from
     * the business. Businesses can go bankrupt while workers are there
     * @param agent: agent to be updated
     */
    virtual void workEconomicUpdate(EconomicAgent* agent) override;

    /**
     * @brief makeHomeless \n
     * Makes an agent homeless. Updates their status and sets their Home location
     * to nullptr
     * @param agent: pointer to the agent to be modified
     */
    virtual void makeHomeless(EconomicAgent* agent) override;

    /**
     * @brief finishEconomicUpdate \n
     * Performs the finishing operations on the Economic update. Enables new
     * businesses to be created if there are less than the maximum number.
     * Additionally, equal distributed the value lost by Agents at home locations
     * among all the businesses.
     * @param redistributedValue: the total amount of value to be distributed to businesses
     * @param type: the type of location for a newly generated business to be
     */
    virtual void finishEconomicUpdate(double redistributedValue, QString type = "Dual") override;

};


#endif // DUALCONTROLLER_H
