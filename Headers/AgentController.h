#ifndef AGENTCONTROLLER_H
#define AGENTCONTROLLER_H

#include <vector>

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QString>
#include <QDir>

#include "Agent.h"

// Forward declaration of the Simulation Class
class Simulation;

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

    /** Vector containing the weighted sums for each adult behavior */
    std::vector<int> adultProbabilities;

    /** Total weight sum for adult behaviors */
    int adultWeightedSum;

    /** Vector of QJsonObjects specifying different child behaviors*/
    std::vector<QJsonObject> childBehaviors;

    /** Vector containing the weighted sums for each adult behavior */
    std::vector<int> childProbabilities;

    /** Total weight sum for adult behaviors */
    int childWeightedSum;

    /** Pointer to the controlling Simulation */
    Simulation* sim;

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
    QJsonValue readAssignmentFromJSON(int behaviorChart, int hour,
                                            bool isAdult);

public:
    /**
     * @brief AgentController \n
     * Initial constructor for the Agent Controller class. Has controller
     * read in each of the behaviors from the JSON resource files and store
     * them as QJsonObjects in a vector. Dynamically determines the number of
     * adult and child behaviors by reading the entries in the bin/behaviors
     * directory
     */
    AgentController(Simulation* sim);

    /**
     * @brief getNumAdultBehaviors \n
     * Getter function for the number of unique adult behaviors that an adult
     * agent could possibly have
     * @return the number of unique behaviors as an integer
     */
    int getNumAdultBehaviors();

    /**
     * @brief getAdultBehavior \n
     * Getter function for a random adult behavior. Selects one of the adult
     * behavior charts according to the probabilities included in the JSON.
     * @return integer index of the behavior chart
     */
    int getAdultBehavior();

    /**
     * @brief numChildBehaviors \n
     * Getter function for the number of unique child behaviors that a child
     * agent could possibly have
     * @return the number of child behaviors
     */
    int getNumChildBehaviors();

    /**
     * @brief getChildBehavior \n
     * Getter function for a random child behavior. Selects one of the child
     * behavior charts according to the probabilities included in the JSON.
     * @return integer index of the behavior chart
     */
    int getChildBehavior();

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
     * @brief getStartingDestination \n
     * Function that returns the specific destination assignment for a specific
     * behavior chart at th start of the simulation (hour 0)
     * @param behaviorChart: the behavior chart to query
     * @param isAdult: determines whether to check the adult beahavior or the
     *        child behavior chart
     * @return QString representing the starting destination of the Agent
     */
    QString getStartingDestination(int behaviorChart, bool isAdult);

    /**
     * @brief getAgentDestination \n
     * Determines what location the agent should be assigned to. Reads the
     * behavior chart to determine the possible locations. Samples which
     * location should be assigned according to the probabilities. Returns
     * the QString corresponding to the selected location
     * @param agent: the agent who will be assigned
     * @param hour: the current hour in the simulation
     * @return which location the agent will be assigned to
     */
    QString getAgentDestination(Agent* agent, int hour);

    /**
     * @brief getAgentLock \n
     * Getter function for the Mutex that locks the Agent vector
     * @return
     */
    QMutex* getAgentLock();

    /**
     * @brief getLocationLock \n
     * Getter function for the Mutex that locks the Locations vector
     * @return
     */
    QMutex* getLocationLock();

    /**
     * @brief updateSingleDestination \n
     * Updates the Destination for a single agent based on their Behavior chart
     * @param agent: pointer to the agent to be updated
     * @param hour: current hour in the Simulation
     * @param randomAllowed: whether Agent movement to random locations is allowed
     */
    virtual void updateSingleDestination(Agent* agent, int hour, bool randomAllowed);

    /**
     * @brief getSim \n
     * Getter function for the Simulation this Controller interacts with
     * @return
     */
    virtual Simulation* getSim();

    /**
     * @brief sampleAgentAge \n
     * Generates a new random age for an agent that is sampled from the standard
     * age distribution of the United States.
     * @return the randomly sampled age as an integer
     */
    static int sampleAgentAge();

    /** Virtual Destructor of the AgentController class */
    virtual ~AgentController();

};

// Rear includes to avoid circular dependencies
#include "Simulation.h"

#endif // AGENTCONTROLLER_H
