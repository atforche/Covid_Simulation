#include "Headers/Simulation.h"

Simulation::Simulation(int numAgents, Ui::MainWindow* ui,
                       std::map<std::string, bool> debug) {

    // Initialize main components of the UI
    assert(numAgents >= 0);
    this->numAgents = numAgents;
    this->agents.reserve(numAgents);
    this->ui = ui;
    this->simHeight = ui->mainCanvas->height();
    this->simWidth = ui->mainCanvas->width();

    // Read in the debug information
    this->debug = debug;

    // Initialize the time information to zero
    this->year = 0;
    this->day = 0;
    this->hour = 0;

    // Create an AgentController for the Simulation
    this->agentController = new AgentController();
}


void Simulation::addAgent(Agent *agent) {
    // Add a new agent to the simulation and add it to the screen
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
    // Increment the time by one frame
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

        // Call update down here to ensure correct hour value passed
        agentController->updateAgentDestination(getAgents(), this->hour);
    }
}


void Simulation::clearScreen() {
    ui->mainCanvas->scene()->clear();
}


void Simulation::clearAgents() {
    agents.clear();
}


void Simulation::generateLocations(Region *region, int num) {

    // Get the boundary of the region and the GraphicsObject
    QRectF regionBound = region->getGraphicsObject()->boundingRect();
    QGraphicsItem* regionGraphics = region->getGraphicsObject();

    // Initialize a vector with enough space for each location
    std::vector<Location*> locations;
    locations.reserve(num);

    // Create random distributions in the range [10, regionBound-10)
    std::uniform_int_distribution<int> xdist(10,
                                            regionBound.width() - 10);
    std::uniform_int_distribution<int> ydist(10,
                                             regionBound.height() - 10);
    std::random_device rand;

    // Create num locations within the region
    for (int i = 0; i < num; ++i) {

        // Randomly sample point from the distribution until one is within the
        // boundaries of the GraphicsObject (Monte Carlo style)
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

    // Give the region its vector of locations
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


AgentController* Simulation::getController() {
    return agentController;
}


bool Simulation::checkDebug(std::string val) {
    if (this->debug.find(val) != this->debug.end()) {
        return this->debug[val];
    }
    return false;
}


Simulation::~Simulation() {
    delete agentController;
}


void Simulation::renderAgentUpdate() {
    // Update the GraphicsObject for each agent in the simulation
    std::vector<Agent*> agents = getAgents();
    for (auto i : agents) {
        i->updateGraphicsObject();
    }
}
