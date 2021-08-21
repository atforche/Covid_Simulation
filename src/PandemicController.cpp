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

}


//******************************************************************************


void PandemicController::updateAgentDestinations(std::vector<Agent *> &agents, int hour) {

    // Reset the counts
    numSusceptible = 0;
    numExposed = 0;
    numInfected = 0;
    numRecovered = 0;

    // Check if the initial infection needs to occur
    if (!initialInfection) {
        if (sim->getDay() == sim->getLagPeriod()) {
            beginInfection();
            initialInfection = true;
        }
    }

    // Check if Agent Compliance will have any effect on infection spread
    bool checkCompliance = (sim->checkDebug("weak non-compliance") ||
                            sim->checkDebug("moderate non-compliance") ||
                            sim->checkDebug("strong non-compliance"));

    // Update every Location if Lockdowns are enforced
    if (sim->checkDebug("weak lockdown") ||
            sim->checkDebug("moderate lockdown") ||
            sim->checkDebug("strong lockdown") ||
            sim->checkDebug("total lockdown")) {
        lockdownLocations();
    }

    // Acquire the lock on the Agents vector
    QMutexLocker agentLock(sim->getAgentsLock());

    // Loop through every agent, store the cast pointers, and reset the number
    // of nearby infected agents
    std::vector<PandemicAgent*> pandemicAgents(agents.size(), nullptr);
    for (size_t i = 0; i < agents.size(); ++i) {
        PandemicAgent* agent = dynamic_cast<PandemicAgent*>(agents[i]);
        agent->resetNearbyInfected();
        pandemicAgents[i] = agent;
    }

    // Loop through every agent
    for (size_t i = 0; i < pandemicAgents.size(); ++i) {

        // Update the Agent's destination if necessary
        updateSingleDestination(pandemicAgents[i], hour, true);

        // Update the count of each Pandemic Status
        PandemicAgent* agent = pandemicAgents[i];
        if (agent->getStatus() == PandemicAgent::SUSCEPTIBLE) {
            numSusceptible++;
        } else if (agent->getStatus() == PandemicAgent::EXPOSED) {
            numExposed++;
            incrementNearbyInfected(pandemicAgents, i);
        } else if (agent->getStatus() == PandemicAgent::INFECTED) {
            numInfected++;
            bool died = agent->evaluateDeathProbability();
            if (died) {
                sim->killAgent(agent, static_cast<int>(i));
                newDailyDeaths++;
                totalDeaths++;
            } else {
                incrementNearbyInfected(pandemicAgents, i);
            }
        } else {
            numRecovered++;
        }
    }

    // Loop through every agent to spread the Infection
    for (size_t i = 0; i < pandemicAgents.size(); ++i) {
        bool newlyExposed = pandemicAgents[i]->evaluateInfectionProbability(checkCompliance);
        if (newlyExposed) {
            numExposed++;
            numSusceptible--;
        }
    }

}


//******************************************************************************


void PandemicController::updateSingleDestination(Agent* agent, int hour, bool) {

    // Run the base destination update (give agents a 1% chance to move to random locations)
    AgentController::updateSingleDestination(agent, hour, rand() % 100 == 0);

    // If the Agent has been assigned to a new location, enforce the Pandemic
    // rules
    QString destinationString = getAgentDestination(agent, hour);
    if (destinationString != "No Change") {
        PandemicAgent* castAgent = dynamic_cast<PandemicAgent*>(agent);
        PandemicLocation* home = dynamic_cast<PandemicLocation*>(castAgent->getLocation(Agent::HOME));
        PandemicLocation* work = dynamic_cast<PandemicLocation*>(castAgent->getLocation(Agent::WORK));
        PandemicLocation* school = dynamic_cast<PandemicLocation*>(castAgent->getLocation(Agent::SCHOOL));
        PandemicLocation* leisure = dynamic_cast<PandemicLocation*>(castAgent->getLocation(Agent::LEISURE));

        // Enforce Lockdowns. Even non-compliant agents must follow lockdowns
        enforceLockdowns(castAgent, home);

        // Calculate whether the Agent will comply with self-enforced measures
        if (!willComply()) {
            castAgent->setCompliance(false);
            return;
        }

        // Update the Agent's compliance status
        castAgent->setCompliance(true);

        // Enforce the Quarantine when Infected Flag and update the currentLocation
        enforceQuarantine(castAgent, home);

        // Enable an Agent to go to Work, School, or Leisure from Home
        applyECommerce(castAgent, home);

        // Enforce the Contact Tracing Flag and update the currentLocation
        enforceContactTracing(castAgent, home, school, work, leisure);

        // Enfore the Government guidelines
        enforceGuidelines(castAgent, home);
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


void PandemicController::enforceLockdowns(PandemicAgent* agent, PandemicLocation* home) {
    PandemicLocation* currentLocation = dynamic_cast<PandemicLocation*>(agent->getCurrentLocation());
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


void PandemicController::enforceQuarantine(PandemicAgent* agent, PandemicLocation* home) {
    if (sim->checkDebug("quarantine when infected")) {
        if (agent->getStatus() == PandemicAgent::INFECTED) {
            agent->setDestination(*home, "Home");
        }
    }
}


//******************************************************************************


void PandemicController::enforceContactTracing(PandemicAgent* agent, PandemicLocation* home, PandemicLocation* school,
                                               PandemicLocation* work, PandemicLocation* leisure) {

    if (sim->checkDebug("weak contact tracing") ||
            sim->checkDebug("moderate contact tracing") ||
            sim->checkDebug("strong contact tracing")) {

        // Determine if the Agent has been exposed at any of their locations
        bool exposed = ((home && home->getNumInfectedAgents() > 0) ||
                        (work && work->getNumInfectedAgents() > 0) ||
                        (school && school->getNumInfectedAgents() > 0) ||
                        (leisure && leisure->getNumInfectedAgents() > 0));

        // Enforce the probabilities of each option
        if (exposed) {
            if (sim->checkDebug("strong contact tracing")) {
                agent->setDestination(*home, "Home");
            } else if (sim->checkDebug("moderate contact tracing")) {
                if (rand() % 4 != 3) {
                    agent->setDestination(*home, "Home");
                }
            } else if (sim->checkDebug("weak contact tracing")) {
                if (rand() % 2 == 0) {
                    agent->setDestination(*home, "Home");
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


void PandemicController::enforceGuidelines(PandemicAgent *agent, PandemicLocation *home) {

    // Guidelines only affect Leisure locations
    PandemicLocation* currentLocation = dynamic_cast<PandemicLocation*>(agent->getCurrentLocation());
    if (currentLocation->getType() != Agent::LEISURE) {
        return;
    }

    if (sim->checkDebug("strong guidelines")) {
        // Have a 25% chance of going to a Leisure location
        if ((rand() % 4) != 0) {
            agent->setDestination(*home, "Home");
        }
    } else if (sim->checkDebug("moderate guidelines")) {
        // Have a 50% chance of going to a Leisure location
        if ((rand() % 2) != 0) {
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


void PandemicController::applyECommerce(PandemicAgent *agent, PandemicLocation *home) {

    // ECommerce does not apply to Home locations
    PandemicLocation* currentLocation = dynamic_cast<PandemicLocation*>(agent->getCurrentLocation());
    if (currentLocation->getType() == Agent::HOME) {
        return;
    }

    if (sim->checkDebug("strong e-commerce")) {
        // Give the Agent a 15% chance each hour to work from Home
        if ((rand() % 100) < 15) {
            agent->setDestination(*home, "Home");
        }
    } else if (sim->checkDebug("moderate e-commerce")) {
        // Give the Agent an 8% chance each hour to work from Home
        if ((rand() % 100) < 8) {
            agent->setDestination(*home, "Home");
        }
    } else if (sim->checkDebug("weak e-commerce")) {
        // Give the Agent an 3% chance each hour to work from Home
        if ((rand() % 100) < 3) {
            agent->setDestination(*home, "Home");
        }
    }
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
            continue;
        }

        if (sim->checkDebug("total lockdown")) {
            location->setStatus(PandemicLocation::LOCKDOWN);
        } else if (sim->checkDebug("strong lockdown")) {
            if (infectedProportion > 0.15) {
                location->setStatus(PandemicLocation::LOCKDOWN);
            }
        } else if (sim->checkDebug("moderate lockdown")) {
            if (infectedProportion > 0.35) {
                location->setStatus(PandemicLocation::LOCKDOWN);
            }
        } else if (sim->checkDebug("weak lockdown")) {
            if (infectedProportion > 0.75) {
                location->setStatus(PandemicLocation::LOCKDOWN);
            }
        } else {
            if (location->getNumInfectedAgents() > 0) {
                location->setStatus(PandemicLocation::EXPOSURE);
            } else {
                location->setStatus(PandemicLocation::NORMAL);
            }
        }
    }
}


//**************************************************************************


void PandemicController::incrementNearbyInfected(std::vector<PandemicAgent*> &pandemicAgents, size_t index) {

    // Grab the position of the Agent to compare against
    Coordinate comparisonPosition = pandemicAgents[index]->getPosition();
    bool isInfected = pandemicAgents[index]->getStatus() == PandemicAgent::INFECTED;

    for (size_t i = 0; i < pandemicAgents.size(); ++i) {
        if (i == index) continue;

        Coordinate agentPosition = pandemicAgents[i]->getPosition();
        double distance = comparisonPosition.distBetween(agentPosition);

        if (distance < INFECTION_RADIUS) {
            pandemicAgents[i]->incrementNearbyInfected(1 + isInfected);
        }
    }
}


//**************************************************************************
