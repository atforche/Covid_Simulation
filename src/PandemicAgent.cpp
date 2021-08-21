#include "Headers/PandemicAgent.h"

PandemicAgent::PandemicAgent(int age, Location* startingLocation,
                             QString startingLocationString, int behavior) :
    Agent(age, startingLocation, startingLocationString, behavior) {

    // Give the agent the default pandemic status
    this->currentStatus = SUSCEPTIBLE;
    this->daysInStage = 0;
    this->compliant = true;
    this->nearbyInfected = 0;

    // Give the agent a random health status, proportional to the US population
    int randNum = rand() % 100;
    if (randNum < 50) {
        this->healthStatus = HEALTHY;
    } else if (randNum < 75) {
        this->healthStatus = MODERATE;
    } else if (randNum < 90) {
        this->healthStatus = POOR;
    } else {
        this->healthStatus = VERY_POOR;
    }
}


//******************************************************************************


PandemicAgent::STATUS PandemicAgent::getStatus() {
    return this->currentStatus;
}


//******************************************************************************


PandemicAgent::HEALTH PandemicAgent::getHealthStatus() {
    return this->healthStatus;
}


//******************************************************************************


void PandemicAgent::setStatus(STATUS newStatus) {
    this->currentStatus = newStatus;
}


//******************************************************************************


bool PandemicAgent::advanceDay() {

    bool newCase = false;
    if (currentStatus == EXPOSED) {
        if (daysInStage + ((rand() % 3) - 1) > 1) {
            makeInfected();
            newCase = true;
        }
    } else if (currentStatus == INFECTED) {
        if (daysInStage + ((rand() % 3) - 1) > 4) {
            makeRecovered();
        }
    } else if (currentStatus == RECOVERED) {
        if (daysInStage + ((rand() % 3) - 1) > 9) {
            makeSusceptible();
        }
    }

    ++this->daysInStage;
    return newCase;
}


//******************************************************************************


void PandemicAgent::makeSusceptible() {
    setStatus(SUSCEPTIBLE);
    this->daysInStage = 0;
}


//******************************************************************************


void PandemicAgent::makeExposed() {
    setStatus(EXPOSED);
    daysInStage = 0;
}


//******************************************************************************


void PandemicAgent::makeInfected() {
    setStatus(INFECTED);
    this->daysInStage = 0;

    // Update the number of Infected agents at each location
    PandemicLocation* location = dynamic_cast<PandemicLocation*>(getLocation(Agent::HOME));
    if (location != nullptr) {
        location->addInfectedAgent();
    }

    location = dynamic_cast<PandemicLocation*>(getLocation(Agent::SCHOOL));
    if (location != nullptr) {
        location->addInfectedAgent();
    }

    location = dynamic_cast<PandemicLocation*>(getLocation(Agent::WORK));
    if (location != nullptr) {
        location->addInfectedAgent();
    }

    location = dynamic_cast<PandemicLocation*>(getLocation(Agent::LEISURE));
    if (location != nullptr) {
        location->addInfectedAgent();
    }
}


//******************************************************************************


void PandemicAgent::makeRecovered() {
    setStatus(RECOVERED);
    this->daysInStage = 0;

    // Update the number of Infected agents at each location
    PandemicLocation* location = dynamic_cast<PandemicLocation*>(getLocation(Agent::HOME));
    if (location != nullptr) {
        location->removeInfectedAgent();
    }

    location = dynamic_cast<PandemicLocation*>(getLocation(Agent::SCHOOL));
    if (location != nullptr) {
        location->removeInfectedAgent();
    }

    location = dynamic_cast<PandemicLocation*>(getLocation(Agent::WORK));
    if (location != nullptr) {
        location->removeInfectedAgent();
    }

    location = dynamic_cast<PandemicLocation*>(getLocation(Agent::LEISURE));
    if (location != nullptr) {
        location->removeInfectedAgent();
    }
}


//******************************************************************************


bool PandemicAgent::evaluateDeathProbability() {

    double survivalProbability;
    if (getAge() < 50) {
        survivalProbability = 100;
    } else {
        survivalProbability = 100 - 2 * (getAge() - 50);
    }

    if (healthStatus == MODERATE) {
        survivalProbability = survivalProbability / 2;
    } else if (healthStatus == POOR) {
        survivalProbability = survivalProbability / 3;
    } else if (healthStatus == VERY_POOR) {
        survivalProbability = survivalProbability / 4;
    }

    if (rand() % static_cast<int>((survivalProbability * 100)) == 0) {
        return true;
    }
    return false;
}


//******************************************************************************


void PandemicAgent::setCompliance(bool compliant) {
    this->compliant = compliant;
}


//******************************************************************************


PandemicAgent::~PandemicAgent() {
    // If the agent was infected, decrement the count at each location
    if (getStatus() == INFECTED) {
        // Update the number of Infected agents at each location
        PandemicLocation* location = dynamic_cast<PandemicLocation*>(getLocation(Agent::HOME));
        if (location != nullptr) {
            location->removeInfectedAgent();
        }

        location = dynamic_cast<PandemicLocation*>(getLocation(Agent::SCHOOL));
        if (location != nullptr) {
            location->removeInfectedAgent();
        }

        location = dynamic_cast<PandemicLocation*>(getLocation(Agent::WORK));
        if (location != nullptr) {
            location->removeInfectedAgent();
        }

        location = dynamic_cast<PandemicLocation*>(getLocation(Agent::LEISURE));
        if (location != nullptr) {
            location->removeInfectedAgent();
        }
    }
}


//******************************************************************************


void PandemicAgent::resetNearbyInfected() {
    nearbyInfected = 0;
}


//******************************************************************************


void PandemicAgent::incrementNearbyInfected(int amount) {
    nearbyInfected += amount;
}


//******************************************************************************


bool PandemicAgent::evaluateInfectionProbability(bool checkCompliance) {
    // Only Susceptible agents can become exposed
    if (getStatus() != PandemicAgent::SUSCEPTIBLE) return false;

    double infectionLiklihood = nearbyInfected * nearbyInfected;
    int threshold = checkCompliance ? 1500 : 1000;

    if (rand() % threshold < infectionLiklihood) {
        makeExposed();
        return true;
    }
    return false;
}


//******************************************************************************
