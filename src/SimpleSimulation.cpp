#include <Headers/SimpleSimulation.h>

SimpleSimulation::SimpleSimulation(int numAgents, Ui::MainWindow* ui) :
    Simulation(numAgents, ui) {

    this->homeRegion = new SquareRegion(50,
                                        Coordinate(0, 0),
                                        getSimHeight() / 2,
                                        QColor(3, 11, 252),
                                        "Home");
    this->workRegion = new SquareRegion(50,
                                        Coordinate(getSimHeight() / 2, 0),
                                        getSimHeight() / 2,
                                        QColor(255, 0, 255),
                                        "Work");
    this->schoolRegion = new SquareRegion(50,
                                        Coordinate(0, getSimHeight() / 2),
                                        getSimHeight() / 2,
                                        QColor(240, 41, 41),
                                        "School");
    this->leisureRegion = new SquareRegion(50,
                                        Coordinate(getSimHeight() / 2,
                                                   getSimHeight() / 2),
                                        getSimHeight() / 2,
                                        QColor(0, 227, 19),
                                        "Leisure");
    addToScene(homeRegion->renderRegion());
    addToScene(homeRegion->renderName());
    addToScene(workRegion->renderRegion());
    addToScene(workRegion->renderName());
    addToScene(schoolRegion->renderRegion());
    addToScene(schoolRegion->renderName());
    addToScene(leisureRegion->renderRegion());
    addToScene(leisureRegion->renderName());
}


void SimpleSimulation::init() {
    Agent* agent = new Agent(36);
    addAgent(agent);
    addToScene(agent->renderAgent());

    homeRegion->generateLocations();
    std::vector<QGraphicsItem*> toRender = homeRegion->renderLocations();
    for(auto item : toRender) {
        addToScene(item);
    }

    workRegion->generateLocations();
    toRender = workRegion->renderLocations();
    for(auto item : toRender) {
        addToScene(item);
    }

    schoolRegion->generateLocations();
    toRender = schoolRegion->renderLocations();
    for(auto item : toRender) {
        addToScene(item);
    }

    leisureRegion->generateLocations();
    toRender = leisureRegion->renderLocations();
    for(auto item : toRender) {
        addToScene(item);
    }
}


void SimpleSimulation::reset() {
    clearScene();
    clearAgents();
}

void SimpleSimulation::test() {
    std::vector<Agent*> agents = getAgents();
    agents[0]->update();
}


void SimpleSimulation::generateAgents(Agent *agent) {
    std::vector<Agent*> agents = getAgents();
    agents[0] = agent;
    addToScene(agent->renderAgent());
}


SimpleSimulation::~SimpleSimulation() {
    clearScene();
}
