#ifndef PANDEMICCONTROLLER_H
#define PANDEMICCONTROLLER_H

#include <algorithm>
#include <deque>

#include "PandemicAgent.h"
#include "Simulation.h"

// Forward declaration
class PandemicSimulation;

/**
 * @brief The PandemicController class
 * Class that enforces the economic rules of the Pandemic Simulation. As agents
 * move around to different locations, they have the change to become infected
 * with a virus, transmit that virus, and even die. Agent's
 * behavior can be dictated by their infected status.
 */
class PandemicController : virtual public AgentController {

private:

    /** Pointer to the Simulation this Controller interacts with */
    PandemicSimulation* sim;

    /** Vector to store all cast Agents */
    std::vector<PandemicAgent*> pandemicAgents;

    /** Whether an Agent's compliance will affect Infection spread */
    bool checkCompliance;

    /** Total number of Deaths in the past day */
    int newDailyDeaths;

    /** Total number of new Cases in the past day */
    int newDailyCases;

    /** Total number of Deaths overall */
    int totalDeaths;

    /** Total number of Cases overall */
    int totalCases;

    /** Total number of Agents that are currently Susceptible */
    int numSusceptible;

    /** Total number of Agents that are currently Exposed */
    int numExposed;

    /** Total number of Agents that are currently Infected */
    int numInfected;

    /** Total number of Homeless Agents that are currently infected */
    int numHomelessInfected;

    /** Total number of Agents that are currently Recovered */
    int numRecovered;

    /** The number of new exposures over the past 24 hours */
    std::deque<int> last24Exposed;

    /** The number of deaths over the past 24 hours */
    std::deque<int> last24Deaths;

    /** Boolean to store whether the initial infection has already happened */
    bool initialInfection;

    /** Radius through which the infection can spread */
    static constexpr int INFECTION_RADIUS = 16;

public:

    /**
     * @brief PandemicController \n
     * Constructor for the Economic Controller class. Acts as the main
     * controller for Pandemic Agents and Pandemic Locations. Enables the
     * behavior of Agents and Locations to change dynamically based on their
     * infected value.
     * @param sim: pointer to the simulation to control
     */
    PandemicController(Simulation* sim);

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
     * @brief updateSingleDestination \n
     * Updates the Destination for a single agent based on their Behavior chart
     * @param agent: pointer to the agent to be updated
     * @param hour: current hour in the Simulation
     * @param randomAllowed: whether Agent movement to random Locations is allowed
     */
    virtual void updateSingleDestination(Agent* agent, int hour, bool randomAllowed);

    /**
     * @brief initializePandemicUpdate \n
     * Initialize the variables needed to carry out the Pandemic update. Resets
     * the nearbyInfected counts for each of the Agents. Additionally, casts each
     * agent to a Pandemic Agent and stores then within the class.
     * @param agents
     */
    void initializePandemicUpdate(std::vector<Agent*> &agents);

    /**
     * @brief agentPandemicUpdate \n
     * Performs the Pandemic update on a single agent. Counts the number of
     * agents in each Pandemic status, and gives infected agent a chance to die.
     * Additionally, counts the number of nearby infected agents near each agent.
     * @param agent: the agent to update
     * @param i: the index of the current agent
     * @return whether the agent has died or not
     */
    virtual bool agentPandemicUpdate(PandemicAgent* agent, int i);

    /**
     * @brief spreadInfection \n
     * Spreads the infection to Susceptible agents. The probability of a
     * Susceptible agent becoming Exposed is proportional to the number
     * of infected agents that are close to the agent in questions
     * @param pandemicAgents: a std::vector of PandemicAgents
     */
    void spreadInfection(std::vector<PandemicAgent*> &pandemicAgents);

    /**
     * @brief spontaneousInfection \n
     * If the virus has become extinct in the population, give the virus a
     * chance to spontaneously reappear. Mimics someone from an outside population
     * coming in and infected locals.
     */
    void spontaneousInfection();

    /**
     * @brief advanceDay \n
     * Starts a new day for the pandemic controller. Updates the charts that
     * display the daily counts of new Cases and Deaths.
     */
    void advanceDay();

    /**
     * @brief getTotalCases \n
     * Getter function for the total number of Cases that have occurred in the
     * Simulation
     * @return the number of total cases as an int
     */
    int getTotalCases();

    /**
     * @brief getDailyCases \n
     * Getter function for the number of new Cases that have occurred in the
     * past day
     * @return the number of cases in the past day as an int
     */
    int getDailyCases();

    /**
     * @brief getTotalDeaths \n
     * Getter function for the total number of Deaths that have occurred in the
     * Simulation
     * @return the number of total deaths as an int
     */
    int getTotalDeaths();

    /**
     * @brief getDailyDeaths \n
     * Getter function for the number of new Deaths that have occured in the
     * past day
     * @return the number of deaths in the past day as an int
     */
    int getDailyDeaths();

    /**
     * @brief getSEIR \n
     * Returns the number of Susceptible, Exposed, Infected, and Recovered
     * agents respectively
     * @return a std::vector<int>
     */
    std::vector<int> getSEIR();

    /**
     * @brief beginInfection \n
     * Function that causes the initial infection to occur in the Simulation.
     * Infects a fixed number of random agents with the virus.
     */
    void beginInfection();

    /**
     * @brief enforceLockdowns \n
     * Function that enforces Lockdowns on certain locations. If a location is
     * locked down, no Agents will be able to travel there. Home locations can
     * never be locked down. Agents locked out of WORK or SCHOOL will just
     * stay home. Agents locked out of LEISURE will try a second leisure location
     * before going home.
     */
    void enforceLockdowns(PandemicAgent* agent, Location* home);

    /**
     * @brief enforceQuarantine \n
     * Function that enforces quarantines for certain agents. If an Agent is
     * infected and quarantines are enforced, the Agent will be unable to leave
     * their house until their infection has subsided.
     */
    void enforceQuarantine(PandemicAgent* agent, Location* home);

    /**
     * @brief enforceContactTracing \n
     * Function that enforces contact tracing for certain Agents. If an Agent
     * has been exposed to an Infected agent, they may be subject to contact
     * tracing. An agent being restricted by contact tracing can leave their
     * house only for a short time, and they spend most of their time at home.
     */
    void enforceContactTracing(PandemicAgent* agent, Location* home);

    /**
     * @brief willComply \n
     * Function to determine if a current agent will comply with the Pandemic
     * Rules. Returns TRUE if the Agent will comply and adjust their schedule
     * to comform to the pandemic rules. Returns FALSE if the Agent will not
     * comply and will proceed with their regular schedule.
     * @return a bool
     */
    bool willComply();

    /**
     * @brief enforceGuidelines \n
     * Function that enforces Government guidelines surrounding the behavior
     * of Agents in the Simulation. Gives Agents a chance to avoid traveling to
     * a Leisure location. Additionally, strong guidelines lead to more
     * mask-wearing and social distancing, which reduces spread.
     */
    void enforceGuidelines(PandemicAgent* agent, Location* home);

    /**
     * @brief applyECommerce \n
     * Functionn that applies E-Commerce to the Simulation. E-Commerce allows
     * some Agents to go to Work, School, or Leisure locations from Home. In a
     * Dual simulation, working from Home results in slightly less Economic value.
     * @return: whether the agent is participating in the Economic from home
     */
    bool applyECommerce(PandemicAgent* agent, Location* home);

    /**
     * @brief lockdownLocations \n
     * Function that will move through every location and lock them down if
     * necessary, depending on the threshold specified in the UI.
     */
    virtual void lockdownLocations();

    /**
     * @brief incrementNearbyInfected \n
     * Function to increment the number of nearby infected for all Agents within
     * a constant radius of the specified agent
     * @param pandemicAgents: std::vector of pointers to all Pandemic Agents in Simulation
     * @param index: index of the Pandemic Agent to compare against
     */
    void incrementNearbyInfected(std::vector<PandemicAgent*> &pandemicAgents, size_t index);

    /**
     * @brief getPandemicAgents \n
     * Getter function for the vector of PandemicAgents that are cast from the
     * base agent vector
     * @return the address of the PandemicAgent vector
     */
    std::vector<PandemicAgent*>& getPandemicAgents();

    /**
     * @brief getNumHomelessInfected \n
     * Getter function for the number of Homeless agents that are infected
     * @return
     */
    int getNumHomelessInfected();

    /**
     * @brief getState \n
     * Gets the attributes used to train the controlling network
     * @return a vector of the state attributes
     */
    std::vector<double> getState();

    /**
     * @brief killAgent \n
     * Updates the Simulation state on an Agent's death
     */
    void killAgents(int numDeaths);

};

// Include at end to avoid circular dependency
#include "PandemicSimulation.h"

#endif // PANDEMICCONTROLLER_H
