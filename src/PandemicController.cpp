#include "Headers/PandemicController.h"


PandemicController::PandemicController(Simulation* sim) :
    AgentController(sim) {

    // Initialize values for the controller
    this->sim = dynamic_cast<PandemicSimulation*>(sim);

    // Initialize all counts to zero
    newDailyDeaths = 0;
    newDailyCases = 0;
    totalDeaths = 0;
    totalCases = 0;
    numSusceptible = 0;
    numExposed = 0;
    numInfected = 0;
    numRecovered = 0;

    initialInfection = false;
    last24Exposed = std::deque<int>(24, 0);
    last24Deaths = std::deque<int>(24, 0);

    // Check if Agent Compliance will have any effect on infection spread
    checkCompliance = (sim->checkDebug("weak non-compliance") ||
                       sim->checkDebug("moderate non-compliance") ||
                       sim->checkDebug("strong non-compliance"));

}


//******************************************************************************


void PandemicController::updateAgentDestinations(std::vector<Agent *> &agents, int hour) {

    // Initialize the needed fields for the Pandemic Update
    initializePandemicUpdate(agents);

    // Loop through every agent
    for (int i = static_cast<int>(pandemicAgents.size()) - 1; i >= 0; --i) {

        // Update the Agent's destination if necessary
        updateSingleDestination(pandemicAgents[i], hour, true);

        // Update the Pandemic status of the Agent
        bool died = agentPandemicUpdate(pandemicAgents[i], static_cast<int>(i));
        if (died) {
            pandemicAgents[i] = nullptr;
        }
    }

    // Give the infection a chance to reappear in the population
    spontaneousInfection();

    // SPREAD THE INFECTION TO NEW AGENTS
    spreadInfection(pandemicAgents);
}


//******************************************************************************


void PandemicController::updateSingleDestination(Agent* agent, int hour, bool) {

    // Cast the agent to a PandemicAgent
    PandemicAgent* castAgent = dynamic_cast<PandemicAgent*>(agent);
    if (castAgent == nullptr) {return;}

    // Get the important locations for the agent
    Location* home = castAgent->getLocation(Agent::HOME);
    if (home == nullptr) {
        home = sim->getHomelessShelter();
    }

    // If a total lockdown is in effect, immediately send all agents home
    if (sim->checkDebug("total lockdown")) {
        agent->setDestination(*home, "Home");
        return;
    }

    // If the Agent has been assigned to a new location, enforce the Pandemic
    // rules
    QString destinationString = getAgentDestination(agent, hour);
    if (destinationString != "No Change") {

        // Calculate whether the Agent will comply with self-enforced measures
        bool compliance = willComply();

        // Run the base destination update (allow agent's to go to random locations if they comply and there's not strong guidelines)
        bool randomAllowed = true;
        if (compliance && (sim->checkDebug("moderate guidelines") || sim->checkDebug("strong guidelines") ||
                           sim->checkDebug("total lockdown"))) {
            randomAllowed = false;
        }
        AgentController::updateSingleDestination(agent, hour, randomAllowed);

        // Enforce Lockdowns. Even non-compliant agents must follow lockdowns
        enforceLockdowns(castAgent, home);

        // Update the Agent's compliance status
        if (!compliance) {
            castAgent->setCompliance(false);
            return;
        }

        // Update the Agent's compliance status
        castAgent->setCompliance(true);

        // Enable an Agent to go to Work, School, or Leisure from Home. If an
        // agent can work from home, they can get around Quarantine and Contact Tracing
        bool workingFromHome = applyECommerce(castAgent, home);
        if (!workingFromHome) {
            // Enforce the Quarantine when Infected Flag and update the currentLocation
            enforceQuarantine(castAgent, home);

            // Enforce the Contact Tracing Flag and update the currentLocation
            enforceContactTracing(castAgent, home);

            // Enfore the Government guidelines
            enforceGuidelines(castAgent, home);
        }
    }
}


//******************************************************************************


void PandemicController::initializePandemicUpdate(std::vector<Agent*> &agents) {

    // Reset the counts
    numSusceptible = 0;
    numExposed = 0;
    numInfected = 0;
    numHomelessInfected = 0;
    numRecovered = 0;

    // Check if the initial infection needs to occur
    if (!initialInfection) {
        if (sim->getDay() >= sim->getLagPeriod()) {
            beginInfection();
            initialInfection = true;
        }
    }

    // Update every Location if Lockdowns are enforced
    lockdownLocations();

    // Acquire the lock on the Agents vector
    QMutexLocker agentLock(sim->getAgentsLock());

    // Loop through every agent, store the cast pointers, and reset the number
    // of nearby infected agents
    pandemicAgents = std::vector<PandemicAgent*>(agents.size(), nullptr);
    for (size_t i = 0; i < agents.size(); ++i) {
        PandemicAgent* agent = dynamic_cast<PandemicAgent*>(agents[i]);
        if (agent == nullptr) {
            pandemicAgents[i] = nullptr;
        } else {
            agent->resetNearbyInfected();
            pandemicAgents[i] = agent;

            // Count how many agents are in each state of infection
            if (agent->getStatus() == PandemicAgent::SUSCEPTIBLE) {
                numSusceptible++;
            } else if (agent->getStatus() == PandemicAgent::EXPOSED) {
                numExposed++;
                incrementNearbyInfected(pandemicAgents, i);
            } else if (agent->getStatus() == PandemicAgent::INFECTED) {
                numInfected++;
                if (agent->getLocation(Agent::HOME) == nullptr) {
                    numHomelessInfected++;
                }
            } else {
                numRecovered++;
            }
        }
    }
}


//******************************************************************************


bool PandemicController::agentPandemicUpdate(PandemicAgent* agent, int i) {
    // Update the count of each Pandemic Status
    bool died = false;
    if (agent->getStatus() == PandemicAgent::EXPOSED) {
        incrementNearbyInfected(pandemicAgents, i);
    } else if (agent->getStatus() == PandemicAgent::INFECTED) {
        died = agent->evaluateDeathProbability(numInfected, sim->getUI()->hospitalCapacity->value());
        if (died) {
            sim->killAgent(agent, i);
            pandemicAgents[i] = nullptr; // Mark the agent as dead in the pandemicAgents vectors
            newDailyDeaths++;
            totalDeaths++;
        } else {
            incrementNearbyInfected(pandemicAgents, i);
        }
    }
    return died;
}


//******************************************************************************


void PandemicController::spreadInfection(std::vector<PandemicAgent*> &pandemicAgents) {

    // Track the number of new infections each hour
    int newExposures = 0;

    bool guidelines = (sim->checkDebug("moderate guidelines") ||
                       sim->checkDebug("strong guidelines"));
    // Loop through every agent to spread the Infection
    for (size_t i = 0; i < pandemicAgents.size(); ++i) {
        if (pandemicAgents[i] == nullptr) {continue;}
        bool newlyExposed = pandemicAgents[i]->evaluateInfectionProbability(checkCompliance, guidelines);
        if (newlyExposed) {
            numExposed++;
            newExposures++;
            numSusceptible--;
        }
    }

    // Update the attributes for the Simulation state
    last24Exposed.pop_front();
    last24Exposed.push_back(newExposures);
}


//******************************************************************************


void PandemicController::spontaneousInfection() {
    if (numExposed == 0 && numInfected == 0 && numSusceptible > 0) {
        if (rand() % 600 == 0) {
            initialInfection = false;
        }
    }
}


//******************************************************************************


void PandemicController::advanceDay() {
    this->newDailyCases = 0;
    this->newDailyDeaths = 0;

    // Advance the infection for each Agent
    std::vector<Agent*> agents = sim->getAgents();
    for (size_t i = 0; i < agents.size(); ++i) {
        PandemicAgent* agent = dynamic_cast<PandemicAgent*>(agents[i]);
        bool newCase = agent->advanceDay();
        if (newCase) {
            newDailyCases++;
            totalCases++;
        }
    }
}


//******************************************************************************


int PandemicController::getTotalCases() {
    return this->totalCases;
}


//******************************************************************************


int PandemicController::getDailyCases() {
    return this->newDailyCases;
}


//******************************************************************************


int PandemicController::getTotalDeaths() {
    return this->totalDeaths;
}


//******************************************************************************


int PandemicController::getDailyDeaths() {
    return this->newDailyDeaths;
}


//******************************************************************************


std::vector<int> PandemicController::getSEIR() {
    std::vector<int> retVal = {numSusceptible, numExposed, numInfected, numRecovered};
    return retVal;
}


//******************************************************************************


void PandemicController::beginInfection() {

    // Initialize variables
    int numToInfect = sim->getInitialInfected();
    std::vector<Agent*> agents = sim->getAgents();

    for (int i = 0; i < numToInfect; ++i) {
        int randIndex = rand() % agents.size();
        PandemicAgent* agent = dynamic_cast<PandemicAgent*>(agents[randIndex]);
        agent->makeExposed();
    }
}


//******************************************************************************


void PandemicController::enforceLockdowns(PandemicAgent* agent, Location* home) {
    PandemicLocation* currentLocation = dynamic_cast<PandemicLocation*>(agent->getCurrentLocation());

    // Homeless and Unemployed agents are unaffected by lockdowns
    if (currentLocation == nullptr) {
        return;
    }

    // If there's a total lockdown, immediately send all agents home
    if (sim->checkDebug("total lockdown")) {
        agent->setDestination(*home, "Home");
        return;
    }

    if (currentLocation->getStatus() == PandemicLocation::LOCKDOWN) {
        if (currentLocation->getType() == Agent::SCHOOL ||
                currentLocation->getType() == Agent::WORK) {
            agent->setDestination(*home, "Home");
        } else {
            // If it's a leisure location thats locked down, then the Agent will try a second one before going home
            PandemicLocation* newOption = dynamic_cast<PandemicLocation*>(sim->getRandomLocation(Agent::LEISURE));
            if (newOption->getStatus() != PandemicLocation::LOCKDOWN) {
                agent->setDestination(*newOption, "Leisure");
            } else {
                agent->setDestination(*home, "Home");
            }
        }
    }
}


//******************************************************************************


void PandemicController::enforceQuarantine(PandemicAgent* agent, Location* home) {
    if (sim->checkDebug("quarantine when infected")) {
        if (agent->getStatus() == PandemicAgent::INFECTED) {
            agent->setDestination(*home, "Home");
        }
    }
}


//******************************************************************************


void PandemicController::enforceContactTracing(PandemicAgent* agent, Location* home_in) {

    PandemicLocation* home = dynamic_cast<PandemicLocation*>(agent->getLocation(Agent::HOME));
    PandemicLocation* work = dynamic_cast<PandemicLocation*>(agent->getLocation(Agent::WORK));
    PandemicLocation* school = dynamic_cast<PandemicLocation*>(agent->getLocation(Agent::SCHOOL));
    PandemicLocation* leisure = dynamic_cast<PandemicLocation*>(agent->getLocation(Agent::LEISURE));

    if (sim->checkDebug("weak contact tracing") ||
            sim->checkDebug("moderate contact tracing") ||
            sim->checkDebug("strong contact tracing")) {

        // Determine if the Agent has been exposed at any of their locations
        bool exposed = ((home && home->getNumInfectedAgents() > 0) ||
                        (home == nullptr && numHomelessInfected > 0) ||
                        (work && work->getNumInfectedAgents() > 0) ||
                        (school && school->getNumInfectedAgents() > 0) ||
                        (leisure && leisure->getNumInfectedAgents() > 0));

        // Enforce the probabilities of each option
        if (exposed) {
            if (sim->checkDebug("strong contact tracing")) {
                agent->setDestination(*home_in, "Home");
            } else if (sim->checkDebug("moderate contact tracing")) {
                if (rand() % 4 != 3) {
                    agent->setDestination(*home_in, "Home");
                }
            } else if (sim->checkDebug("weak contact tracing")) {
                if (rand() % 2 == 0) {
                    agent->setDestination(*home_in, "Home");
                }
            }
        }
    }
}


//******************************************************************************


bool PandemicController::willComply() {

    if (sim->checkDebug("strong non-compliance")) {
        return (rand() % 2) != 0; // 50/50 chance to comply
    } else if (sim->checkDebug("moderate non-compliance")) {
        return (rand() % 4) != 0; // 75/25 chance to comply
    } else if (sim->checkDebug("weak non-compliance")) {
        return (rand() % 8) != 0; // 87.5/12.5 chance to comply
    }

    return true;
}


//******************************************************************************


void PandemicController::enforceGuidelines(PandemicAgent *agent, Location *home) {

    // Guidelines only affect Leisure locations
    PandemicLocation* currentLocation = dynamic_cast<PandemicLocation*>(agent->getCurrentLocation());
    if (currentLocation == nullptr || currentLocation->getType() != Agent::LEISURE) {
        return;
    }

    if (sim->checkDebug("strong guidelines")) {
        // Have a 50% chance of going to a Leisure location
        if ((rand() % 2) != 0) {
            agent->setDestination(*home, "Home");
        }
    } else if (sim->checkDebug("moderate guidelines")) {
        // Have a 60% chance of going to a Leisure location
        if ((rand() % 10) < 6) {
            agent->setDestination(*home, "Home");
        }
    } else if (sim->checkDebug("weak guidelines")) {
        // Have a 75% chance of going to a Leisure location
        if ((rand() % 4) == 0) {
            agent->setDestination(*home, "home");
        }
    }
}


//******************************************************************************


bool PandemicController::applyECommerce(PandemicAgent *agent, Location *home) {

    // ECommerce does not apply to Home locations
    PandemicLocation* currentLocation = dynamic_cast<PandemicLocation*>(agent->getCurrentLocation());
    if (currentLocation == nullptr || currentLocation->getType() == Agent::HOME ||
            home == nullptr) {
        return false;
    }

    // Send the Agent home, but make it appear to be at the other Location from an Economic Standpoint
    bool workingFromHome = false;
    if (sim->checkDebug("strong e-commerce")) {
        // Give the Agent a 75% chance each hour to work from Home
        if ((rand() % 100) < 75) {
            agent->setDestination(*home, agent->getDestinationString());
            workingFromHome = true;
        }
    } else if (sim->checkDebug("moderate e-commerce")) {
        // Give the Agent an 50% chance each hour to work from Home
        if ((rand() % 100) < 50) {
            agent->setDestination(*home, agent->getDestinationString());
            workingFromHome = true;
        }
    } else if (sim->checkDebug("weak e-commerce")) {
        // Give the Agent an 25% chance each hour to work from Home
        if ((rand() % 100) < 25) {
            agent->setDestination(*home, agent->getDestinationString());
            workingFromHome = true;
        }
    }

    return workingFromHome;
}


//******************************************************************************


void PandemicController::lockdownLocations() {
    // Update every Location if Lockdowns are enforced
    std::vector<Location*> locations = sim->getAllLocations();
    for (size_t i = 0; i < locations.size(); ++i) {
        // Get the Location and calculate the infected proportion
        PandemicLocation* location = dynamic_cast<PandemicLocation*>(locations[i]);
        double infectedProportion = static_cast<double>(location->getNumInfectedAgents()) / location->getNumAgents();

        // Home locations cannot go on lockdown
        if (location->getType() == Agent::HOME) {
            if (location->getNumInfectedAgents() > 0) {
                location->setStatus(PandemicLocation::EXPOSURE);
            } else {
                location->setStatus(PandemicLocation::NORMAL);
            }
            continue;
        }

        if (sim->checkDebug("total lockdown")) {
            location->setStatus(PandemicLocation::LOCKDOWN);
            continue;
        } else if (sim->checkDebug("strong lockdown")) {
            if (infectedProportion > 0.20) {
                location->setStatus(PandemicLocation::LOCKDOWN);
                continue;
            }
        } else if (sim->checkDebug("moderate lockdown")) {
            if (infectedProportion > 0.35) {
                location->setStatus(PandemicLocation::LOCKDOWN);
                continue;
            }
        } else if (sim->checkDebug("weak lockdown")) {
            if (infectedProportion > 0.75) {
                location->setStatus(PandemicLocation::LOCKDOWN);
                continue;
            }
        }

        if (location->getNumInfectedAgents() > 0) {
            location->setStatus(PandemicLocation::EXPOSURE);
        } else {
            location->setStatus(PandemicLocation::NORMAL);
        }
    }
}


//**************************************************************************


void PandemicController::incrementNearbyInfected(std::vector<PandemicAgent*> &pandemicAgents, size_t index) {

    // Grab the position of the Agent to compare against
    Coordinate comparisonPosition = pandemicAgents[index]->getPosition();
    bool isInfected = pandemicAgents[index]->getStatus() == PandemicAgent::INFECTED;

    for (size_t i = 0; i < pandemicAgents.size(); ++i) {
        if (i == index) {continue;}
        if (pandemicAgents[i] == nullptr) {continue;}

        Coordinate agentPosition = pandemicAgents[i]->getPosition();
        double distance = comparisonPosition.distBetween(agentPosition);

        if (distance < INFECTION_RADIUS) {
            pandemicAgents[i]->incrementNearbyInfected(1 + isInfected);
        }
    }
}


//**************************************************************************


std::vector<PandemicAgent*>& PandemicController::getPandemicAgents() {
    return pandemicAgents;
}


//**************************************************************************


int PandemicController::getNumHomelessInfected() {
    return this->numHomelessInfected;
}


//**************************************************************************


std::vector<double> PandemicController::getState() {
    return std::vector<double> ({static_cast<double>(last24Exposed.back()),
                                 static_cast<double>(std::accumulate(last24Exposed.begin(), last24Exposed.end(), 0.0)),
                                 static_cast<double>(last24Deaths.back()),
                                 static_cast<double>(std::accumulate(last24Deaths.begin(), last24Deaths.end(), 0.0)),
                                 static_cast<double>(numInfected) / sim->getCurrentNumAgents() * 100,
                                 static_cast<double>(numExposed) / sim->getCurrentNumAgents() * 100,
                                 static_cast<double>(numRecovered) / sim->getCurrentNumAgents() * 100,
                                 static_cast<double>(numSusceptible) / sim->getCurrentNumAgents() * 100,
                                 static_cast<double>(numInfected > sim->getUI()->hospitalCapacity->value() * 100)});
}


//**************************************************************************


void PandemicController::killAgents(int numDeaths) {
    // Update the Simulations tate
    last24Deaths.pop_front();
    last24Deaths.push_back(numDeaths);
}

//**************************************************************************
