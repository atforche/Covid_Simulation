#include "Headers/EconomicController.h"


EconomicController::EconomicController(Simulation* sim) :
    AgentController(sim) {
    this->sim = dynamic_cast<EconomicSimulation*>(sim);
    agentValue = 0;
    businessValue = 0;
}


//******************************************************************************


void EconomicController::updateAgentDestinations(std::vector<Agent *> &agents, int hour) {

    // BUSINESS UPDATES ********************************************************
    // Reset the business and agent values
    businessValue = 0;
    agentValue = 0;
    double redistributedValue = 0; // Amount of value agents lose at home to distribute equally among businesses

    // Update each Location in the Simulation
    std::vector<Location*> workLocations = sim->getRegion(Agent::WORK)->getLocations();
    for (int i = static_cast<int>(workLocations.size()) - 1; i >= 0; --i) {
        EconomicLocation* workLocation = dynamic_cast<EconomicLocation*>(workLocations[i]);

        // Reset the Locations counter for the current day
        workLocation->startNewDay();

        // Have each business pay an overhead during the day
        int currentHour = sim->getHour();
        if (currentHour > 7 && currentHour < 20) {
            workLocation->incrementValue(-1 * workOverhead);
        }

        // Add the businesses value to the total
        businessValue += workLocation->getValue();

        // If a business runs out of money, it becomes bankrupt
        if (workLocation->getValue() == 0) {
            bankruptBusiness(workLocation);
        }
    }

    // AGENT UPDATES ***********************************************************
    // Loop through every agent
    for (size_t i = 0; i < agents.size(); ++i) {

        // Update each Agents location according to their Behavior Chart
        updateSingleDestination(agents[i], hour, true);

        // Cast the the Agent and its Location to Economic variants
        EconomicAgent* agent = dynamic_cast<EconomicAgent*>(agents[i]);
        QString currentDestinaton = agent->getDestinationString();

        // Update the Agent according to its current location
        if (currentDestinaton == "Home") {
            redistributedValue += homeEconomicUpdate(agent);
        } else if (currentDestinaton == "School") {
            schoolEconomicUpdate(agent);
        } else if (currentDestinaton == "Work") {
            workEconomicUpdate(agent);
        } else if (currentDestinaton == "Leisure") {
            leisureEconomicUpdate(agent);
        }

        // Add the agent's value to the total
        agentValue += agent->getValue();
    }

    // CLEAN UP ****************************************************************
    // If there are less than the initial amount of businesses, have a chance
    // to generate a new business and leisure location
    workLocations = sim->getRegion(Agent::WORK)->getLocations();
    if (static_cast<int>(workLocations.size()) < sim->getUI()->numLocations->value()) {
        if (rand() % 5 == 0) {
            generateNewBusiness();
        }
    }

    // Distribute the value lost from agents at home equally to each business
    std::vector<Location*> locations = sim->getRegion(Agent::WORK)->getLocations();
    businessValue += redistributedValue;
    for (size_t i = 0; i < locations.size(); ++i) {
        EconomicLocation* location = dynamic_cast<EconomicLocation*>(locations[i]);
        location->incrementValue(redistributedValue / locations.size());
    }

}


//******************************************************************************


void EconomicController::bankruptBusiness(EconomicLocation *victim) {

    // Businesses can't go bankrupt on Day 1
    if (sim->getDay() == 0) {
        return;
    }

    // Acquire the lock to provide mutual exclusion
    QMutexLocker locationLock(sim->getLocationLock());

    // Grab the agents from the Business and Leisure location
    std::unordered_set<Agent*> employees = victim->getAgents();
    std::unordered_set<Agent*> customers = victim->getSibling()->getAgents();

    // Mark each of the employee agents as unemployeed
    for (auto it = employees.begin(); it != employees.end(); ++it) {
        EconomicAgent* agent = static_cast<EconomicAgent*>(*it);
        makeUnemployed(agent);
    }

    // Remove the Leisure location
    sim->getRegion(Agent::LEISURE)->removeLocation(victim->getSibling());

    // Remove the Business location
    sim->getRegion(Agent::WORK)->removeLocation(victim);

    // Reassign each customer to a new leisure location
    for (auto it = customers.begin(); it != customers.end(); ++it) {
        Location* newLocation = sim->getRandomLocation(Agent::LEISURE);
        (*it)->setLocation(newLocation, Agent::LEISURE);
        if (newLocation != nullptr) {
            (*it)->getLocation(Agent::LEISURE)->addAgent(*it);
        }
    }
}


//******************************************************************************


void EconomicController::makeHomeless(EconomicAgent *agent) {
    // Agent's can't become homeless on Day 1
    if (sim->getDay() == 0) {
        return;
    }

    // Updates the Agent's status and location
    if (agent->getStatus() == EconomicAgent::UNEMPLOYED) {
        agent->setStatus(EconomicAgent::BOTH);
    } else {
        agent->setStatus(EconomicAgent::HOMELESS);
    }
    agent->setLocation(nullptr, Agent::HOME);
}


//******************************************************************************


void EconomicController::makeUnemployed(EconomicAgent *agent) {

    // Agent's can't become unemployed on Day 1
    if (sim->getDay() == 0) {
        return;
    }

    if (agent->getStatus() == EconomicAgent::HOMELESS) {
        agent->setStatus(EconomicAgent::BOTH);
    } else {
        agent->setStatus(EconomicAgent::UNEMPLOYED);
    }
    agent->setLocation(nullptr, Agent::WORK);
}


//******************************************************************************


double EconomicController::homeEconomicUpdate(EconomicAgent *agent) {

    // Handle agents that are homeless
    EconomicAgent::STATUS status = agent->getStatus();
    if (status == EconomicAgent::HOMELESS || status == EconomicAgent::BOTH) {

        // Given Homeless agents a small chance to get a home back if they can afford it
        if (rand() % 100 < 10) {
            EconomicLocation* potentialHome = dynamic_cast<EconomicLocation*>(sim->getRandomLocation(Agent::HOME));
            if (agent->getValue() > 2 * potentialHome->getCost()) {
                agent->setLocation(potentialHome, Agent::HOME);
                potentialHome->addAgent(agent);

                // Update the Agent's status as appriopriate
                if (status == EconomicAgent::BOTH) {
                    agent->setStatus(EconomicAgent::UNEMPLOYED);
                } else {
                    agent->setStatus(EconomicAgent::NORMAL);
                }
            }
        }
        return 0;
    }

    // Grab the current Location the agent is at
    EconomicLocation* currentLocation = dynamic_cast<EconomicLocation*>(agent->getCurrentLocation());

    // Only adults lose value at home, children are free-loaders
    if (agent->isAdult() && agent->getValue() >= currentLocation->getCost()) {
        agent->decrementValue(currentLocation->getCost());

        // Distribute a portion of the agent's lost value to each business location
        return currentLocation->getCost() * homeLossProportion;

    } else if (agent->isAdult()) {
        // If the agent is an adult without enough money to pay, they become homeless
        // Children will never become homeless (simplifying assumption)
        makeHomeless(agent);
    }

    return 0;
}


//******************************************************************************


void EconomicController::schoolEconomicUpdate(EconomicAgent *agent) {

    // School locations simply give value to children
    EconomicLocation* currentLocation = dynamic_cast<EconomicLocation*>(agent->getCurrentLocation());
    agent->incrementValue(currentLocation->getCost());
}


//******************************************************************************


void EconomicController::workEconomicUpdate(EconomicAgent *agent) {

    // Handle agents that are unemployed
    EconomicAgent::STATUS status = agent->getStatus();
    if (status == EconomicAgent::UNEMPLOYED || status == EconomicAgent::BOTH) {
        // Give agent a chance to gain a small amount of value
        if (rand() % 100 < 20) {
            agent->incrementValue(rand() % 5 + 1);
        }

        // Give agent a small chance of finding a new job
        EconomicLocation* newPotentialJob = dynamic_cast<EconomicLocation*>(sim->getRandomLocation(Agent::WORK));
        bool canHire = newPotentialJob->makeHire();

        // If the location can hire an agent, give them a 50% chance of getting the job
        if (canHire && rand() % 2 == 0) {
            agent->setLocation(newPotentialJob, Agent::WORK);
            newPotentialJob->addAgent(agent);

            // Update the status of the Agent
            if (agent->getStatus() == EconomicAgent::BOTH) {
                agent->setStatus(EconomicAgent::HOMELESS);
            } else {
                agent->setStatus(EconomicAgent::NORMAL);
            }
        }

        // Agent will not have a chance to get value until the next hour
        return;
    }

    // Grab the current Location the agent is at
    EconomicLocation* currentLocation = dynamic_cast<EconomicLocation*>(agent->getCurrentLocation());

    // Agents gain value from being at work
    if (currentLocation->getValue() > currentLocation->getCost()) {
        agent->incrementValue(currentLocation->getCost());

        // A portion of value gained by agents is lost from the business
        currentLocation->incrementValue(-1 * workLossProportion * currentLocation->getCost());
        businessValue = std::max(0.0, businessValue - (workLossProportion * currentLocation->getCost()));
    } else {

        // If the business is unable to pay it's workers, it goes bankrupt
        bankruptBusiness(currentLocation);

    }
}


//******************************************************************************


void EconomicController::leisureEconomicUpdate(EconomicAgent *agent) {

    // Need to be extra careful getting location
    EconomicLocation* currentLocation = nullptr;
    Location* simpleLocation = agent->getCurrentLocation();
    if (simpleLocation == nullptr) {
        return;
    } else {
        currentLocation = dynamic_cast<EconomicLocation*>(simpleLocation);
        if (currentLocation == nullptr) {
            return;
        }
    }

    // Homeless agents spend less money at leisure locations
    if (agent->getStatus() == EconomicAgent::BOTH || agent->getStatus() == EconomicAgent::HOMELESS) {
        if (rand() % 2 == 0) {
            agent->decrementValue(1);

            // The value lost by agents is given back to the business
            EconomicLocation* sibling = dynamic_cast<EconomicLocation*>(currentLocation->getSibling());
            sibling->incrementValue(currentLocation->getCost());
            businessValue += currentLocation->getCost();
        }
        return;
    }

    // Agents lose value from being at the leisure locations
    if (agent->getValue() > currentLocation->getCost()) {
        agent->decrementValue(currentLocation->getCost());

        // The value lost by agents is given back to the business
        EconomicLocation* sibling = dynamic_cast<EconomicLocation*>(currentLocation->getSibling());
        sibling->incrementValue(currentLocation->getCost());
        businessValue += currentLocation->getCost();
    }
}


//******************************************************************************


void EconomicController::generateNewBusiness() {

    // Generate a new work and leisure location
    sim->getRegion(Agent::WORK)->generateLocations(1, "Economic");
    sim->getRegion(Agent::LEISURE)->generateLocations(1, "Economic");

    // Grab each new location
    EconomicLocation* newWork = dynamic_cast<EconomicLocation*>(sim->getRegion(Agent::WORK)->getLocations().back());
    EconomicLocation* newLeisure = dynamic_cast<EconomicLocation*>(sim->getRegion(Agent::LEISURE)->getLocations().back());

    // Establish a sibling relationship between agents
    newWork->setSibling(newLeisure);
    newLeisure->setSibling(newWork);

    // Render the new locations to the screen
    sim->addToScreen(newWork->getGraphicsObject());
    sim->addToScreen(newLeisure->getGraphicsObject());

    // Give the new work location some amount of value
    newWork->incrementValue(rand() % 200 + 500);

    // Grab the agents in the Simulation
    std::vector<Agent*> agents = sim->getAgents();

    // Assign agents to the new Work Location
    int unemployedAgents = sim->getNumUnemployedAgents();

    // If there are unemployed agents, assign up to five of them to work at
    // the new location
    if (unemployedAgents > 0) {
        int newAgents = std::min(5, unemployedAgents);
        std::vector<Agent*> unemployedAgents = sim->getUnemployedAgents();
        for (int i = 0; i < newAgents; ++i) {
            EconomicAgent* agent = dynamic_cast<EconomicAgent*>(unemployedAgents[i]);
            agent->setLocation(newWork, Agent::WORK);
            newWork->addAgent(agent);

            // Update the status of the agent
            if (agent->getStatus() == EconomicAgent::BOTH) {
                agent->setStatus(EconomicAgent::HOMELESS);
            } else {
                agent->setStatus(EconomicAgent::NORMAL);
            }
        }

    } else {
        // Otherwise, select a small number of employed agents to work there
        int newAgents = std::min(3, static_cast<int>(agents.size()));
        for (int i = 0; i < newAgents; ++i) {
            EconomicAgent* agent = dynamic_cast<EconomicAgent*>(agents[rand() % agents.size()]);
            agent->setLocation(newWork, Agent::WORK);
            newWork->addAgent(agent);
        }
    }


    // Assign agents to the new Leisure location
    int newAgents = std::min(rand() % 5 + 3, static_cast<int>(agents.size()));
    for (int i = 0; i < newAgents; ++i) {
        EconomicAgent* agent = dynamic_cast<EconomicAgent*>(agents[rand() % agents.size()]);
        agent->setLocation(newLeisure, Agent::LEISURE);
        newLeisure->addAgent(agent);
    }
}


//******************************************************************************


int EconomicController::getTotalAgentValue() {
    return this->agentValue;
}


//******************************************************************************


int EconomicController::getTotalBusinessValue() {
    return this->businessValue;
}


//******************************************************************************
