#include "Headers/Simulation.h"

Simulation::Simulation(int numAgents, Ui::MainWindow* ui) {
    assert(numAgents >= 0);
    this->numAgents = numAgents;
    this->agents.reserve(numAgents);
    this->ui = ui;
    this->simHeight = ui->mainCanvas->height();
    this->simWidth = ui->mainCanvas->width();
}


void Simulation::addAgent(Agent *agent) {
    if (this->agents.size() < static_cast<size_t>(this->numAgents)) {
        this->agents.push_back(agent);
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
