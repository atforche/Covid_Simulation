#ifndef AGENT_H
#define AGENT_H

/**
 * @brief The Agent Class
 * Class that specifies the functionality of the Agents in the simulation
 * Contains all information required for the agent to act in the environment
 */

#include <Headers/Coordinate.h>
#include <ui_mainwindow.h>
#include <QGraphicsRectItem>

class Agent {

private:
    /** The current location of the Agent as a Coordinate*/
    Coordinate* position;

    /** The current age of the Agent as an Integer */
    int age;

    /** Int representing how wide the agents should be rendered as */
    const int AGENT_WIDTH = 2;

    /** Rectangle corresponding to this rendered agent */
    QGraphicsRectItem* rect;

public:

    /**
     * @brief Agent
     * Constructor for the Agent Class
     * @param age: the current age of the Agent
     */
    Agent(int age);

    /**
     * @brief renderAgent
     * Function to generate the QGraphicsRectItem that represents this
     * specific agent. Return value should be stored within Simulation
     * and added to UI element within simulation
     */
    QGraphicsRectItem* renderAgent();

    /**
     * @brief update
     * Function to update and redraw the agent in a new position
     */
    void update();

    /**
     * @brief ~Agent
     * Destructor to free memory from the Agent class
     */
    ~Agent();

};

#endif // AGENT_H
