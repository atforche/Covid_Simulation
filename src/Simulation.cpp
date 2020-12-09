#include "Headers/Simulation.h"

Simulation::Simulation(int numAgents, Ui::MainWindow* ui) {
    assert(numAgents >= 0);
    this->locationGenerator = new LocationGenerator();
    this->numAgents = numAgents;
    this->agents.reserve(numAgents);
    this->ui = ui;
    this->simHeight = ui->mainCanvas->height();
    this->simWidth = ui->mainCanvas->width();
}


void Simulation::addAgent(Agent *agent) {
    if (this->agents.size() < static_cast<size_t>(this->numAgents)) {
        this->agents.push_back(agent);
        addToScene(agent->renderAgent());

        if (agent->getVisualize()) {
            std::vector<QGraphicsItem*> lines = agent->renderVisualize();
            for (size_t i = 0; i < lines.size(); ++i) {
                addToScene(lines[i]);
            }
        }
    }
}


std::vector<Agent*>& Simulation::getAgents() {
    return this->agents;
}


void Simulation::addToScene(QGraphicsItem *item) {
    ui->mainCanvas->scene()->addItem(item);
}


void Simulation::clearScene() {
    ui->mainCanvas->scene()->clear();
}


void Simulation::clearAgents() {
    agents.clear();
}


int Simulation::getSimWidth() {
    return this->simWidth;
}


int Simulation::getSimHeight() {
    return this->simHeight;
}


LocationGenerator* Simulation::getLocationGenerator() {
    return this->locationGenerator;
}


int Simulation::getNumAgents() {
    return this->numAgents;
}


bool Simulation::visualizeChecked() {
    return (ui->visualize->checkState() == Qt::CheckState::Checked);
}


Simulation::~Simulation() {
    delete locationGenerator;
}
