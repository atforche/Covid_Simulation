#include "Headers/EconomicController.h"


EconomicController::EconomicController(Simulation* sim) {
    this->sim = sim;
    agentValue = 0;
    businessValue = 0;
}


//******************************************************************************


int* EconomicController::executeEconomicUpdate() {
    static int counter = 0;

    // Trigger the update every hour
    if (counter == 0) {

        // Update each Location in the Simulation
        std::vector<Location*> workLocations = sim->getRegion(Agent::WORK)->getLocations();
        businessValue = 0;
        for (size_t i = 0; i < workLocations.size(); ++i) {
            EconomicLocation* workLocation = dynamic_cast<EconomicLocation*>(workLocations[i]);

            // Have each business pay an overhead during the day
            int currentHour = sim->getHour();
            if (currentHour > 7 && currentHour < 20) {
                workLocation->incrementValue(-1 * workOverhead);
            }
            businessValue += workLocation->getValue();
        }

        // Grab the set of Agents from the Simulation
        std::vector<Agent*> agents = sim->getAgents();
        agentValue = 0;

        for (size_t i = 0; i < agents.size(); ++i) {
            // Cast the the Agent and its Location to Economic variants
            EconomicAgent* agent = dynamic_cast<EconomicAgent*>(agents[i]);
            EconomicLocation* currentLocation = dynamic_cast<EconomicLocation*>(agent->getCurrentLocation());

            int type = currentLocation->getType();
            if (type == Agent::HOME) {

                // Only adults lose value at home, children are free-loaders
                if (agent->isAdult() && agent->getValue() > currentLocation->getCost()) {
                    agent->decrementValue(currentLocation->getCost());

                    // A certain proportion of the agent's value gets distributed to a random business
                    int numBusinesses = sim->getRegion(Agent::WORK)->getNumLocations();
                    int randomIndex = rand() % numBusinesses;
                    EconomicLocation* location = dynamic_cast<EconomicLocation*>(sim->getRegion(Agent::WORK)->getLocations().at(randomIndex));
                    location->incrementValue(homeLossProportion * currentLocation->getCost());
                    businessValue += currentLocation->getCost();
                }

            } else if (type == Agent::SCHOOL) {

                // School locations simply give value to children because why not
                agent->incrementValue(currentLocation->getCost());

            } else if (type == Agent::WORK) {

                // Agents gain value from being at work
                if (currentLocation->getValue() > currentLocation->getCost()) {
                    agent->incrementValue(currentLocation->getCost());

                    // A portion of value gained by agents is lost from the business
                    currentLocation->incrementValue(-1 * workLossProportion * currentLocation->getCost());
                    businessValue = std::max(0.0, businessValue - (workLossProportion * currentLocation->getCost()));
                }

            } else if (type == Agent::LEISURE) {

                // Agents lose value from being at the leisure locations
                if (agent->getValue() > currentLocation->getCost()) {
                    agent->decrementValue(currentLocation->getCost());

                    // The value lost by agents is given back to the business
                    EconomicLocation* sibling = dynamic_cast<EconomicLocation*>(currentLocation->getSibling());
                    sibling->incrementValue(currentLocation->getCost());
                    businessValue += currentLocation->getCost();
                }
            }

            // If an adult runs out of money, they become homeless

            // If there are no more adults with money in a home,
            // all children become homeless and the house is vacant

            // If a business runs out of money, all of it's workers become
            // unemployed and the business disappears

            // All agents assigned to the sibling leisure location are reassigned

            // Unemployed agents have a random chance of becoming employed each day

            // Add the agent's value to the total
            agentValue += agent->getValue();
        }
    }

    // Increment and reset the counter if necessary
    counter++;
    if (counter == Simulation::FRAMES_PER_HOUR) {
        counter = 0;
    }

    // Return the total value in every agent
    int* returnVal = new int[] {agentValue, businessValue};
    return returnVal;
}


//******************************************************************************
