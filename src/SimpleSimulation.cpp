#include <Headers/SimpleSimulation.h>
#include "Headers/Agent.h"
#include "Headers/EconomicAgent.h"
#include "Headers/PandemicAgent.h"
#include "Headers/DualAgent.h"

SimpleSimulation::SimpleSimulation(int numAgents, Ui::MainWindow* ui,
                                   std::map<std::string, bool> debug) :
    Simulation(numAgents, ui, debug) {

    // Create four square regions (Home, Work, School, Leisure)
    this->homeRegion = new SquareRegion(this,
                                        Coordinate(0, 0),
                                        getSimHeight() / 2,
                                        QColor(3, 11, 252),
                                        "Home");
    this->workRegion = new SquareRegion(this,
                                        Coordinate(getSimHeight() / 2, 0),
                                        getSimHeight() / 2,
                                        QColor(255, 0, 255),
                                        "Work");
    this->schoolRegion = new SquareRegion(this,
                                          Coordinate(0, getSimHeight() / 2),
                                          getSimHeight() / 2,
                                          QColor(240, 41, 41),
                                          "School");
    this->leisureRegion = new SquareRegion(this,
                                           Coordinate(getSimHeight() / 2,
                                                   getSimHeight() / 2),
                                            getSimHeight() / 2,
                                            QColor(0, 227, 19),
                                            "Leisure");

    // Add the region squares and the name labels to the screen
    addToScreen(homeRegion->getGraphicsObject());
    addToScreen(homeRegion->getNameGraphicsObject());

    addToScreen(workRegion->getGraphicsObject());
    addToScreen(workRegion->getNameGraphicsObject());

    addToScreen(schoolRegion->getGraphicsObject());
    addToScreen(schoolRegion->getNameGraphicsObject());

    addToScreen(leisureRegion->getGraphicsObject());
    addToScreen(leisureRegion->getNameGraphicsObject());
}


//******************************************************************************


SimpleSimulation::~SimpleSimulation() {
    clearScreen();
    delete this->homeRegion;
    delete this->schoolRegion;
    delete this->workRegion;
    delete this->leisureRegion;
}


//******************************************************************************


void SimpleSimulation::init(std::string type) {
    // Generate locations within the HomeRegion and add them to the screen
    homeRegion->generateLocations(getNumLocations(), type);
    std::vector<QGraphicsItem*> toRender = homeRegion->getLocationsGraphicsObject();
    for(auto item : toRender) {
        addToScreen(item);
    }

    // Generate locations within the WorkRegion and add them to the screen
    workRegion->generateLocations(getNumLocations(), type);
    toRender = workRegion->getLocationsGraphicsObject();
    for(auto item : toRender) {
        addToScreen(item);
    }

    // Generate locations within the SchoolRegion and add them to the screen
    schoolRegion->generateLocations(getNumLocations(), type);
    toRender = schoolRegion->getLocationsGraphicsObject();
    for(auto item : toRender) {
        addToScreen(item);
    }

    // Generate locations within the LeisureREgion and add them to the screen
    leisureRegion->generateLocations(getNumLocations(), type);
    toRender = leisureRegion->getLocationsGraphicsObject();
    for(auto item : toRender) {
        addToScreen(item);
    }

    // Generate a set of agents for the simulation
    generateAgents(getInitialNumAgents(), false, type);
}


//******************************************************************************


void SimpleSimulation::execute() {
    // Advance time by a single tick
    advanceTime();

    // Advance time for each agent
    std::vector<Agent*> agents = getAgents();
    for (int i = 0; i < getCurrentNumAgents(); ++i) {
        agents[i]->takeTimeStep();
    }
}


//******************************************************************************


void SimpleSimulation::reset() {

    // Set the reset flag in the base Simulation
    setReset(true);

    // Completely reset the screen and the simulation
    clearScreen();
    clearAgents();

    // Redraw the regions to the screen to prevent blank area
    homeRegion->setGraphicsObject(homeRegion->createNewGraphicsItem());
    addToScreen(homeRegion->getGraphicsObject());
    addToScreen(homeRegion->getNameGraphicsObject());

    workRegion->setGraphicsObject(workRegion->createNewGraphicsItem());
    addToScreen(workRegion->getGraphicsObject());
    addToScreen(workRegion->getNameGraphicsObject());

    schoolRegion->setGraphicsObject(schoolRegion->createNewGraphicsItem());
    addToScreen(schoolRegion->getGraphicsObject());
    addToScreen(schoolRegion->getNameGraphicsObject());

    leisureRegion->setGraphicsObject(leisureRegion->createNewGraphicsItem());
    addToScreen(leisureRegion->getGraphicsObject());
    addToScreen(leisureRegion->getNameGraphicsObject());
}


//******************************************************************************


void SimpleSimulation::generateAgents(int num, bool birth, std::string type) {
    // Create a random color for each strategy
    std::vector<QColor> agentColors;
    int adultBehaviors = getController()->getNumAdultBehaviors();
    int childBehaviors = getController()->getNumChildBehaviors();
    for (int i = 0; i < adultBehaviors + childBehaviors; ++i) {
        agentColors.push_back(QColor(rand() % 256,
                                     rand() % 256,
                                     rand() % 256));
    }

    // Get a set of locations from each Region
    std::vector<Location*> homeLocations = homeRegion->getLocations();
    std::vector<Location*> schoolLocations = schoolRegion->getLocations();
    std::vector<Location*> workLocations = workRegion->getLocations();
    std::vector<Location*> leisureLocations = leisureRegion->getLocations();

    for (int i = 0; i < num; ++i) {

        // Set the Location assignments for each agent
        Location* homeLocation = nullptr;
        Location* schoolLocation = nullptr;
        Location* workLocation = nullptr;
        Location* leisureLocation = nullptr;

        // Randomly sample the four locations of interest for the agent
        if (homeLocations.size() > 0) {
            homeLocation = homeLocations[rand() % homeLocations.size()];
        }
        if (schoolLocations.size() > 0) {
            schoolLocation = schoolLocations[rand() % schoolLocations.size()];
        }
        if (workLocations.size() > 0) {
            workLocation = workLocations[rand() % workLocations.size()];
        }
        if (leisureLocations.size() > 0) {
            leisureLocation = leisureLocations[rand() % leisureLocations.size()];
        }

        // Randomly sample an age assignment for the agent (update later)
        int ageAssignment = 0;
        if (!birth) {
            ageAssignment = AgentController::sampleAgentAge();
        }

        // Randomly sample a behavior assignment of the agent based on its age
        int behaviorAssignment;
        if (ageAssignment >= 18) {
            behaviorAssignment = getController()->getAdultBehavior();
        } else {
            behaviorAssignment = getController()->getChildBehavior();
        }

        // Determine the starting position of this behavior chart and assign
        // this starting position of the agent to it
        QString startingLocation = getController()->getStartingDestination(
                    behaviorAssignment, ageAssignment >= 18);

        // Set the initial location of the agent based on the behavior chart
        Location* initialLocation = nullptr;
        if (startingLocation == "Home") {
            initialLocation = homeLocation;
        } else if (startingLocation == "School") {
            initialLocation = schoolLocation;
        } else if (startingLocation == "Work") {
            initialLocation = workLocation;
        } else if (startingLocation == "Leisure") {
            initialLocation = leisureLocation;
        } else {
            throw "Invalid Behavior File Loaded";
        }

        // Create an agent with the determines behavior and location
        Agent* agent;
        if (type == "Economic") {
            agent = new EconomicAgent(0,
                                      ageAssignment,
                                      initialLocation,
                                      startingLocation,
                                      behaviorAssignment);
        } else if (type == "Pandemic") {
            agent = new PandemicAgent(ageAssignment,
                                      initialLocation,
                                      startingLocation,
                                      behaviorAssignment);
        } else if (type == "Dual") {
            agent = new DualAgent(0,
                                  ageAssignment,
                                  initialLocation,
                                  startingLocation,
                                  behaviorAssignment);
        } else {
            agent = new Agent(ageAssignment,
                              initialLocation,
                              startingLocation,
                              behaviorAssignment);
        }

        // If selected, set a unique color for each behavior
        if (checkDebug("visualize behaviors")) {
            agent->getGraphicsObject()->setPen(
                        agentColors[behaviorAssignment + agent->isAdult() * childBehaviors]
                        );
        }

        // Set the location for each of the Agents locations
        agent->setLocation(homeLocation, Agent::HOME);
        if (homeLocation != nullptr) {
            homeLocation->addAgent(agent);
        }

        agent->setLocation(schoolLocation, Agent::SCHOOL);
        if (schoolLocation != nullptr) {
            schoolLocation->addAgent(agent);
        }

        agent->setLocation(workLocation, Agent::WORK);
        if (workLocation != nullptr) {
            workLocation->addAgent(agent);
        }

        agent->setLocation(leisureLocation, Agent::LEISURE);
        if (leisureLocation != nullptr) {
            leisureLocation->addAgent(agent);
        }

        // Add the agent to the simulation
        addAgent(agent);
    }
}


//******************************************************************************


Location* SimpleSimulation::getRandomLocation(Agent::LOCATIONS which) {
    if (which == Agent::LOCATIONS::HOME) {
        return homeRegion->getRandomLocation();
    } else if (which == Agent::LOCATIONS::WORK) {
        return workRegion->getRandomLocation();
    } else if (which == Agent::LOCATIONS::SCHOOL) {
        return schoolRegion->getRandomLocation();
    } else if (which == Agent::LOCATIONS::LEISURE) {
        return leisureRegion->getRandomLocation();
    }
    return nullptr;
}


//******************************************************************************


Region* SimpleSimulation::getRegion(Agent::LOCATIONS which) {
    if (which == Agent::HOME) {
        return homeRegion;
    } else if (which == Agent::SCHOOL) {
        return schoolRegion;
    } else if (which == Agent::WORK) {
        return workRegion;
    } else if (which == Agent::LEISURE) {
        return leisureRegion;
    }

    return nullptr;
}


//******************************************************************************


std::vector<Location*> SimpleSimulation::getAllLocations() {
    std::vector<Location*> locations;

    // Grab the locations from each Region and append them to the vector
    std::vector<Location*> homeLocations = homeRegion->getLocations();
    locations.reserve(4 * homeLocations.size());
    locations.insert(locations.end(), homeLocations.begin(), homeLocations.end());

    std::vector<Location*> workLocations = workRegion->getLocations();
    locations.insert(locations.end(), workLocations.begin(), workLocations.end());

    std::vector<Location*> schoolLocations = schoolRegion->getLocations();
    locations.insert(locations.end(), schoolLocations.begin(), schoolLocations.end());

    std::vector<Location*> leisureLocations = leisureRegion->getLocations();
    locations.insert(locations.end(), leisureLocations.begin(), leisureLocations.end());

    return locations;
}


//******************************************************************************


void SimpleSimulation::renderCharts(const QString &which, bool newChartView) {
    if (which == "ALL") {
        renderChartUpdates("AGE", newChartView);
        renderChartUpdates("BEHAVIOR", newChartView);
        renderChartUpdates("DESTINATION", newChartView);
    } else {
        renderChartUpdates(which, newChartView);
    }
}
