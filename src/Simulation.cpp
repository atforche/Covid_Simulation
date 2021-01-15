#include "Headers/Simulation.h"

Simulation::Simulation(int numAgents, Ui::MainWindow* ui) {
    assert(numAgents >= 0);
    this->locationGenerator = new LocationGenerator();
    this->numAgents = numAgents;
    this->agents.reserve(numAgents);
    this->ui = ui;
    this->simHeight = ui->mainCanvas->height();
    this->simWidth = ui->mainCanvas->width();

    this->year = 0;
    this->day = 0;
    this->hour = 0;

    this->agentController = new AgentController(NUM_BEHAVIORS);
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


void Simulation::renderAgentUpdate() {
    std::vector<Agent*> agents = getAgents();
    for (auto i : agents) {
        i->renderUpdate();
    }
}


std::vector<Agent*>& Simulation::getAgents() {
    return this->agents;
}


void Simulation::addToScene(QGraphicsItem *item) {
    ui->mainCanvas->scene()->addItem(item);
}


void Simulation::advanceTime() {
    static int numFrames = 0;
    numFrames++;
    if (numFrames >= FRAMES_PER_HOUR) {
        numFrames = 1;
        this->hour++;
        ui->hour->setText(QString::number(this->hour));
        if (this->hour == 24) {
            this->hour = 0;
            ui->hour->setText(QString::number(this->hour));
            this->day++;
            ui->day->setText(QString::number(this->day));
            if (this->day == 365) {
                this->day = 0;
                ui->day->setText(QString::number(this->day));
                this->year++;
                ui->year->setText(QString::number(this->year));
            }
        }
        // Call down here to ensure correct hour value passed
        agentController->setDestinations(getAgents(), this->hour);
    }
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


int Simulation::getYear() {
    return year;
}


int Simulation::getDay() {
    return day;
}


int Simulation::getHour() {
    return hour;
}


Simulation::~Simulation() {
    delete locationGenerator;
    delete agentController;
}
