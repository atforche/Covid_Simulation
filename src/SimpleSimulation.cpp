#include <Headers/SimpleSimulation.h>

SimpleSimulation::SimpleSimulation(int numAgents, Ui::MainWindow* ui,
                                   std::map<std::string, bool> debug) :
    Simulation(numAgents, ui, debug) {

    // Create four square regions (Home, Work, School, Leisure)
    this->homeRegion = new SquareRegion(ui->numLocations->value(),
                                        Coordinate(0, 0),
                                        getSimHeight() / 2,
                                        QColor(3, 11, 252),
                                        "Home");
    this->workRegion = new SquareRegion(ui->numLocations->value(),
                                        Coordinate(getSimHeight() / 2, 0),
                                        getSimHeight() / 2,
                                        QColor(255, 0, 255),
                                        "Work");
    this->schoolRegion = new SquareRegion(ui->numLocations->value(),
                                        Coordinate(0, getSimHeight() / 2),
                                        getSimHeight() / 2,
                                        QColor(240, 41, 41),
                                        "School");
    this->leisureRegion = new SquareRegion(ui->numLocations->value(),
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


void SimpleSimulation::init() {
    // Generate locations within the HomeRegion and add them to the screen
    generateLocations(homeRegion, homeRegion->getNumLocations());
    std::vector<QGraphicsItem*> toRender = homeRegion->getLocationsGraphicsObject();
    for(auto item : toRender) {
        addToScreen(item);
    }

    // Generate locations within the WorkRegion and add them to the screen
    generateLocations(workRegion, workRegion->getNumLocations());
    toRender = workRegion->getLocationsGraphicsObject();
    for(auto item : toRender) {
        addToScreen(item);
    }

    // Generate locations within the SchoolRegion and add them to the screen
    generateLocations(schoolRegion, schoolRegion->getNumLocations());
    toRender = schoolRegion->getLocationsGraphicsObject();
    for(auto item : toRender) {
        addToScreen(item);
    }

    // Generate locations within the LeisureREgion and add them to the screen
    generateLocations(leisureRegion, leisureRegion->getNumLocations());
    toRender = leisureRegion->getLocationsGraphicsObject();
    for(auto item : toRender) {
        addToScreen(item);
    }

    // Generate a set of agents for the simulation
    generateAgents();
}


//******************************************************************************


void SimpleSimulation::execute() {
    // Advance time by a single tick
    advanceTime();

    // Advance time for each agent
    std::vector<Agent*> agents = getAgents();
    for (int i = 0; i < getNumAgents(); ++i) {
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


void SimpleSimulation::generateAgents() {
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

    for (int i = 0; i < getNumAgents(); ++i) {
        // Randomly sample the four locations of interest for the agent
        int homeIndex = rand() % homeLocations.size();
        int schoolIndex = rand() % schoolLocations.size();
        int workIndex = rand() % workLocations.size();
        int leisureIndex = rand() % leisureLocations.size();

        // Randomly sample an age assignment for the agent (update later)
        int ageAssignment = AgentController::sampleAgentAge();

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
        Agent* agent = new Agent(ageAssignment,
                                 initialLocation,
                                 behaviorAssignment);

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


void SimpleSimulation::renderCharts(const QString &which, bool newChartView) {
    if (which == "ALL") {
        renderAgeChart(newChartView);
        renderBehaviorChart(newChartView);
    } else if (which == "AGE") {
        renderAgeChart(newChartView);
    } else if (which == "BEHAVIOR") {
        renderBehaviorChart(newChartView);
    }
}
