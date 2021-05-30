#include "Headers/Simulation.h"

void Simulation::ageAgents() {

    bool behaviorsUpdated = false;
    for (size_t i = 0; i < agents.size(); ++i) {
        // Increment the age of the agent
        int newAge = agents[i]->incrementAge();

        // If the agent has become an adult, give it a new behavior chart
        if (newAge == 18) {
            behaviorsUpdated = true;
            int newBehavior = agentController->getAdultBehavior();
            agents[i]->setBehavior(newBehavior);
        }
    }

    // Emit the signal allowing the main thread to update the chart
    emit updateChart("AGE", false);

    // If a behavior has been changed, update the behavior chart also
    if (behaviorsUpdated) {
        emit updateChart("BEHAVIOR", false);
    }
}


//******************************************************************************


void Simulation::addChartToView(QChart* chart, int num) {
    if (num == 0) {
        if (ui->graphView1->chart() != chart) {
            QtCharts::QChart* victim = ui->graphView1->chart();
            ui->graphView1->setChart(chart);
            delete victim;
        }
    } else if (num == 1) {
        if (ui->graphView2->chart() != chart) {
            QtCharts::QChart* victim = ui->graphView2->chart();
            ui->graphView2->setChart(chart);
            delete victim;
        }
    }
}


//******************************************************************************


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
    this->chartViews = nullptr;

    // Initialize the chartObject vectors
    ageHelper = new AgeChartHelper();
    behaviorHelper = new BehaviorChartHelper();
}


//******************************************************************************


Simulation::~Simulation() {
    // Remove any memory traces from the chartViews
    ui->graphView1->setChart(new QtCharts::QChart());
    ui->graphView2->setChart(new QtCharts::QChart());

    while (agents.size() > 0) {
        Agent* victim = agents.back();
        agents.pop_back();
        delete victim;
    }
    delete this->chartViews;

    delete agentController;
    delete ageHelper;
    delete behaviorHelper;
}


//******************************************************************************


void Simulation::addAgent(Agent *agent) {
    // Add a new agent to the simulation and add it to the screen
    if (this->agents.size() < static_cast<size_t>(this->numAgents)) {
        this->agents.push_back(agent);
        addToScreen(agent->getGraphicsObject());
    }
}


//******************************************************************************


std::vector<Agent*>& Simulation::getAgents() {
    return this->agents;
}


//******************************************************************************


void Simulation::addToScreen(QGraphicsItem *item) {
    ui->mainCanvas->scene()->addItem(item);
}


//******************************************************************************


void Simulation::advanceTime() {
    // Increment the time by one frame
    static int numFrames = 0;
    numFrames++;
    if (numFrames >= FRAMES_PER_HOUR) {
        numFrames = 1;
        ageAgents();
        this->hour++;
        ui->hour->setText(QString::number(this->hour));
        if (this->hour == 24) {
            this->hour = 0;
            ui->hour->setText(QString::number(this->hour));
            this->day++;
            ui->day->setText(QString::number(this->day));
            if (this->day == 365) {
                this->day = 0;
                ageAgents(); // Update the age for every agent in the sim
                ui->day->setText(QString::number(this->day));
                this->year++;
                ui->year->setText(QString::number(this->year));
            }
        }

        // Update each agent's destination assignment each hour
        agentController->updateAgentDestinations(getAgents(), this->hour);
    }
}


//******************************************************************************


void Simulation::clearScreen() {
    ui->mainCanvas->scene()->clear();
}


//******************************************************************************


void Simulation::clearAgents() {
    agents.clear();
}


//******************************************************************************


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


//******************************************************************************


int Simulation::getSimHeight() {
    return this->simHeight;
}


//******************************************************************************


int Simulation::getSimWidth() {
    return this->simWidth;
}


//******************************************************************************


int Simulation::getNumAgents() {
    return this->numAgents;
}


//******************************************************************************


int Simulation::getYear() {
    return year;
}


//******************************************************************************


int Simulation::getDay() {
    return day;
}


//******************************************************************************


int Simulation::getHour() {
    return hour;
}


//******************************************************************************


AgentController* Simulation::getController() {
    return agentController;
}


//******************************************************************************


bool Simulation::checkDebug(std::string val) {
    if (this->debug.find(val) != this->debug.end()) {
        return this->debug[val];
    }
    return false;
}


//******************************************************************************


void Simulation::mapChartViews() {

    static std::unordered_map<int, QString> indexMap = {
        {0, "AGE"},
        {1, "BEHAVIOR"}
    };

    // Delete the existing map to overwrite it
    delete this->chartViews;
    this->chartViews = new std::unordered_map<QString, int>;

    // Initialize vectors for each key
    (*chartViews)["AGE"] = -1;
    (*chartViews)["BEHAVIOR"] = -1;

    QString graph = indexMap[ui->graph1Selection->currentIndex()];
    (*chartViews)[graph] = 0;

    graph = indexMap[ui->graph2Selection->currentIndex()];
    (*chartViews)[graph] = 1;
}


//******************************************************************************


int Simulation::getChartView(QString type) {
    return (*chartViews)[type];
}


//******************************************************************************


void Simulation::renderAgeChart(bool newChartView) {

    // If the graph won't be displayed, don't create it
    int graphView = (*chartViews)["AGE"];
    if (graphView == -1) {
        return;
    }

    if (newChartView) {
        QtCharts::QChart* chart = ageHelper->getAgeChart(&getAgents());
        addChartToView(chart, graphView);
    } else {
        ageHelper->updateAgeChart(&getAgents());
    }
}


//******************************************************************************


void Simulation::renderBehaviorChart(bool newChartView) {

    // If the graph won't be displayed, don't create it
    int graphView = (*chartViews)["BEHAVIOR"];
    if (graphView == -1) {
        return;
    }

    int numAdultBehaviors = getController()->getNumAdultBehaviors();
    int numChildBehaviors = getController()->getNumChildBehaviors();

    if (newChartView) {
        QtCharts::QChart* chart = behaviorHelper->getBehaviorChart(&getAgents(), numAdultBehaviors, numChildBehaviors);
        addChartToView(chart, graphView);
    } else {
        behaviorHelper->updateBehaviorChart(&getAgents(), numAdultBehaviors, numChildBehaviors);
    }
}


//******************************************************************************


void Simulation::renderAgentUpdate() {
    // Update the GraphicsObject for each agent in the simulation
    std::vector<Agent*> agents = getAgents();
    for (auto i : agents) {
        i->updateGraphicsObject();
    }
}

