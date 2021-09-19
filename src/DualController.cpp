#include "Headers/DualController.h"
#include "Headers/DualLocation.h"
#include "Headers/DualSimulation.h"
#include "Headers/DualAgent.h"

DualController::DualController(Simulation* sim) :
    AgentController(sim),
    PandemicController(sim),
    EconomicController(sim) {};


//******************************************************************************


void DualController::updateAgentDestinations(std::vector<Agent *> &agents, int hour) {

    // Acquire the Locations lock to avoid race conditions
    QMutexLocker locationsLock(getLocationLock());

    // Initialize all values needed for the Pandemic update
    initializePandemicUpdate(agents);

    // Update the Economic value at each Business location
    businessEconomicUpdate(hour);

    // Perform the Pandemic and Economic updates for every agent
    locationsLock.unlock();
    double redistributedValue = 0;
    QMutexLocker agentLock(getAgentLock());

    std::vector<PandemicAgent*> pandemicAgents = getPandemicAgents();
    for (int i = static_cast<int>(agents.size()) - 1; i >= 0; --i) {
        // Update each Agents location according to their Behavior Chart
        updateSingleDestination(agents[i], hour, true);

        bool died = agentPandemicUpdate(pandemicAgents[i], static_cast<int>(i));
        if (died) {
            pandemicAgents[i] = nullptr;
        } else {
            redistributedValue += agentEconomicUpdate(dynamic_cast<EconomicAgent*>(agents[i]));
        }
    }

    // Spread the Infection to new Agents
    spreadInfection(pandemicAgents);

    // Give the virus a change to re-emerge if it disappears
    spontaneousInfection();

    // Distribute value to locations and spawn new businesses if applicable
    finishEconomicUpdate(redistributedValue, "Dual");
}


//******************************************************************************


void DualController::updateSingleDestination(Agent *agent, int hour, bool randomAllowed) {
    PandemicController::updateSingleDestination(agent, hour, randomAllowed);
}


//******************************************************************************


void DualController::businessEconomicUpdate(int hour) {

    EconomicSimulation* sim = dynamic_cast<EconomicSimulation*>(getSim());

    // Call the base businessEconomicUpdate
    EconomicController::businessEconomicUpdate(hour);

    // Update Agent and Business value according to the Economic assistance flag
    if (sim->checkDebug("weak assistance") ||
            sim->checkDebug("moderate assistance") ||
            sim->checkDebug("strong assistance")) {

        // Loop through all the work locations
        std::vector<Location*> workLocations = sim->getRegion(Agent::WORK)->getLocations();
        for (int i = static_cast<int>(workLocations.size()) - 1; i >= 0; --i) {
            DualLocation* workLocation = dynamic_cast<DualLocation*>(workLocations[i]);

            // Only affect the work location during the day
            int currentHour = sim->getHour();
            if (currentHour > 7 && currentHour < 20) {

                // If the business is locked down, give them and each of
                // their employees some additional value
                if (workLocation->getStatus() == PandemicLocation::LOCKDOWN) {
                    double additionalAssistance;
                    if (sim->checkDebug("strong assistance")) {
                        additionalAssistance = 0.4;
                    } else if (sim->checkDebug("moderate assistance")) {
                        additionalAssistance = 0.25;
                    } else {
                        additionalAssistance = 0.15;
                    }

                    // Add the additional assistance to the Agent
                    workLocation->incrementValue(additionalAssistance * workOverhead);

                    // Provide additional assistance to each of the Locations workers
                    std::unordered_set<Agent*> workers = workLocation->getAgents();
                    for (auto it = workers.begin(); it != workers.end(); ++it) {
                        DualAgent* worker = dynamic_cast<DualAgent*>(*it);
                        worker->incrementValue(additionalAssistance * workLocation->getCost());
                    }

                } else {
                    // Otherwise, make the business pay some additional overhead
                    double additionalOverhead;
                    if (sim->checkDebug("strong assistance")) {
                        additionalOverhead = 0.2;
                    } else if (sim->checkDebug("moderate assistance")) {
                        additionalOverhead = 0.125;
                    } else {
                        additionalOverhead = 0.075;
                    }

                    // Decrease the work locations value by the additional overhead
                    workLocation->incrementValue(-1 * additionalOverhead * workOverhead);
                }
            }
        }
    }
}


//******************************************************************************


double DualController::homeEconomicUpdate(EconomicAgent *agent) {

    // Get the simulation
    DualSimulation* sim = dynamic_cast<DualSimulation*>(getSim());
    DualAgent* dualAgent = dynamic_cast<DualAgent*>(agent);

    // Prevent infected homeless agents from moving into new homes if any sort
    // of lockdown is in effect
    double returnValue = 0;
    if (agent->getStatus() == EconomicAgent::HOMELESS ||
            agent->getStatus() == EconomicAgent::BOTH) {

        // Check to make sure some sort of lockdown is in effect
        if (sim->checkDebug("weak lockdown") ||
                sim->checkDebug("moderate lockdown") ||
                sim->checkDebug("strong lockdown")) {

            // If a homeless agent is infected or exposed, prevent them from getting
            // placed in a new home
            if (dualAgent->PandemicAgent::getStatus() != PandemicAgent::EXPOSED &&
                    dualAgent->PandemicAgent::getStatus() != PandemicAgent::INFECTED) {
                // Call the base EconomicController::homeEconomicUpdate function
                returnValue = EconomicController::homeEconomicUpdate(agent);
            }
        }
    } else {

        // Call the base EconomicController::homeEconomicUpdate function
        returnValue = EconomicController::homeEconomicUpdate(agent);
    }

    // If there is government assistance, give homeless agents some value
    if (agent->getStatus() == EconomicAgent::HOMELESS ||
            agent->getStatus() == EconomicAgent::BOTH) {

        if (sim->checkDebug("strong assistance")) {
            agent->incrementValue(3);
        } else if (sim->checkDebug("moderate assistance")) {
            agent->incrementValue(2);
        } else if (sim->checkDebug("weak assistance")) {
            agent->incrementValue(1);
        }
    } else {
        // If there is government assistance, make the agents pay some extra for their homes
        int homeCost = dynamic_cast<EconomicLocation*>(agent->getLocation(Agent::HOME))->getCost();
        if (sim->checkDebug("strong assistance")) {
            agent->incrementValue(-1 * 0.2 * homeCost);
        } else if (sim->checkDebug("moderate assistance")) {
            agent->incrementValue(-1 * 0.125 * homeCost);
        } else if (sim->checkDebug("weak assistance")) {
            agent->incrementValue(-1 * 0.075 * homeCost);
        }
    }

    return returnValue;
}


//******************************************************************************


void DualController::workEconomicUpdate(EconomicAgent *agent) {

    // Get the simulation
    DualSimulation* sim = dynamic_cast<DualSimulation*>(getSim());

    // Call the base EconomicSimulation::workEconomicUpdate function
    EconomicController::workEconomicUpdate(agent);

    // If there is government assistance, give unemployed agents some value
    if (agent->getStatus() == EconomicAgent::UNEMPLOYED ||
            agent->getStatus() == EconomicAgent::BOTH) {

        if (sim->checkDebug("strong assistance")) {
            agent->incrementValue(3);
        } else if (sim->checkDebug("moderate assistance")) {
            agent->incrementValue(2);
        } else if (sim->checkDebug("weak assistance")) {
            agent->incrementValue(1);
        }
    }
}


//******************************************************************************


void DualController::lockdownLocations() {

    // Update every Location if Lockdowns are enforced
    PandemicSimulation* sim = dynamic_cast<PandemicSimulation*>(getSim());
    std::vector<Location*> locations = sim->getAllLocations();
    for (size_t i = 0; i < locations.size(); ++i) {
        // Get the Location and calculate the infected proportion
        DualLocation* location = dynamic_cast<DualLocation*>(locations[i]);
        double infectedProportion = location->getNumAgents() > 0 ?
                    static_cast<double>(location->getNumInfectedAgents()) / location->getNumAgents() :
                    0;

        // Home locations cannot go on lockdown
        if (location->PandemicLocation::getType() == Agent::HOME) {
            continue;
        }

        // If it's a leisure location and it's sibling is locked down,
        // automatically lock down also
        DualLocation* sibling = dynamic_cast<DualLocation*>(location->getSibling());
        if (location->PandemicLocation::getType() == Agent::LEISURE &&
                sibling != nullptr && sibling->PandemicLocation::getStatus() == PandemicLocation::LOCKDOWN) {
            location->setStatus(PandemicLocation::LOCKDOWN);
            continue;
        }

        // If all of the Agents at the Location are Infected, immediately lockdown
        if (infectedProportion == 1) {
            location->setStatus(PandemicLocation::LOCKDOWN);
            continue;
        }

        if (sim->checkDebug("total lockdown")) {
            location->setStatus(PandemicLocation::LOCKDOWN);
        } else if (sim->checkDebug("strong lockdown")) {
            if (infectedProportion > 0.20) {
                location->setStatus(PandemicLocation::LOCKDOWN);
            }
        } else if (sim->checkDebug("moderate lockdown")) {
            if (infectedProportion > 0.40) {
                location->setStatus(PandemicLocation::LOCKDOWN);
            }
        } else if (sim->checkDebug("weak lockdown")) {
            if (infectedProportion > 0.60) {
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


void DualController::makeHomeless(EconomicAgent *agent) {

    // Get the Simulation and Agent
    DualSimulation* sim = dynamic_cast<DualSimulation*>(getSim());
    DualAgent* dualAgent = dynamic_cast<DualAgent*>(agent);

    // Agent's can't become homeless on Day 1
    if (sim->getDay() == 0) {
        return;
    }

    // If an Agent is Infected and quarantine is being enforced, then they cannot
    // become homeless
    if (dualAgent->PandemicAgent::getStatus() == PandemicAgent::INFECTED &&
            sim->checkDebug("quarantine when infected")) {
        return;
    }

    // If an Agent is Exposed and contact tracing is being enforced, then they
    // cannot become homeless
    if (dualAgent->PandemicAgent::getStatus() == PandemicAgent::EXPOSED &&
            (sim->checkDebug("weak contact tracing") ||
             sim->checkDebug("moderate contact tracing") ||
             sim->checkDebug("strong contact tracing"))) {
        return;
    }

    // If there is an outbreak at the homeless shelter, don't allow more agents
    // to become homeless
    if (getNumHomelessInfected() > 0) {
        return;
    }


    // Updates the Agent's status and location
    if (agent->getStatus() == EconomicAgent::UNEMPLOYED) {
        agent->setStatus(EconomicAgent::BOTH);
    } else {
        agent->setStatus(EconomicAgent::HOMELESS);
    }

    // Update the numInfectedAgents count for the Agent's current home location
    DualLocation* currentHome = dynamic_cast<DualLocation*>(agent->getLocation(Agent::HOME));

    if (dualAgent != nullptr && currentHome != nullptr) {
        if (dualAgent->PandemicAgent::getStatus() == PandemicAgent::INFECTED) {
            currentHome->removeInfectedAgent();
        }
    }

    // Update the location pointer of the Agent
    agent->setLocation(nullptr, Agent::HOME);
}


//******************************************************************************


void DualController::finishEconomicUpdate(double redistributedValue, QString type) {

    DualSimulation* sim = dynamic_cast<DualSimulation*>(getSim());

    // If there are less than the initial amount of businesses, have a chance
    // to generate a new business and leisure location
    std::vector<Location*> workLocations = sim->getRegion(Agent::WORK)->getLocations();
    if (static_cast<int>(workLocations.size()) < sim->getUI()->numLocations->value()) {
        if (rand() % 5 == 0) {
            generateNewBusiness(type);
        }
    }

    // Distribute the value lost from agents at home equally to each business
    int openBusinesses = 0;
    std::vector<Location*> locations = sim->getRegion(Agent::WORK)->getLocations();

    // Count the number of business that are opened
    for (size_t i = 0; i < locations.size(); ++i) {
        DualLocation* location = dynamic_cast<DualLocation*>(locations[i]);
        if (location->PandemicLocation::getStatus() != PandemicLocation::LOCKDOWN) {
            openBusinesses++;
        }
    }

    // If there are no open businesses, don't distribute any value
    if (openBusinesses == 0) {
        return;
    }

    // Distribute the value to each Open business
    setTotalBusinessValue(getTotalBusinessValue() + redistributedValue);
    for (size_t i = 0; i < locations.size(); ++i) {
        DualLocation* location = dynamic_cast<DualLocation*>(locations[i]);
        if (location->PandemicLocation::getStatus() != PandemicLocation::LOCKDOWN) {
            location->incrementValue(redistributedValue / openBusinesses);
        }
    }
}


//******************************************************************************


