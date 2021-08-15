#ifndef PANDEMICCONTROLLER_H
#define PANDEMICCONTROLLER_H

#include <algorithm>

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
class PandemicController {

private:

    /** Pointer to the Simulation this Controller interacts with */
    PandemicSimulation* sim;

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

    /** Total number of Agents that are currently Recovered */
    int numRecovered;

    /** Map storing the value for each response flag */
    std::map<std::string, std::string> flags;

    /** Boolean to store whether the initial infection has already happened */
    bool initialInfection;

public:

    /**
     * @brief PandemicController \n
     * Constructor for the Economic Controller class. Acts as the main
     * controller for Pandemic Agents and Pandemic Locations. Enables the
     * behavior of Agents and Locations to change dynamically based on their
     * infected value.
     * @param sim: pointer to the simulation to control
     * @param flags: map that stores the status of each Pandemic Response
     *               flag from the UI
     */
    PandemicController(Simulation* sim, std::map<std::string, std::string> flags);

    /**
     * @brief executePandemicUpdate \n
     * Executes the transmission of the virus in the Simulation. Each hour each
     * agent can either get the virus from another agent or give the virus to
     * another agent. Different flags determine the exact actions that agents
     * or locations will take when an infected agent arrives at their location.
     * @returns: the total number of Agents in each SEIR category
     */
    std::vector<int> executePandemicUpdate();

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
     * @brief beginInfection \n
     * Function that causes the initial infection to occur in the Simulation.
     * Infects a fixed number of random agents with the virus.
     */
    void beginInfection();

    /**
     * @brief overrideAgentDestinations \n
     * Overrides the Agent's destination assignments according to the current
     * rules of the Pandemic.
     * @param agents: vector of all Agents in the Simulation
     */
    void overrideAgentDestinations(std::vector<Agent*> &agents);

    /**
     * @brief enforceLockdowns \n
     * Function that enforces Lockdowns on certain locations. If a location is
     * locked down, no Agents will be able to travel there. Home locations can
     * never be locked down. Agents locked out of WORK or SCHOOL will just
     * stay home. Agents locked out of LEISURE will try a second leisure location
     * before going home.
     */
    void enforceLockdowns(PandemicAgent* agent, PandemicLocation* home);

    /**
     * @brief enforceQuarantine \n
     * Function that enforces quarantines for certain agents. If an Agent is
     * infected and quarantines are enforced, the Agent will be unable to leave
     * their house until their infection has subsided.
     */
    void enforceQuarantine(PandemicAgent* agent, PandemicLocation* home);

    /**
     * @brief enforceContactTracing \n
     * Function that enforces contact tracing for certain Agents. If an Agent
     * has been exposed to an Infected agent, they may be subject to contact
     * tracing. An agent being restricted by contact tracing can leave their
     * house only for a short time, and they spend most of their time at home.
     */
    void enforceContactTracing(PandemicAgent* agent, PandemicLocation* home, PandemicLocation* school,
                               PandemicLocation* work, PandemicLocation* leisure);

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
    void enforceGuidelines(PandemicAgent* agent, PandemicLocation* home);

    /**
     * @brief applyECommerce \n
     * Functionn that applies E-Commerce to the Simulation. E-Commerce allows
     * some Agents to go to Work, School, or Leisure locations from Home. In a
     * Dual simulation, working from Home results in slightly less Economic value.
     */
    void applyECommerce(PandemicAgent* agent, PandemicLocation* home);

};

// Include at end to avoid circular dependency
#include "PandemicSimulation.h"

#endif // PANDEMICCONTROLLER_H
