#include "Headers/EconomicSimulation.h"

EconomicSimulation::EconomicSimulation(int initialValue, int numAgents,
                                       Ui::MainWindow* ui,
                                       std::map<std::string, bool> debug) :
    SimpleSimulation(numAgents, ui, debug) {

    // Give the simulation some initial economic value
    this->totalEconomicValue = initialValue;

    // Initialize the Chart Helpers
    agentValueHelper = new AgentValueChartHelper(initialValue);
    workValueHelper = new WorkValueChartHelper(initialValue);
    totalValueHelper = new TotalValueChartHelper();
    statusHelper = new EconomicStatusChartHelper();

    // Create a controller for the economy
    setAgentController(new EconomicController(this));
}


//******************************************************************************


void EconomicSimulation::execute() {

    // Static counter to determine if an hour has passed
    static int numFrames = 0;

    // Run the Simple Simulation's execute function
    SimpleSimulation::execute();

    // Update the overall values
    EconomicController* econController = dynamic_cast<EconomicController*>(getController());
    this->businessEconomicValue = econController->getTotalBusinessValue();
    this->totalEconomicValue = businessEconomicValue + econController->getTotalAgentValue();

    // Update the current value displayed on the screen
    getUI()->currentValue->setText(QString::number(totalEconomicValue));

    // Every hour update the Economic Charts
    if (numFrames == FRAMES_PER_HOUR) {
        numFrames = 0;
        // Update the economic value in each chart
        updateEconomicCharts();
    }
    // Update the Frame Counter
    numFrames++;
}


//******************************************************************************


void EconomicSimulation::init(std::string type) {
    // Run the base init function
    SimpleSimulation::init(type);

    // Create sibling pairs between Work and Leisure locations
    std::vector<Location*> workLocations = getRegion(Agent::WORK)->getLocations();
    std::vector<Location*> leisureLocations = getRegion(Agent::LEISURE)->getLocations();

    // Distribute a portion economic value equally to Work Locations
    double businessValue = totalEconomicValue / 2;

    for (size_t i = 0; i < workLocations.size(); ++i) {
        // Cast the base Location pointers to EconomicLocation pointers
        EconomicLocation* workLocation = dynamic_cast<EconomicLocation*>(workLocations[i]);
        EconomicLocation* leisureLocation = dynamic_cast<EconomicLocation*>(leisureLocations[i]);

        // Establish the sibling relationship between them
        workLocation->setSibling(leisureLocation);
        leisureLocation->setSibling(workLocation);

        // Give the business a certain portion of the Sim's total Economic Value
        workLocation->incrementValue(businessValue / workLocations.size());
    }

    // Initialize the total value for businesses
    businessEconomicValue = businessValue;

    // Update the totalEconomicValue now that the business and agent values
    // have been calculated
    this->totalEconomicValue = this->agentEconomicValue + this->businessEconomicValue;

    // Initialize the homeless shelter location
    Coordinate newPosition = getRegion(Agent::HOME)->getRandomCoordinate();
    Location* newHomelessShelter = new Location(newPosition.getCoord(Coordinate::X),
                                                newPosition.getCoord(Coordinate::Y));
    setHomelessShelter(newHomelessShelter);

    // Add the homeless shelter to the screen
    addToScreen(newHomelessShelter->getGraphicsObject());
}


//******************************************************************************


void EconomicSimulation::generateAgents(int num, bool birth, std::string type) {
    SimpleSimulation::generateAgents(num, birth, type);

    // Grab all the agents
    QMutexLocker lock(getAgentsLock());
    std::vector<Agent*> agents = getAgents();

    // Distribute economic value to agents based on their age
    if (!birth) {

        // Agents start with 50% of the total value
        int agentValue = totalEconomicValue / 2;
        int actualAgentValue = 0;

        // Count how many agents fall in each age group
        std::vector<int> ageCounts(7, 0);
        for (size_t i = 0; i < agents.size(); ++i) {
            int age = agents[i]->getAge();
            if (age < 18) {
                ageCounts[0] += 1;
            } else if (age < 35) {
                ageCounts[1] += 1;
            } else if (age < 45) {
                ageCounts[2] += 1;
            } else if (age < 55) {
                ageCounts[3] += 1;
            } else if (age < 65) {
                ageCounts[4] += 1;
            } else if (age < 75) {
                ageCounts[5] += 1;
            } else {
                ageCounts[6] += 1;
            }
        }

        // Now distribute money equally among agents in each age bracket
        for (size_t i = 0; i < agents.size(); ++i) {
            EconomicAgent* agent = dynamic_cast<EconomicAgent*>(agents[i]);
            int age = agent->getAge();
            if (age < 18) {
                agent->incrementValue(0);
            } else if (age < 35) {
                agent->incrementValue(agentValue * 0.0137 / ageCounts[1]);
                actualAgentValue += agentValue * 0.0137 / ageCounts[1];
            } else if (age < 45) {
                agent->incrementValue(agentValue * 0.0906 / ageCounts[2]);
                actualAgentValue += agentValue * 0.0906 / ageCounts[2];
            } else if (age < 55) {
                agent->incrementValue(agentValue * 0.1673 / ageCounts[3]);
                actualAgentValue += agentValue * 0.1673 / ageCounts[3];
            } else if (age < 65) {
                agent->incrementValue(agentValue * 0.2109 / ageCounts[4]);
                actualAgentValue += agentValue * 0.2109 / ageCounts[4];
            } else if (age < 75) {
                agent->incrementValue(agentValue * 0.2644 / ageCounts[5]);
                actualAgentValue += agentValue * 0.2644 / ageCounts[5];
            } else {
                agent->incrementValue(agentValue * 0.2529 / ageCounts[6]);
                actualAgentValue += agentValue * 0.2529 / ageCounts[6];
            }
        }

        // Update the overall Agent value total
        this->agentEconomicValue = actualAgentValue;
    }

}


//******************************************************************************


int EconomicSimulation::getCurrentValue() {
    return this->totalEconomicValue;
}


//******************************************************************************


void EconomicSimulation::renderChartUpdates(QString which, bool newChartView) {
    int graphView = getChartView(which);
    std::string which_std = which.toStdString();
    if (graphView == -1) {
        return;
    }

    if (which == "AGENT VALUE") {
        if (newChartView) {
            QtCharts::QChart* chart = agentValueHelper->getChart(&getAgents());
            addChartToView(chart, graphView);
        } else {
            agentValueHelper->updateChart(&getAgents());
        }
    } else if (which == "BUSINESS VALUE") {
        if (newChartView) {
            QtCharts::QChart* chart = workValueHelper->getChart(&getRegion(Agent::WORK)->getLocations());
            addChartToView(chart, graphView);
        } else {
            workValueHelper->updateChart(&getRegion(Agent::WORK)->getLocations());
        }
    } else if (which == "TOTAL VALUE") {
        if (newChartView) {
            QtCharts::QChart* chart = totalValueHelper->getChart(totalEconomicValue);
            addChartToView(chart, graphView);
        } else {
            totalValueHelper->updateChart(totalEconomicValue);
        }
    } else if (which == "ECONOMIC STATUS") {
        if (newChartView) {
            QtCharts::QChart* chart = statusHelper->getChart(getNumHomelessAgents(),
                                                             getNumUnemployedAgents(),
                                                             static_cast<int>(getAgents().size()));
            addChartToView(chart, graphView);
        } else {
            statusHelper->updateChart(getNumHomelessAgents(),
                                      getNumUnemployedAgents(),
                                      static_cast<int>(getAgents().size()));
        }
    } else {
        SimpleSimulation::renderChartUpdates(which, newChartView);
    }
}


//******************************************************************************


void EconomicSimulation::updateEconomicCharts() {
    renderCharts("AGENT VALUE", false);
    renderCharts("BUSINESS VALUE", false);
    renderCharts("TOTAL VALUE", false);
    renderCharts("ECONOMIC STATUS", false);
}


//******************************************************************************


std::vector<int>& EconomicSimulation::getBusinessThresholds() {
    return workValueHelper->getThresholds();
}


//******************************************************************************


int EconomicSimulation::getNumHomelessAgents() {

    std::vector<Agent*> agents = getAgents();
    double homelessCount = 0;

    // Count each agent that is homeless
    for (size_t i = 0; i < agents.size(); ++i) {
        EconomicAgent* agent = dynamic_cast<EconomicAgent*>(agents[i]);
        if (agent->getStatus() == EconomicAgent::BOTH) {
            homelessCount++;
        } else if (agent->getStatus() == EconomicAgent::HOMELESS) {
            homelessCount++;
        }
    }

    return homelessCount;
}


//******************************************************************************


std::vector<Agent*> EconomicSimulation::getHomelessAgents() {
    std::vector<Agent*> retVal;
    std::vector<Agent*> agents = getAgents();
    for (size_t i = 0; i < agents.size(); ++i) {
        EconomicAgent* agent = dynamic_cast<EconomicAgent*>(agents[i]);
        if (agent->getStatus() == EconomicAgent::BOTH || agent->getStatus() == EconomicAgent::HOMELESS) {
            retVal.push_back(agent);
        }
    }

    return retVal;
}


//******************************************************************************


int EconomicSimulation::getNumUnemployedAgents() {

    std::vector<Agent*> agents = getAgents();
    double unemployedCount = 0;

    // Count each agent that is homeless
    for (size_t i = 0; i < agents.size(); ++i) {
        EconomicAgent* agent = dynamic_cast<EconomicAgent*>(agents[i]);
        if (agent->getStatus() == EconomicAgent::BOTH) {
            unemployedCount++;
        } else if (agent->getStatus() == EconomicAgent::UNEMPLOYED) {
            unemployedCount++;
        }
    }

    return unemployedCount;
}


//******************************************************************************


std::vector<Agent*> EconomicSimulation::getUnemployedAgents() {
    std::vector<Agent*> retVal;
    std::vector<Agent*> agents = getAgents();
    for (size_t i = 0; i < agents.size(); ++i) {
        EconomicAgent* agent = dynamic_cast<EconomicAgent*>(agents[i]);
        if (agent->getStatus() == EconomicAgent::BOTH || agent->getStatus() == EconomicAgent::UNEMPLOYED) {
            retVal.push_back(agent);
        }
    }

    return retVal;
}


//******************************************************************************


void EconomicSimulation::setBusinessEconomicValue(double newValue) {
    this->businessEconomicValue = newValue;
}


//******************************************************************************


void EconomicSimulation::setTotalEconomicValue(double newValue) {
    this->totalEconomicValue = newValue;
}


//******************************************************************************


double EconomicSimulation::getTotalEconomicValue() {
    return this->totalEconomicValue;
}


//******************************************************************************


EconomicSimulation::~EconomicSimulation() {

    // Remove any memory traces from the chartViews
    getUI()->graphView1->setChart(new QtCharts::QChart());
    getUI()->graphView2->setChart(new QtCharts::QChart());
    getUI()->graphView3->setChart(new QtCharts::QChart());

    // Delete the chart helpers
    delete agentValueHelper;
    delete workValueHelper;
    delete totalValueHelper;
}


//******************************************************************************


void EconomicSimulation::renderAgentUpdate() {
    Simulation::renderAgentUpdate();

    // Lock the screen queue to synchronize with other threads
    QMutexLocker locationLock(getLocationLock());

    // Ensure the Simulation wasn't reset while waiting for the Lock
    if (wasReset()) {
        return;
    }

    const static std::vector<int> businessThresholds = workValueHelper->getThresholds();
    const static std::vector<int> agentThresholds = agentValueHelper->getThresholds();

    // Update the colors of the Locations based on their current value
    std::vector<Location*> workLocations = getRegion(Agent::WORK)->getLocations();
    for (size_t i = 0; i < workLocations.size(); ++i) {
        // Cast the pointer to an Economic Location*
        EconomicLocation* workLocation = dynamic_cast<EconomicLocation*>(workLocations[i]);
        int locationValue = workLocation->getValue();

        // Determine the appriopriate color from the value
        QColor color;
        if (locationValue == businessThresholds[0]) {
            color = QColor(255, 0, 0);
        } else if (locationValue < businessThresholds[1]) {
            color = QColor(235, 143, 52);
        } else if (locationValue < businessThresholds[2]) {
            color = QColor(235, 220, 52);
        } else if (locationValue < businessThresholds[3]) {
            color = QColor(186, 235, 52);
        } else {
            color = QColor(27, 163, 3);
        }

        // Assign the color to both the Work and Leisure location
        workLocation->setColor(color);
        workLocation->getSibling()->setColor(color);
    }

    // Unlock the Locations lock
    locationLock.unlock();

    // Update the colors of the Agents based on their current value
    QMutexLocker lock(getAgentsLock());
    std::vector<Agent*> agents = getAgents();
    for (size_t i = 0; i < agents.size(); ++i) {
        // Cast the agent to an Economic Agent
        EconomicAgent* agent = dynamic_cast<EconomicAgent*>(agents[i]);
        int agentValue = agent->getValue();

        // Determine the appriopriate color from the value
        QColor color;
        if (agentValue == agentThresholds[0]) {
            color = QColor(255, 0, 0);
        } else if (agentValue < agentThresholds[1]) {
            color = QColor(235, 143, 52);
        } else if (agentValue < agentThresholds[2]) {
            color = QColor(235, 220, 52);
        } else if (agentValue < agentThresholds[3]) {
            color = QColor(186, 235, 52);
        } else {
            color = QColor(27, 163, 3);
        }

        // Update the agents color
        agent->setColor(color);
    }
}


//******************************************************************************


void EconomicSimulation::renderCharts(const QString &which, bool newChartView) {

    if (getUI()->headlessMode->checkState() == Qt::CheckState::Checked) {
        return;
    }

    if (which == "ALL") {
        Simulation::renderChartUpdates("AGE", newChartView);
        Simulation::renderChartUpdates("BEHAVIOR", newChartView);
        Simulation::renderChartUpdates("DESTINATION", newChartView);
        renderChartUpdates("AGENT VALUE", newChartView);
        renderChartUpdates("BUSINESS VALUE", newChartView);
        renderChartUpdates("TOTAL VALUE", newChartView);
        renderChartUpdates("ECONOMIC STATUS", newChartView);
    } else {
        renderChartUpdates(which, newChartView);
    }
 }
