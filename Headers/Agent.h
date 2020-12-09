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
#include "Location.h"

class Agent {

private:
    /** The current location of the Agent as a Coordinate*/
    Coordinate* position;

    /** The current age of the Agent as an Integer */
    int age;

    /** Int representing how wide the agents should be rendered as */
    const int AGENT_WIDTH = 2;

    /** Visualizes connection between agent to assignments if true */
    bool visualize;

    /** Rectangle corresponding to this rendered agent */
    QGraphicsRectItem* rect;

    /** Pointer to the agent's home location */
    Location* home;

    /** Line used to visualize assignment to home Location*/
    QGraphicsLineItem* homeLine;

    /** Pointer to the agent's school location, may be nullptr*/
    Location* school;

    /** Line used to visualize assignment to school Location */
    QGraphicsLineItem* schoolLine;

    /** Pointer to the agent's work location, may be nullptr*/
    Location* work;

    /** Line used to visualize assignment to work Location*/
    QGraphicsLineItem* workLine;

    /** Pointer to the agent's leisure location */
    Location* leisure;

    /** Line used to visualize assignment to leisure Location */
    QGraphicsLineItem* leisureLine;

public:

    /**
     * @brief Agent
     * Constructor for the Agent Class
     * @param age: the current age of the Agent
     */
    Agent(int age,  Coordinate position, bool visualize = false);

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
     * @brief setHome: Setter function for the agent's home Location
     * @param home: the home Location the agent is assigned to
     */
    void setHome(Location* home);

    /**
     * @brief getHome: Getter function for the agent's home Location
     * @return the agent's home Location
     */
    Location* getHome();

    /**
     * @brief setSchool: Setter function for the agent's school Location
     * @param school: the school Location the agent is assigned to
     */
    void setSchool(Location* school);

    /**
     * @brief getSchool: Getter function for the agent's school Location
     * @return the agent's school Location
     */
    Location* getSchool();

    /**
     * @brief setWork: Setter function for the agent's work Location
     * @param work: the work Location the agent is assigned to
     */
    void setWork(Location* work);

    /**
     * @brief getWork: Getter function for the agent's work Location
     * @return the agent's work Location
     */
    Location* getWork();

    /**
     * @brief setLeisure: Setter function for the agent's leisure Location
     * @param leisure: the leisure Location the agent is assigned to
     */
    void setLeisure(Location* leisure);

    /**
     * @brief getLeisure: Getter function for the agent's leisure Location
     * @return the agent's leisure location
     */
    Location* getLeisure();

    /**
     * @brief getPosition: Getter function for the position of the agent
     * @return the position of the agent as a Coordinate
     */
    Coordinate* getPosition();

    /**
     * @brief getVisualize: Getter function for whether to visualize the agent's
     * assignments to other points
     * @return this->visualize
     */
    bool getVisualize();

    /**
     * @brief initVisualize
     * Function to initialize the visualizing lines that visualize to assignment
     * of the agent to its locations; Must be called after the agent is
     * assigned to its locations
     */
    void initVisualize();

    /**
     * @brief renderVisualize
     * Function that returns the visualization lines that need to be rendered
     * to the screen
     * @return a vector of QGraphicsItem* that contains the four lines that
     * visualize assignment to locations
     */
    std::vector<QGraphicsItem*> renderVisualize();

    /**
     * @brief ~Agent
     * Destructor to free memory from the Agent class
     */
    ~Agent();

};

#endif // AGENT_H
