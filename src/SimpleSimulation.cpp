#include <Headers/SimpleSimulation.h>
#include "Headers/Agent.h"
#include "Headers/EconomicAgent.h"

SimpleSimulation::SimpleSimulation(int numAgents, Ui::MainWindow* ui,
                                   std::map<std::string, bool> debug) :
    Simulation(numAgents, ui, debug) {

    // Create four square regions (Home, Work, School, Leisure)
    this->homeRegion = new SquareRegion(Coordinate(0, 0),
                                        getSimHeight() / 2,
                                        QColor(3, 11, 252),
                                        "Home");
    this->workRegion = new SquareRegion(Coordinate(getSimHeight() / 2, 0),
                                        getSimHeight() / 2,
                                        QColor(255, 0, 255),
                                        "Work");
    this->schoolRegion = new SquareRegion(Coordinate(0, getSimHeight() / 2),
                                            getSimHeight() / 2,
                                            QColor(240, 41, 41),
                                            "School");
    this->leisureRegion = new SquareRegion(Coordinate(getSimHeight() / 2,
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
    generateAgents(getInitialNumAgents());
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
    // Completely reset the screen and the simulation
    clearScreen();
    clearAgents();

    // Redraw the regions to the screen to prevent blank area
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


void SimpleSimulation::test() {
    std::vector<Agent*> agents = getAgents();
    agents[0]->takeTimeStep();
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
        // Randomly sample the four locations of interest for the agent
        int homeIndex = rand() % homeLocations.size();
        int schoolIndex = rand() % schoolLocations.size();
        int workIndex = rand() % workLocations.size();
        int leisureIndex = rand() % leisureLocations.size();

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
        Location* initialLocation;
        if (startingLocation == "Home") {
            initialLocation = homeLocations[homeIndex];
        } else if (startingLocation == "School") {
            initialLocation = schoolLocations[schoolIndex];
        } else if (startingLocation == "Work") {
            initialLocation = workLocations[workIndex];
        } else if (startingLocation == "Leisure") {
            initialLocation = leisureLocations[leisureIndex];
        } else {
            throw "Invalid Behavior File Loaded";
        }

        // Create an agent with the determines behavior and location
        Agent* agent;
        if (type == "Simple") {
            agent = new Agent(ageAssignment,
                              initialLocation,
                              startingLocation,
                              behaviorAssignment);
        } else {
            agent = new EconomicAgent(0,
                                      ageAssignment,
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
        agent->setLocation(homeLocations[homeIndex], Agent::HOME);
        homeLocations[homeIndex]->addAgent(agent);

        agent->setLocation(schoolLocations[schoolIndex], Agent::SCHOOL);
        schoolLocations[schoolIndex]->addAgent(agent);

        agent->setLocation(workLocations[workIndex], Agent::WORK);
        workLocations[workIndex]->addAgent(agent);

        agent->setLocation(leisureLocations[leisureIndex], Agent::LEISURE);
        leisureLocations[leisureIndex]->addAgent(agent);

        // Add the agent to the simulation
        addAgent(agent);
    }
}


//******************************************************************************


Location* SimpleSimulation::getRandomLocation(Agent::LOCATIONS which) {
    if (which == Agent::LOCATIONS::HOME) {
        std::vector<Location*> locations = homeRegion->getLocations();
        return locations[rand() % locations.size()];
    } else if (which == Agent::LOCATIONS::WORK) {
        std::vector<Location*> locations = workRegion->getLocations();
        return locations[rand() % locations.size()];
    } else if (which == Agent::LOCATIONS::SCHOOL) {
        std::vector<Location*> locations = schoolRegion->getLocations();
        return locations[rand() % locations.size()];
    } else if (which == Agent::LOCATIONS::LEISURE) {
        std::vector<Location*> locations = leisureRegion->getLocations();
        return locations[rand() % locations.size()];
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


void SimpleSimulation::renderCharts(const QString &which, bool newChartView) {
    if (which == "ALL") {
        renderChartUpdates("AGE", newChartView);
        renderChartUpdates("BEHAVIOR", newChartView);
        renderChartUpdates("DESTINATION", newChartView);
    } else {
        renderChartUpdates(which, newChartView);
    }
}
