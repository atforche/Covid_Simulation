#include "Headers/PandemicSimulation.h"


PandemicSimulation::PandemicSimulation(int lagPeriod, int initialInfected, int initialValue, int numAgents,
                                       Ui::MainWindow* ui, std::map<std::string, bool> debug) :
    EconomicSimulation(initialValue, numAgents, ui, debug) {

    // Initialize parameters of Simulation
    this->lagPeriod = lagPeriod;
    this->initialInfected = initialInfected;

    // Initialize the chart helpers
    SEIRHelper = new SEIRChartHelper();
    dailyTrackerHelper = new DailyTrackerChartHelper();

    // Initialize the pandemic controller
    setAgentController(new PandemicController(this));

    // Initialize all values to zero
    numSusceptible = 0;
    numExposed = 0;
    numInfected = 0;
    numRecovered = 0;
}


//******************************************************************************


void PandemicSimulation::execute() {

    // Static counter to determine if an hour has passed
    static int numFrames = 0;

    // Cast the AgentController to a PandemicController
    PandemicController* controller = dynamic_cast<PandemicController*>(getController());

    // Update the Debug information in the Simulation
    updateDebug();

    // SIMPLESIMULATION::EXECUTE ***********************************************
    // Advance the time in the Simulation and update the Agent's destinations
    advanceTime();

    if (numFrames == FRAMES_PER_HOUR) {
        // Update the Pandemic and Locations according to the Rules
        std::vector<int> SEIR = controller->getSEIR();
        numSusceptible = SEIR[PandemicAgent::SUSCEPTIBLE];
        numExposed = SEIR[PandemicAgent::EXPOSED];
        numInfected = SEIR[PandemicAgent::INFECTED];
        numRecovered = SEIR[PandemicAgent::RECOVERED];
    }

    // Advance time for each agent
    std::vector<Agent*> agents = getAgents();
    for (int i = 0; i < getCurrentNumAgents(); ++i) {
        agents[i]->takeTimeStep();
    }

    // Every hour update the SEIR Chart
    if (numFrames == FRAMES_PER_HOUR) {
        updateChart("SEIR", false);
        numFrames = 0;

        // Update the Daily Tracker chart every day
        if (getHour() == 0) {
            updateChart("DAILY TRACKER", false);
            controller->advanceDay();
        }

        // Update the total counts on the UI
        getUI()->totalCases->setText(QString::number(controller->getTotalCases()));
        getUI()->totalDeaths->setText(QString::number(controller->getTotalDeaths()));
    }

    // Update the Frame Counter
    numFrames++;
}


//******************************************************************************


void PandemicSimulation::init(std::string type) {
    // Call the base SimpleSimulation init function to create Pandemic locations
    SimpleSimulation::init(type);
}


//******************************************************************************


void PandemicSimulation::generateAgents(int num, bool birth, std::string) {
    // Call the base SimpleSimulation generateAgents function to create
    // Pandemic agents
    SimpleSimulation::generateAgents(num, birth, "Pandemic");
}


//******************************************************************************


void PandemicSimulation::renderChartUpdates(QString which, bool newChartView) {
    int graphView = getChartView(which);
    if (graphView == -1) {
        return;
    }

    if (which == "SEIR") {
        if (newChartView) {
            QtCharts::QChart* chart = SEIRHelper->getChart(getNumSEIR(), getCurrentNumAgents());
            addChartToView(chart, graphView);
        } else {
            SEIRHelper->updateChart(getNumSEIR(), getCurrentNumAgents());
        }
    } else if (which == "DAILY TRACKER") {
        PandemicController* controller = dynamic_cast<PandemicController*>(getController());
        if (newChartView) {
            QtCharts::QChart* chart = dailyTrackerHelper->getChart(controller->getDailyCases(),
                                                                   controller->getDailyDeaths());
            addChartToView(chart, graphView);
        } else {
            dailyTrackerHelper->updateChart(controller->getDailyCases(),
                                            controller->getDailyDeaths());
        }
    } else {
        SimpleSimulation::renderChartUpdates(which, newChartView);
    }
}


//******************************************************************************


std::vector<int> PandemicSimulation::getNumSEIR() {
    return std::vector<int>{numSusceptible, numExposed, numInfected, numRecovered};
}


//******************************************************************************


int PandemicSimulation::getLagPeriod() {
    return this->lagPeriod;
}


//******************************************************************************


int PandemicSimulation::getInitialInfected() {
    return this->initialInfected;
}


//******************************************************************************

void PandemicSimulation::updatePandemicCharts(bool newDay) {
    if (newDay) {
        renderChartUpdates("DAILY TRACKER", false);
    }
    renderChartUpdates("SEIR", false);
}


//******************************************************************************


int PandemicSimulation::getTotalCases() {
    return dynamic_cast<PandemicController*>(getController())->getTotalCases();
}


//******************************************************************************


int PandemicSimulation::getTotalDeaths() {
    return dynamic_cast<PandemicController*>(getController())->getTotalDeaths();
}


//******************************************************************************


PandemicSimulation::~PandemicSimulation() {}


//******************************************************************************


void PandemicSimulation::renderAgentUpdate() {
    Simulation::renderAgentUpdate();

    // Lock the screen and Location vectors
    QMutexLocker locationsLock(getLocationLock());

    // Update the colors of each Location accordingly
    std::vector<Location*> locations = getAllLocations();
    for (size_t i = 0; i < locations.size(); ++i) {
        PandemicLocation* location = dynamic_cast<PandemicLocation*>(locations[i]);

        if (location == nullptr) continue;

        if (location->getStatus() == PandemicLocation::EXPOSURE) {
            location->setColor(PandemicLocation::EXPOSEDCOLOR);
        } else if (location->getStatus() == PandemicLocation::LOCKDOWN) {
            location->setColor(PandemicLocation::LOCKDOWNCOLOR);
        } else {
            location->setColor(PandemicLocation::NORMALCOLOR);
        }
    }

    // Lock the screen queue to synchronize with other threads
    locationsLock.unlock();
    QMutexLocker agentLock(getAgentsLock());

    // Ensure the Simulation wasn't reset while waiting for the Lock
    if (wasReset()) {
        return;
    }

    // Currently only update Agents colors based on their pandemic status
    std::vector<Agent*> agents = getAgents();
    for (size_t i = 0; i < agents.size(); ++i) {
        PandemicAgent* agent = dynamic_cast<PandemicAgent*>(agents[i]);

        PandemicAgent::STATUS status = agent->getStatus();
        if (status == PandemicAgent::SUSCEPTIBLE) {
            agent->setColor(SUSCEPTIBLECOLOR);
        } else if (status == PandemicAgent::EXPOSED) {
            agent->setColor(EXPOSEDCOLOR);
        } else if (status == PandemicAgent::INFECTED) {
            agent->setColor(INFECTEDCOLOR);
        } else if (status == PandemicAgent::RECOVERED) {
            agent->setColor(RECOVEREDCOLOR);
        }
    }
}


//******************************************************************************


void PandemicSimulation::renderCharts(const QString &which, bool newChartView) {

    if (getUI()->headlessMode->checkState() == Qt::CheckState::Checked) {
        return;
    }

    if (which == "ALL") {
        Simulation::renderChartUpdates("AGE", newChartView);
        Simulation::renderChartUpdates("BEHAVIOR", newChartView);
        Simulation::renderChartUpdates("DESTINATION", newChartView);
        renderChartUpdates("SEIR", newChartView);
        renderChartUpdates("DAILY TRACKER", newChartView);
    } else {
        renderChartUpdates(which, newChartView);
    }
}


//******************************************************************************
