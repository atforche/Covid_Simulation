#ifndef AGENTGENERATOR_H
#define AGENTGENERATOR_H

#include "Region.h"
#include <random>
#include <vector>
#include "Agent.h"

/**
 * @brief The AgentGenerator class
 * Class that handles randomly generating agents within each region and
 * assigning them to different home, work, school, and leisure locations
 */

class AgentGenerator {

private:

    /** Pointer to the home region */
    Region* home;

    /** Pointer to the school region */
    Region* school;

    /** Pointer to the work region */
    Region* work;

    /** Pointer to the leisure region */
    Region* leisure;

public:

    /**
     * @brief AgentGenerator
     * Constructor for the Agent Generator Class. This class handles generating
     * agents for a Simulation, assigning each agent to one location in each
     * Region, and giving the Agent a starting position (currently always at
     * home)
     * @param home: a pointer to the home Region in the simulation
     * @param school: a pointer to the school Region in the simulation
     * @param work: a pointer to the work Region in the simulation
     * @param leisure: a pointer to the leisure Region in the simulation
     */
    AgentGenerator(Region* home, Region* school, Region* work, Region* leisure);

    /**
     * @brief generate
     * Function to generator a number of Agents and randomly assign them to
     * different locations in each region. A total of num agents are generated.
     * Each agent is randomly assigned to a single location within each region.
     * The agent is given a random starting position that is centered around
     * its home location. Returns a vector of generated agents to be added to
     * the simulation.
     * @param num: the number of agents to be generated
     * @param visualize: visualize connections between agents to assignments if
     * true
     * @return a vector of pointers pointing to the generated agents
     */
    std::vector<Agent*> generate(int num, bool visualize = false);

};

#endif // AGENTGENERATOR_H
