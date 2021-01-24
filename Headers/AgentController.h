#ifndef AGENTCONTROLLER_H
#define AGENTCONTROLLER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QString>
#include <QDir>
#include <vector>
#include "Agent.h"

/**
 * @brief The AgentController class \n
 * Class that sits within the Simulation and dynamically determines each
 * agents destination based on its assigned behavior plan. Reads in each
 * of the behavior plans from their JSON resources files, which specify the
 * destination of the agent at every hour of the day
 */
class AgentController {

private:

    /** Vector containing the QJsonObjects that specify different behaviors*/
    std::vector<QJsonObject> adultBehaviors;

    /** Vector of QJsonObjects specifying different child behaviors*/
    std::vector<QJsonObject> childBehaviors;

public:
    /**
     * @brief AgentController \n
     * Initial constructor for the Agent Controller class. Has controller
     * read in each of the behaviors from the JSON resource files and store
     * them as QJsonObjects in a vector. Dynamically determines the number of
     * adult and child behaviors by reading the entries in the bin/behaviors
     * directory
     */
    AgentController();

    /**
     * @brief getNumBehaviors \n
     * Getter function for the number of unique adult behaviors that an adult
     * agent could possibly have
     * @return the number of unique behaviors as an integer
     */
    int numAdultBehaviors();

    /**
     * @brief numChildBehaviors \n
     * Getter function for the number of unique child behaviors that a child
     * agent could possibly have
     * @return the number of child behaviors
     */
    int numChildBehaviors();

    /**
     * @brief assignAgentDestinations \n
     * Function that goes through each of the agents and assigns them to their
     * correct destinations. Should be called from Simulation::advanceTime() at
     * an hourly interval to save computations
     * @param agents: a vector of Agent* pointing to each agent in the Simulation
     * @param hour: the current hour of the simulation
     */
    void assignAgentDestinations(std::vector<Agent*> agents, int hour);

    /**
     * @brief getDestinationAssignment \n
     * Function that returns the specific destination assignment for a specific
     * behavior chart at a specific hour
     * @param behaviorChart: the behavior chart to query
     * @param hour: the hour of the day to query
     * @param isAdult: determines whether to check the adult beahavior or the
     *        child behavior chart
     * @return
     */
    QString getDestinationAssignment(int behaviorChart, int hour, bool isAdult);

};

#endif // AGENTCONTROLLER_H
