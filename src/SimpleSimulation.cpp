#include <Headers/SimpleSimulation.h>

SimpleSimulation::SimpleSimulation(int numAgents, Ui::MainWindow* ui,
                                   std::map<std::string, bool> debug) :
    Simulation(numAgents, ui, debug) {

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

    addToScreen(homeRegion->getGraphicsObject());
    addToScreen(homeRegion->getNameGraphicsObject());
    addToScreen(workRegion->getGraphicsObject());
    addToScreen(workRegion->getNameGraphicsObject());
    addToScreen(schoolRegion->getGraphicsObject());
    addToScreen(schoolRegion->getNameGraphicsObject());
    addToScreen(leisureRegion->getGraphicsObject());
    addToScreen(leisureRegion->getNameGraphicsObject());
}


void SimpleSimulation::init() {

    generateLocations(homeRegion, homeRegion->getNumLocations());
    std::vector<QGraphicsItem*> toRender = homeRegion->getLocationsGraphicsObject();
    for(auto item : toRender) {
        addToScreen(item);
    }

    generateLocations(workRegion, workRegion->getNumLocations());
    toRender = workRegion->getLocationsGraphicsObject();
    for(auto item : toRender) {
        addToScreen(item);
    }

    generateLocations(schoolRegion, schoolRegion->getNumLocations());
    toRender = schoolRegion->getLocationsGraphicsObject();
    for(auto item : toRender) {
        addToScreen(item);
    }

    generateLocations(leisureRegion, leisureRegion->getNumLocations());
    toRender = leisureRegion->getLocationsGraphicsObject();
    for(auto item : toRender) {
        addToScreen(item);
    }

    generateAgents();

}


void SimpleSimulation::execute() {
    advanceTime();
    std::vector<Agent*> agents = getAgents();
    for (int i = 0; i < getNumAgents(); ++i) {
        agents[i]->takeTimeStep();
    }
}


void SimpleSimulation::reset() {
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


void SimpleSimulation::test() {
    std::vector<Agent*> agents = getAgents();
    agents[0]->takeTimeStep();
}


void SimpleSimulation::generateAgents() {

    // Dynamically create enough colors to uniquely visualize each of the
    // different strategies
    std::vector<QColor> agentColors;
    int adultBehaviors = getController()->getNumAdultBehaviors();
    int childBehaviors = getController()->getNumChildBehaviors();
    for (int i = 0; i < adultBehaviors + childBehaviors; ++i) {
        agentColors.push_back(QColor(rand() % 256,
                                     rand() % 256,
                                     rand() % 256));
    }

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

        // Randomly sample an age assignment for the agent
        int ageAssignment = rand() % 99;

        // Randomly sample a behavior assignment of the agent based on its age
        int behaviorAssignment;
        if (ageAssignment >= 18) {
            behaviorAssignment = rand() % getController()->getNumAdultBehaviors();
        } else {
            behaviorAssignment = rand() % getController()->getNumChildBehaviors();
        }

        // Determine the starting position of this behavior chart and assign
        // this starting position of the agent to it
        QString startingLocation = getController()->getStartingDestination(
                    behaviorAssignment, ageAssignment >= 18);

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

        // Create the agent with the sampled behavior assignment and starting
        // location
        Agent* agent = new Agent(ageAssignment,
                                 initialLocation,
                                 behaviorAssignment);

        // If the agentBehavior checkbox is checked. Assign unique colors to
        // each behavior if checked (needs fixing to get debug into)
        if (checkDebug("visualize behaviors")) {
            agent->getGraphicsObject()->setPen(
                        agentColors[behaviorAssignment + agent->isAdult() * childBehaviors]
                        );
        }

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


SimpleSimulation::~SimpleSimulation() {
    clearScreen();
}
