#include <Headers/SimpleSimulation.h>

SimpleSimulation::SimpleSimulation(int numAgents, Ui::MainWindow* ui) :
    Simulation(numAgents, ui) {

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
    std::vector<Location*> homeLocations = homeRegion->getLocations();
    std::vector<Location*> schoolLocations = schoolRegion->getLocations();
    std::vector<Location*> workLocations = workRegion->getLocations();
    std::vector<Location*> leisureLocations = leisureRegion->getLocations();

    for (int i = 0; i < getNumAgents(); ++i) {
        int homeIndex = rand() % homeLocations.size();
        Coordinate* homePosition = homeLocations[homeIndex]->getPosition();
        Agent* agent = new Agent(rand() % 99,
                                 new Coordinate(homePosition->getCoord(Coordinate::X) + rand() % 10,
                                            homePosition->getCoord(Coordinate::Y) + rand() % 10));

        int schoolIndex = rand() % schoolLocations.size();
        int workIndex = rand() % workLocations.size();
        int leisureIndex = rand() % leisureLocations.size();

        agent->setLocation(homeLocations[homeIndex], Agent::HOME);
        homeLocations[homeIndex]->addAgent(agent);
        agent->setLocation(schoolLocations[schoolIndex], Agent::SCHOOL);
        schoolLocations[schoolIndex]->addAgent(agent);
        agent->setLocation(workLocations[workIndex], Agent::WORK);
        workLocations[workIndex]->addAgent(agent);
        agent->setLocation(leisureLocations[leisureIndex], Agent::LEISURE);
        leisureLocations[leisureIndex]->addAgent(agent);

        addAgent(agent);
    }
}


SimpleSimulation::~SimpleSimulation() {
    clearScreen();
}
