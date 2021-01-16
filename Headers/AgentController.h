#ifndef AGENTCONTROLLER_H
#define AGENTCONTROLLER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QString>
#include <vector>
#include "Agent.h"

/**
 * @brief The AgentController class
 * Class that sits within the Simulation and dynamically determines each
 * agents destination based on its assigned behavior plan. Reads in each
 * of the behavior plans from their JSON resources files, which specify the
 * destination of the agent at every hour of the day
 */
class AgentController {

private:
    /** The number of unique behaviors that agents can have */
    int numBehaviors;

    /** Vector containing the QJsonObjects that specify different behaviors*/
    std::vector<QJsonObject> behaviors;

public:
    /**
     * @brief AgentController \n
     * Initial constructor for the Agent Controller class. Has controller
     * read in each of the behaviors from the JSON resource files and store
     * them as QJsonObjects in a vector.
     * @param numBehaviors: int that specifies the number of unique behaviors
     *                      to read in
     */
    AgentController(int numBehaviors);

    /**
     * @brief getNumBehaviors \n
     * Getter function for the number of unique behaviors that the Agent
     * controller is currently supporting
     * @return the number of unique behaviors as an integer
     */
    int getNumBehaviors();

    /**
     * @brief assignAgentDestinations \n
     * Function that goes through each of the agents and assigns them to their
     * correct destinations. Should be called from Simulation::advanceTime() at
     * an hourly interval to save computations
     * @param agents: a vector of Agent* pointing to each agent in the Simulation
     * @param hour: the current hour of the simulation
     */
    void assignAgentDestinations(std::vector<Agent*> agents, int hour);

};

#endif // AGENTCONTROLLER_H
