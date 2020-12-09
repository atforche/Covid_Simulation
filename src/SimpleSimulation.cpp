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
    addToScene(homeRegion->renderRegion());
    addToScene(homeRegion->renderName());
    addToScene(workRegion->renderRegion());
    addToScene(workRegion->renderName());
    addToScene(schoolRegion->renderRegion());
    addToScene(schoolRegion->renderName());
    addToScene(leisureRegion->renderRegion());
    addToScene(leisureRegion->renderName());


    agentGenerator = new AgentGenerator(homeRegion,
                                        schoolRegion,
                                        workRegion,
                                        leisureRegion);
}


void SimpleSimulation::init() {

    homeRegion->generateLocations(getLocationGenerator());
    std::vector<QGraphicsItem*> toRender = homeRegion->renderLocations();
    for(auto item : toRender) {
        addToScene(item);
    }

    workRegion->generateLocations(getLocationGenerator());
    toRender = workRegion->renderLocations();
    for(auto item : toRender) {
        addToScene(item);
    }

    schoolRegion->generateLocations(getLocationGenerator());
    toRender = schoolRegion->renderLocations();
    for(auto item : toRender) {
        addToScene(item);
    }

    leisureRegion->generateLocations(getLocationGenerator());
    toRender = leisureRegion->renderLocations();
    for(auto item : toRender) {
        addToScene(item);
    }

    generateAgents();

}


void SimpleSimulation::reset() {
    clearScene();
    clearAgents();

    // Redraw the regions to the screen
    addToScene(homeRegion->renderRegion());
    addToScene(homeRegion->renderName());
    addToScene(workRegion->renderRegion());
    addToScene(workRegion->renderName());
    addToScene(schoolRegion->renderRegion());
    addToScene(schoolRegion->renderName());
    addToScene(leisureRegion->renderRegion());
    addToScene(leisureRegion->renderName());
}


void SimpleSimulation::test() {
    std::vector<Agent*> agents = getAgents();
    agents[0]->update();
}


void SimpleSimulation::generateAgents() {
    std::vector<Agent*> agents = agentGenerator->generate(getNumAgents(),
                                                         visualizeChecked());
    for (auto agent : agents) {
        addAgent(agent);
    }
}


SimpleSimulation::~SimpleSimulation() {
    clearScene();
}
