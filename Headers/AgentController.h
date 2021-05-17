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
     * @brief getNumAdultBehaviors \n
     * Getter function for the number of unique adult behaviors that an adult
     * agent could possibly have
     * @return the number of unique behaviors as an integer
     */
    int getNumAdultBehaviors();

    /**
     * @brief numChildBehaviors \n
     * Getter function for the number of unique child behaviors that a child
     * agent could possibly have
     * @return the number of child behaviors
     */
    int getNumChildBehaviors();

    /**
     * @brief updateAgentDestinations \n
     * Function that goes through each of the agents and assigns them to their
     * correct destinations. Should be called from Simulation::advanceTime() at
     * an hourly interval to save computations
     * @param agents: a vector of Agent* pointing to each agent in the Simulation
     * @param hour: the current hour of the simulation
     */
    void updateAgentDestination(std::vector<Agent*> agents, int hour);

    /**
     * @brief getStartingDestination \n
     * Function that returns the specific destination assignment for a specific
     * behavior chart at th start of the simulation (hour 0)
     * @param behaviorChart: the behavior chart to query
     * @param isAdult: determines whether to check the adult beahavior or the
     *        child behavior chart
     * @return
     */
    QString getStartingDestination(int behaviorChart, bool isAdult);

    /**
     * @brief evaluateDestinationProbabilities \n
     * Evaluates the different possible locations and their probabilities.
     * Randomly selects a single location with weighted probability.
     * @param keys: different locations to select from
     * @param probabilities: unique weighted probability for each location
     * @return the selected location
     */
    QString evaluateDestinationProbabilities(QStringList &keys,
                                             std::vector<double> &probabilities);

    /**
     * @brief extractDestinationAssignment \n
     * Returns the destination assignment for the given behaviorChart at the
     * given hour as a QJsonValue. Returns a JSON string if single destination
     * is specified. Returns a JSON object is multiple destinations are
     * possible. Returns a JSON string of "No Change" if hour is not specified
     * @param behaviorChart: behavior chart assigned to agent
     * @param hour: hour for which to search the chart
     * @param isAdult: whether the agent is an adult
     * @return the destination assignment for the given hour
     */
    QJsonValue extractDestinationAssignment(int behaviorChart, int hour,
                                            bool isAdult);

    /**
     * @brief getDestinationAssignment \n
     * Determines what location the agent should be assign to. Reads the
     * behavior chart to determine the possible locations. Samples which
     * location should be assigned according to the probabilities. Returns
     * the QString corresponding to the selected location
     * @param agent: the agent who will be assigned
     * @param hour: the current hour in the simulation
     * @return which location the agent will be assigned to
     */
    QString getDestinationAssignment(Agent* agent, int hour);

};

#endif // AGENTCONTROLLER_H
