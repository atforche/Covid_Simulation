#ifndef ECONOMICCONTROLLER_H
#define ECONOMICCONTROLLER_H

#include <algorithm>
#include <deque>

#include "EconomicAgent.h"
#include "Simulation.h"
#include "AgentController.h"

// Forward declaration
class EconomicSimulation;

/**
 * @brief The EconomicController class
 * Class that enforces the economic rules of the Economic Simulation. As agents
 * move around to different locations, they gain and lose value according to
 * difference rules. Business locations can disappear and appear. Agent's
 * behavior can be dictated by their economic status.
 */
class EconomicController : virtual public AgentController {

private:

    /** Pointer to the Simulation this Controller interacts with */
    EconomicSimulation* sim;

    /** Int to store the total value in all the agents */
    int agentValue;

    /** Int to store the total value in all businesses */
    int businessValue;

    /** Deque to store the change in Economic value over the past 24 hours */
    std::deque<double> changeInValue;

    /** The value at the previous step in the Simulation */
    int previousValue;

public:

    /** Constant overhead each business pays each hour during the day */
    static constexpr int workOverhead = 15;

    /** Proportion of worker's wages the business actually loses */
    static constexpr double workLossProportion = 0.50;

    /** Proportion of agent's home expenses that go to businesses */
    static constexpr double homeLossProportion = 0.85;

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
     * @brief updateAgentDestinations \n
     * Function that goes through each of the agents and assigns them to their
     * correct destinations. Should be called from Simulation::advanceTime() at
     * an hourly interval to save computations
     * @param agents: a vector of Agent* pointing to each agent in the Simulation
     * @param hour: the current hour of the simulation
     */
    virtual void updateAgentDestinations(std::vector<Agent*> &agents, int hour);

    /**
     * @brief businessEconomicUpdate \n
     * Perform the economic update for the Business locations. Each business
     * location loses a fixed amount of value during business hours, and a
     * business will go bankrupt if they run out of money.
     * @param hour: the current hour in the Simulation
     */
    virtual void businessEconomicUpdate(int hour);

    /**
     * @brief agentEconomicUpdate \n
     * Perform the economic update for the Agents. Agents gain and lose value
     * depending on their current location. Agents can also become homeless if
     * they run out of money, or unemployed if their place of work goes
     * bankrupt.
     * @param agent: the agent to perform the update on
     * @return the amount of value the agent lost that will be redistributed to all businesses
     */
    virtual double agentEconomicUpdate(EconomicAgent* agent);

    /**
     * @brief finishEconomicUpdate \n
     * Performs the finishing operations on the Economic update. Enables new
     * businesses to be created if there are less than the maximum number.
     * Additionally, equal distributed the value lost by Agents at home locations
     * among all the businesses.
     * @param redistributedValue: the total amount of value to be distributed to businesses
     * @param type: the type of location for a newly generated business to be
     */
    virtual void finishEconomicUpdate(double redistributedValue, QString type = "Economic");

    /**
     * @brief bankruptBusiness \n
     * Bankrupts the victim EconomicLocation, which removes it and it's sibling
     * leisure location from the Simulation. All agents assigned to the business
     * location become unemployed. All agents assigned to the leisure location
     * are randomly assigned to a new leisure location.
     * @param victim: the EconomicLocation to be destroyed
     * @param index: the index in the locations vector
     */
    void bankruptBusiness(EconomicLocation* victim);

    /**
     * @brief makeHomeless \n
     * Makes an agent homeless. Updates their status and sets their Home location
     * to nullptr
     * @param agent: pointer to the agent to be modified
     */
    virtual void makeHomeless(EconomicAgent* agent);

    /**
     * @brief makeUnemployed \n
     * Makes an agent unemployed. Updates their status and sets their Work location
     * to nullptr
     * @param agent: pointer to the agent to be modified
     */
    void makeUnemployed(EconomicAgent* agent);

    /**
     * @brief homeEconomicUpdate \n
     * Perform the economic updates associated with Home locations. When at home,
     * agents lose value to various expenses, and some of that value gets given
     * back to businesses. Agents can go homeless while at home.
     * @param agent: agent to be updated
     * @returns value lost by agent that should be distributed to businesses
     */
    virtual double homeEconomicUpdate(EconomicAgent* agent);

    /**
     * @brief workEconomicUpdate \n
     * Perform the economic updates associated with Work locations. When at work,
     * agents gain value from working, and some of the value gets taken from
     * the business. Businesses can go bankrupt while workers are there
     * @param agent: agent to be updated
     */
    virtual void workEconomicUpdate(EconomicAgent* agent);

    /**
     * @brief schoolEconomicUpdate \n
     * Perform the economic updates associated with School locations. When at school,
     * agents gain a small amount of value
     * @param agent: agent to be updated
     */
    void schoolEconomicUpdate(EconomicAgent* agent);

    /**
     * @brief leisureEconomicUpdate \n
     * Perform the economic updates associated with Leisure locations. When at leisure,
     * agents lose value, all of which goes back to the sibling business location.
     * Agents have the potential to run out of money, but will not become homeless
     * until they return back home
     * @param agent
     */
    void leisureEconomicUpdate(EconomicAgent* agent);

    /**
     * @brief generateNewBusiness \n
     * Creates a new business to add to the Simulation. Gives the business a
     * small amount of initial value to get started. Selects a small number of
     * unemployed individuals to work at this location (if no unemployed agents,
     * select a single employed agent). Also, creates a new sibling leisure
     * location and assigned a small number of agents to it.
     */
    void generateNewBusiness(QString type);

    /**
     * @brief getTotalAgentValue \n
     * Getter function for the total amount of value held by Agents in the
     * Simulation
     * @return an int
     */
    int getTotalAgentValue();

    /**
     * @brief getTotalBusinessValue \n
     * Getter function for the total amount of value held by Businesses in the
     * Simulation
     * @return an int
     */
    int getTotalBusinessValue();

    /**
     * @brief setTotalBusinessValue \n
     * Setter function for the total amoutn of value held by Agents in the
     * Simulation
     */
    void setTotalAgentValue(int newValue);

    /**
     * @brief setTotalBusinessValue \n
     * Setter function for the total amoutn of value held by Businesses in the
     * Simulation
     */
    void setTotalBusinessValue(int newValue);

    /**
     * @brief getState \n
     * Gets the attributes used to train the controlling network
     * @return a vector of the state attributes
     */
    std::vector<double> getState();

    /**
     * @brief finishEconomicState \n
     * Function that updates the Economic portion of the Simulation state
     * @param newTotalValue: new total value of the Simulation
     */
    void finishEconomicState();

};

// Include at end to avoid circular dependency
#include "EconomicSimulation.h"

#endif // ECONOMICCONTROLLER_H
