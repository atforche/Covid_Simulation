#ifndef PANDEMICAGENT_H
#define PANDEMICAGENT_H

#include "Agent.h"
#include "PandemicLocation.h"


/**
 * @brief The PandemicAgent class
 * Class that inherits from the Agent class. Extends the functionality of a
 * basic agent with the ability to participate in the SEIR model.
 * Pandmic Agents can also have different pandmic statuses (SUSCEPTIBLE,
 * EXPOSED, INFECTED, or RECOVERED)
 */
class PandemicAgent : public Agent {

public:

    /** Enum that indicates the current pandemic status of the agent */
    enum STATUS {SUSCEPTIBLE, EXPOSED, INFECTED, RECOVERED};

    /** Enum that indicates the overal health of the agent */
    enum HEALTH {HEALTHY, MODERATE, POOR, VERY_POOR};

    /**
     * @brief PandemicAgent \n
     * Constructor for the PandemicAgent class. Constructs a base agent that
     * also possesses the default Pandemic status.
     * @param age: the current age of the agent
     * @param startingLocation: a pointer to the Agent's starting location
     * @param startingLocationString: the type of location the agent starts at
     * @param behavior: the behavior chart assignment for the agent
     */
    PandemicAgent(int age, Location* startingLocation,
                  QString startingLocationString, int behavior);

    /**
     * @brief getStatus \n
     * Get the current economic status of the Agent, either NORMAL, HOMELESS,
     * UMEMPLOYED, or BOTH
     * @return the agent's economic status as an ENUM
     */
    STATUS getStatus();

    /**
     * @brief getHealthStatus \n
     * Getter function for the overall Health status of the agent
     * @return the agent's overall health status as an ENUM
     */
    HEALTH getHealthStatus();

    /**
     * @brief setStatus \n
     * Set the current economic status of the Agent
     * @param newStatus: the new economic status of the agent
     */
    void setStatus(STATUS newStatus);

    /**
     * @brief advanceDay \n
     * Advances the day for the current Agent. Increments the counter for the
     * number of days the Agent resides in each stage.
     * @returns: whether the Agent should be counted as a new case
     */
    bool advanceDay();

    /**
     * @brief makeSusceptible \n
     * Makes the Agent susceptible to the Virus.
     */
    void makeSusceptible();

    /**
     * @brief makeExposed \n
     * Makes the Agent exposed to the Virus.
     */
    void makeExposed();

    /**
     * @brief makeInfected \n
     * Makes the Agent infected with the Virus.
     */
    void makeInfected();

    /**
     * @brief makeRecovered \n
     * Makes the Agent recovered from the Virus.
     */
    void makeRecovered();

    /**
     * @brief evaluateDeathProbability \n
     * Function to determine if the Agent will die this hour or continue to live.
     * The probability that an infected agent will die is proportional to their
     * age and health status.
     * @returns: whether the Agent dies or not
     */
    bool evaluateDeathProbability();

    /**
     * @brief setCompliance \n
     * Setter function for whether the Agent is complying with mandates or not.
     * Compliance affects the reduction in virus spread associated with
     * mask-wearing and social distancing.
     * @param compliant: whether the Agent is complying
     */
    void setCompliance(bool compliant);

    /**
     * @brief resetNearbyInfected \n
     * Function that resets the count of nearby infected agents.
     */
    void resetNearbyInfected();

    /**
     * @brief incrementNearbyInfected \n
     * Setter function that increments the number of nearby agents. Increment
     * the number by one for a nearby exposed Agent and by two for a nearby
     * Infected agent.
     * @param amount: integer for the type of nearby agent. 1 for Exposed, 2 for Infected
     */
    void incrementNearbyInfected(int amount);

    /**
     * @brief evaluateInfectionProbability \n
     * Function to evaluate the probability of an Agent becoming exposed, based
     * on the number of nearby infected agents.
     * @return whether the agent will become Exposed or not
     */
    bool evaluateInfectionProbability(bool checkCompliance);

    /** Destructor for the Pandemic Agent class*/
    ~PandemicAgent();

private:

    /** Current economic status of the Agent */
    STATUS currentStatus;

    /** Overall health status of the Agent */
    HEALTH healthStatus;

    /** Whether the Agent is complying with Pandemic rules or not */
    bool compliant;

    /** Counter for the number of days the Agent has been in their current Pandemic status */
    int daysInStage;

    /** Counter for the number of nearby Infected Agents */
    int nearbyInfected;

};


#endif // PANDEMICAGENT_H
