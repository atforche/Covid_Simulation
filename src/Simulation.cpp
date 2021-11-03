#include "Headers/Simulation.h"
#include "Headers/EconomicController.h"

// Initialize the static members of the Simulation Class
int Simulation::FRAMES_PER_HOUR = 20;


//******************************************************************************


void Simulation::ageAgents() {

    bool behaviorsUpdated = false;
    bool agentKilled = false;

    // Lock the Agents vector to provide mutual exclusion while deleting agents
    QMutexLocker agentLock(getAgentsLock());

    for (int i = static_cast<int>(agents.size()) - 1; i >= 0; --i) {
        // Increment the age of the agent
        int newAge = agents[i]->incrementAge();

        // If the agent has become an adult, give it a new behavior chart
        if (newAge == 18) {
            behaviorsUpdated = true;
            int newBehavior = agentController->getAdultBehavior();
            agents[i]->setBehavior(newBehavior);
        } else if (newAge == 100) {
            // Kill the agent and remove it
            killAgent(agents.at(i), i);
        }
    }

    if (agentKilled) {
        emit updateChart("ALL", false);
    } else {
        // Emit the signal allowing the main thread to update the chart
        emit updateChart("AGE", false);

        // If a behavior has been changed, update the behavior chart also
        if (behaviorsUpdated) {
            emit updateChart("BEHAVIOR", false);
        }
    }
}


//******************************************************************************


Simulation::Simulation(int numAgents, Ui::MainWindow* ui,
                       std::map<std::string, bool> debug) {

    // Initialize main components of the UI
    assert(numAgents >= 0);
    this->initialNumAgents = numAgents;
    this->agents.reserve(numAgents);
    this->ui = ui;
    this->simHeight = ui->mainCanvas->height();
    this->simWidth = ui->mainCanvas->width();
    this->isReset = false;

    // Read in the debug information
    this->debug = debug;

    // Initialize the time information to zero
    this->year = 0;
    this->day = 0;
    this->hour = 0;

    // Create an AgentController for the Simulation
    this->agentController = new AgentController(this);
    this->chartViews = nullptr;

    // Initialize the chartObject vectors
    ageHelper = new AgeChartHelper();
    behaviorHelper = new BehaviorChartHelper();
    destinationHelper = new DestinationChartHelper();

    // Initialize the locks
    agentLock = new QMutex();
    locationLock = new QMutex();
    queueLock = new QMutex();

    // Update the population counter
    ui->currentPopulation->setText(QString::number(initialNumAgents));

    // Default initialize the homeless shelter
    this->homelessShelter = nullptr;
}


//******************************************************************************


Simulation::~Simulation() {
    // Remove any memory traces from the chartViews
    ui->graphView1->setChart(new QtCharts::QChart());
    ui->graphView2->setChart(new QtCharts::QChart());
    ui->graphView3->setChart(new QtCharts::QChart());

    while (agents.size() > 0) {
        Agent* victim = agents.back();
        agents.pop_back();
        delete victim;
    }
    delete this->chartViews;

    delete agentController;
    delete ageHelper;
    delete behaviorHelper;
    delete destinationHelper;

    delete agentLock;
}


//******************************************************************************


int Simulation::getNumLocations() {
    return ui->numLocations->value();
}


//******************************************************************************


void Simulation::addAgent(Agent *agent) {
    // Add a new agent to the simulation and add it to the screen
    if (this->agents.size() < static_cast<size_t>(2 * this->initialNumAgents)) {
        this->agents.push_back(agent);
        addToAddQueue(agent->getGraphicsObject());
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
    static bool firstFrame = true;

    // Always force the Agent Update to run on the first frame
    if (firstFrame) {
        agentController->updateAgentDestinations(getAgents(), this->hour);
        firstFrame = false;
    }

    if (numFrames == FRAMES_PER_HOUR) {
        // Increment the number of hours
        numFrames = 0;
        this->hour++;
        ui->hour->setText(QString::number(this->hour));

        if (this->hour == 24) {
            // Increment the number of days
            this->hour = 0;
            ui->hour->setText(QString::number(this->hour));
            this->day++;
            ui->day->setText(QString::number(this->day));
            ui->currentPopulation->setText(QString::number(getCurrentNumAgents())); // Update the population counter

            if (this->day == 365) {
                // Increment the number of years
                this->day = 0;
                ageAgents(); // Update the age for every agent in the sim
                ui->day->setText(QString::number(this->day));
                this->year++;
                ui->year->setText(QString::number(this->year));
            }
        }

        // Update each agent's destination assignment each hour
        agentController->updateAgentDestinations(getAgents(), this->hour);
        emit updateChart("DESTINATION", false);
    }

    // Update the population counter
    ui->currentPopulation->setText(QString::number(getCurrentNumAgents()));

    // Increment the frame counter
    numFrames++;
}


//******************************************************************************



void Simulation::killAgent(Agent *victim, int index) {

    // Remove the agent from each Location it belongs to
    if (victim->getLocation(Agent::HOME)) {
        victim->getLocation(Agent::HOME)->removeAgent(victim);
    }
    if (victim->getLocation(Agent::SCHOOL)) {
        victim->getLocation(Agent::SCHOOL)->removeAgent(victim);
    }
    if (victim->getLocation(Agent::WORK)) {
        victim->getLocation(Agent::WORK)->removeAgent(victim);
    }
    if (victim->getLocation(Agent::LEISURE)) {
        victim->getLocation(Agent::LEISURE)->removeAgent(victim);
    }

    // Remove the Agent from the Screen
    addToRemoveQueue(victim->getGraphicsObject());

    // Remove the agent from the vector of agents held in the simulation
    std::iter_swap(agents.begin() + index, agents.begin() + agents.size() - 1);
    agents.pop_back();

    // Delete the agent
    delete victim;
}


//******************************************************************************


void Simulation::birthAgent() {
    if (rand() % 100 == 0) {
        generateAgents(1, true);
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


int Simulation::getSimHeight() {
    return this->simHeight;
}


//******************************************************************************


int Simulation::getSimWidth() {
    return this->simWidth;
}


//******************************************************************************


int Simulation::getInitialNumAgents() {
    return initialNumAgents;
}


//******************************************************************************


int Simulation::getCurrentNumAgents() {
    return static_cast<int>(agents.size());
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

    std::unordered_map<int, QString> indexMap = {
        {0, "AGE"},
        {1, "BEHAVIOR"},
        {2, "DESTINATION"}
    };

    if (ui->simulationType->currentText() == "Economic Simulation") {
        indexMap[3] = "AGENT VALUE";
        indexMap[4] = "ECONOMIC STATUS";
        indexMap[5] = "BUSINESS VALUE";
        indexMap[6] = "TOTAL VALUE";
    } else if (ui->simulationType->currentText() == "Pandemic Simulation") {
        indexMap[3] = "SEIR";
        indexMap[4] = "DAILY TRACKER";
    } else if (ui->simulationType->currentText() == "Dual Simulation") {
        indexMap[3] = "AGENT VALUE";
        indexMap[4] = "ECONOMIC STATUS";
        indexMap[5] = "BUSINESS VALUE";
        indexMap[6] = "TOTAL VALUE";
        indexMap[7] = "SEIR";
        indexMap[8] = "DAILY TRACKER";
    }

    // Delete the existing map to overwrite it
    delete this->chartViews;
    this->chartViews = new std::unordered_map<QString, int>;

    // Initialize vectors for each key
    (*chartViews)["AGE"] = -1;
    (*chartViews)["BEHAVIOR"] = -1;
    (*chartViews)["DESTINATION"] = -1;
    (*chartViews)["AGENT VALUE"] = -1;
    (*chartViews)["ECONOMIC STATUS"] = -1;
    (*chartViews)["BUSINESS VALUE"] = -1;
    (*chartViews)["TOTAL VALUE"] = -1;
    (*chartViews)["SEIR"] = -1;
    (*chartViews)["DAILY TRACKER"] = -1;

    QString graph = indexMap[ui->graph1Selection->currentIndex()];
    (*chartViews)[graph] = 0;

    graph = indexMap[ui->graph2Selection->currentIndex()];
    (*chartViews)[graph] = 1;

    graph = indexMap[ui->graph3Selection->currentIndex()];
    (*chartViews)[graph] = 2;
}


//******************************************************************************


int Simulation::getChartView(QString type) {
    return (*chartViews)[type];
}


//******************************************************************************


void Simulation::renderChartUpdates(QString which, bool newChartView) {
    // If the graph won't be displayed, don't create it
    int graphView = (*chartViews)[which];
    if (graphView == -1) {
        return;
    }

    if (which == "AGE") {
        if (newChartView) {
            QtCharts::QChart* chart = ageHelper->getChart(&getAgents());
            addChartToView(chart, graphView);
        } else {
            ageHelper->updateChart(&getAgents());
        }
    } else if (which == "BEHAVIOR") {
        int numAdultBehaviors = getController()->getNumAdultBehaviors();
        int numChildBehaviors = getController()->getNumChildBehaviors();

        if (newChartView) {
            QtCharts::QChart* chart = behaviorHelper->getChart(&getAgents(), numAdultBehaviors, numChildBehaviors);
            addChartToView(chart, graphView);
        } else {
            behaviorHelper->updateChart(&getAgents(), numAdultBehaviors, numChildBehaviors);
        }
    } else if (which == "DESTINATION") {
        if (newChartView) {
            QtCharts::QChart* chart = destinationHelper->getChart(&getAgents());
            addChartToView(chart, graphView);
        } else {
            destinationHelper->updateChart(&getAgents());
        }
    }
}


//******************************************************************************


QMutex* Simulation::getAgentsLock() {
    return this->agentLock;
}


//******************************************************************************


QMutex* Simulation::getLocationLock() {
    return this->locationLock;
}


//******************************************************************************


QMutex* Simulation::getQueueLock() {
    return this->queueLock;
}


//******************************************************************************


void Simulation::addToAddQueue(QGraphicsItem *item) {
    QMutexLocker lock(getQueueLock());
    addQueue.push_back(item);
}


//******************************************************************************


void Simulation::addToRemoveQueue(QGraphicsItem *item) {
    QMutexLocker lock(getQueueLock());
    removeQueue.push_back(item);
}


//******************************************************************************


Ui::MainWindow* Simulation::getUI() {
    return ui;
}


//******************************************************************************


void Simulation::addChartToView(QChart* chart, int num) {
    if (num == 0) {
        QString title = ui->graphView1->chart()->title();
        QtCharts::QChart* victim = ui->graphView1->chart();
        ui->graphView1->setChart(chart);
        if (title == "") {
            delete victim;
        }
    } else if (num == 1) {
        QString title = ui->graphView2->chart()->title();
        QtCharts::QChart* victim = ui->graphView2->chart();
        ui->graphView2->setChart(chart);
        if (title == "") {
            delete victim;
        }
    } else if (num == 2) {
        QString title = ui->graphView3->chart()->title();
        QtCharts::QChart* victim = ui->graphView3->chart();
        ui->graphView3->setChart(chart);
        if (title == "") {
            delete victim;
        }
    }
}


//******************************************************************************


bool Simulation::wasReset() {
    return this->isReset;
}


//******************************************************************************


void Simulation::setReset(bool resetIn) {
    this->isReset = resetIn;
}


//******************************************************************************


void Simulation::setAgentController(AgentController *newController) {
    delete agentController;
    this->agentController = newController;
}


//******************************************************************************


void Simulation::setHomelessShelter(Location *newLocation) {
    this->homelessShelter = newLocation;
}


//******************************************************************************


Location* Simulation::getHomelessShelter() {
    return this->homelessShelter;
}


//******************************************************************************


void Simulation::updateDebug() {
    this->debug = MainWindow::checkDebugInfo(ui);
}


//******************************************************************************


void Simulation::renderAgentUpdate() {

    // Acquire the lock to prevent an Agent from begin Deleted
    QMutexLocker agentLock(getAgentsLock());
    QMutexLocker screenLock(getQueueLock());

    // Add each of the new QGraphicsObjects to the Screen
    while (addQueue.size() > 0) {
        addToScreen(addQueue.back());
        addQueue.pop_back();
    }

    // Remove each of the old QGraphicsObjects from the screen
    while (removeQueue.size() > 0) {
        ui->mainCanvas->scene()->removeItem(removeQueue.back());
        removeQueue.pop_back();
    }

    // Update the GraphicsObject for each agent in the simulation
    std::vector<Agent*> agents = getAgents();
    for (auto i : agents) {
        i->updateGraphicsObject();
    }
}


//******************************************************************************





