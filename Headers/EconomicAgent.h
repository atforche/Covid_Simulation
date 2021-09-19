#ifndef ECONOMICAGENT_H
#define ECONOMICAGENT_H

#include "Agent.h"


/**
 * @brief The EconomicAgent class
 * Class that inherits from the Agent class. Extends the functionality of a
 * basic agent with the ability to possess, gain, and lose economic value.
 * Economic Agents can also have different economic statuses (NORMAL,
 * UNEMPLOYED, HOMELESS, or BOTH)
 */
class EconomicAgent : virtual public Agent {

public:

    /** Enum that indicates the current economic status of the agent */
    enum STATUS {NORMAL, UNEMPLOYED, HOMELESS, BOTH};

    /**
     * @brief EconomicAgent \n
     * Constructor for the EconomicAgent class. Constructs a base agent that
     * also possesses a certain amount of Economic value.
     * @param initialValue: the initial economic value the agent has
     * @param age: the current age of the agent
     * @param startingLocation: a pointer to the Agent's starting location
     * @param startingLocationString: the type of location the agent starts at
     * @param behavior: the behavior chart assignment for the agent
     */
    EconomicAgent(int initialValue, int age, Location* startingLocation,
                  QString startingLocationString, int behavior);

    /**
     * @brief incrementValue \n
     * Increments the amount of economic value that the Agent possesses by
     * a certain amount.
     * @param amount: amount to increase the agent's value by
     * @return the current value of the agent
     */
    int incrementValue(int amount);

    /**
     * @brief decrementValue \n
     * Decrements the amount of economic value that the Agent possesses. If the
     * value that an agent possesses falls below zero, there are consequences
     * that are yet to be implemented
     * @param amount: amount to decrease the agent's value by
     * @return the current value of the agent
     */
    int decrementValue(int amount);

    /**
     * @brief getValue \n
     * Getter function for the current value of the Agent.
     * @return the current value of the agent as an int
     */
    int getValue();

    /**
     * @brief getStatus \n
     * Get the current economic status of the Agent, either NORMAL, HOMELESS,
     * UMEMPLOYED, or BOTH
     * @return the agent's economic status as an ENUM
     */
    STATUS getStatus();

    /**
     * @brief setStatus \n
     * Set the current economic status of the Agent
     * @param newStatus: the new economic status of the agent
     */
    void setStatus(STATUS newStatus);


private:

    /** Integer that stores the current economic value the agent possesses */
    int economicValue;

    /** Current economic status of the Agent */
    STATUS currentStatus;

};

#endif // ECONOMICAGENT_H
