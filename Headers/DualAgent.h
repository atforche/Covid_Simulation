#ifndef DUALAGENT_H
#define DUALAGENT_H

#include "PandemicAgent.h"
#include "EconomicAgent.h"


/**
 * @brief The DualAgent class \n
 * Class that extends the base Agent class to include both Economic value
 * and a Pandemic status.
 */
class DualAgent : public PandemicAgent, public EconomicAgent {

private:


public:

    /**
     * @brief DualAgent \n
     * Constructor for the DualAgent class. Constructs a base agent that
     * also possesses a certain amount of Economic value and Pandemic Status.
     * @param initialValue: the initial economic value the agent has
     * @param age: the current age of the agent
     * @param startingLocation: a pointer to the Agent's starting location
     * @param startingLocationString: the type of location the agent starts at
     * @param behavior: the behavior chart assignment for the agent
     */
    DualAgent(int initialValue, int age, Location* startingLocation,
                  QString startingLocationString, int behavior);

};

#endif // DUALAGENT_H
