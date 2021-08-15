#include "Headers/PandemicController.h"


PandemicController::PandemicController(Simulation* sim,
                                       std::map<std::string, std::string> flags) {

    // Initialize values for the controller
    this->sim = dynamic_cast<PandemicSimulation*>(sim);
    this->flags = flags;

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


std::vector<int> PandemicController::executePandemicUpdate() {

    // Grab the agents and the Locations
    std::vector<Agent*> agents = sim->getAgents();

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

    // Update every Agent
    for (int i = static_cast<int>(agents.size()) - 1; i >= 0; --i) {
        PandemicAgent* agent = dynamic_cast<PandemicAgent*>(agents[i]);
        if (agent->getStatus() == PandemicAgent::SUSCEPTIBLE) {
            numSusceptible++;
        } else if (agent->getStatus() == PandemicAgent::EXPOSED) {
            numExposed++;
        } else if (agent->getStatus() == PandemicAgent::INFECTED) {
            numInfected++;
            bool died = agent->evaluateDeathProbability();
            if (died) {
                sim->killAgent(agent, i);
                newDailyDeaths++;
                totalDeaths++;
            }
        } else {
            numRecovered++;
        }
    }

    // Update every Location if Lockdowns are enforced
    if (sim->checkDebug("weak lockdown") ||
            sim->checkDebug("moderate lockdown") ||
            sim->checkDebug("strong lockdown")) {

        std::vector<Location*> locations = sim->getAllLocations();
        for (size_t i = 0; i < locations.size(); ++i) {
            // Get the Location and calculate the infected proportion
            PandemicLocation* location = dynamic_cast<PandemicLocation*>(locations[i]);
            double infectedProportion = static_cast<double>(location->getNumInfectedAgents()) / location->getNumAgents();

            // Home locations cannot go on lockdown
            if (location->getType() == Agent::HOME) {
                continue;
            }

            if (sim->checkDebug("strong lockdown")) {
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

    return std::vector<int>{numSusceptible, numExposed, numInfected, numRecovered};
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


void PandemicController::overrideAgentDestinations(std::vector<Agent *> &agents) {

    for (size_t i = 0; i < agents.size(); ++i) {
        PandemicAgent* agent = dynamic_cast<PandemicAgent*>(agents[i]);
        PandemicLocation* home = dynamic_cast<PandemicLocation*>(agent->getLocation(Agent::HOME));
        PandemicLocation* work = dynamic_cast<PandemicLocation*>(agent->getLocation(Agent::WORK));
        PandemicLocation* school = dynamic_cast<PandemicLocation*>(agent->getLocation(Agent::SCHOOL));
        PandemicLocation* leisure = dynamic_cast<PandemicLocation*>(agent->getLocation(Agent::LEISURE));

        // Enforce Lockdowns. Even non-compliant agents must follow lockdowns
        enforceLockdowns(agent, home);

        // Calculate whether the Agent will comply with self-enforced measures
        if (!willComply()) {
            continue;
        }

        // Enforce the Quarantine when Infected Flag and update the currentLocation
        enforceQuarantine(agent, home);

        // Enable an Agent to go to Work, School, or Leisure from Home
        applyECommerce(agent, home);

        // Enforce the Contact Tracing Flag and update the currentLocation
        enforceContactTracing(agent, home, school, work, leisure);

        // Enfore the Government guidelines
        enforceGuidelines(agent, home);
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
                if (rand() % 3 == 0) {
                    agent->setDestination(*home, "Home");
                }
            } else if (sim->checkDebug("moderate contact tracing")) {
                if (rand() % 7 == 0) {
                    agent->setDestination(*home, "Home");
                }
            } else if (sim->checkDebug("weak contact tracing")) {
                if (rand() % 20 == 0) {
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
