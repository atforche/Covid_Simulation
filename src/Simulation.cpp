#include "Headers/Simulation.h"

Simulation::Simulation(int numAgents, Ui::MainWindow* ui) {
    assert(numAgents >= 0);
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
        addToScreen(agent->getGraphicsObject());
    }
}


std::vector<Agent*>& Simulation::getAgents() {
    return this->agents;
}


void Simulation::addToScreen(QGraphicsItem *item) {
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
        agentController->assignAgentDestinations(getAgents(), this->hour);
    }
}


void Simulation::clearScreen() {
    ui->mainCanvas->scene()->clear();
}


void Simulation::clearAgents() {
    agents.clear();
}


void Simulation::generateLocations(Region *region, int num) {
    QRectF regionBound = region->getGraphicsObject()->boundingRect();
    QGraphicsItem* regionGraphics = region->getGraphicsObject();

    std::vector<Location*> locations;
    locations.reserve(num);

    std::uniform_int_distribution<int> xdist(10,
                                            regionBound.width() - 10);
    std::uniform_int_distribution<int> ydist(10,
                                             regionBound.height() - 10);
    std::random_device rand;
    // For each location, random sample points within the boundingRect until the
    // point lies within the shape
    for (int i = 0; i < num; ++i) {
        while(true) {
            int randx = xdist(rand);
            int randy = ydist(rand);

            QPointF check(randx + regionGraphics->boundingRect().topLeft().x(),
                          randy + regionGraphics->boundingRect().topLeft().y());
            if (regionGraphics->boundingRect().contains(check)) {
                locations.push_back(new Location(check.x(), check.y()));
                break;
            }
        }
    }
    region->setLocations(locations);
}


int Simulation::getSimWidth() {
    return this->simWidth;
}


int Simulation::getSimHeight() {
    return this->simHeight;
}


int Simulation::getNumAgents() {
    return this->numAgents;
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
    delete agentController;
}


void Simulation::renderAgentUpdate() {
    std::vector<Agent*> agents = getAgents();
    for (auto i : agents) {
        i->updateGraphicsObject();
    }
}
