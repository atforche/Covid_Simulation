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

    // Create a controller for the economy
    this->economicController = new EconomicController(this);

}


//******************************************************************************


void EconomicSimulation::execute() {

    // Static counter to determine if an hour has passed
    static int numFrames = 0;

    // Run the Simple Simulation's execute function
    SimpleSimulation::execute();

    // Run the economic update and grab the total Agent and Business value
    int* totalValues = economicController->executeEconomicUpdate();

    // Update the overall values
    this->businessEconomicValue = totalValues[1];
    this->totalEconomicValue = totalValues[0] + totalValues[1];
    delete totalValues;

    // Update the current value displayed on the screen
    getUI()->currentValue->setText(QString::number(totalEconomicValue));

    // Update the Frame Counter
    numFrames++;
    if (numFrames == FRAMES_PER_HOUR) {
        numFrames = 0;

        // Update the economic value in each chart
        updateEconomicCharts();
    }
}


//******************************************************************************


void EconomicSimulation::init(std::string type) {
    // Run the base init function
    SimpleSimulation::init("Economic");

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

}


//******************************************************************************


void EconomicSimulation::generateAgents(int num, bool birth, std::string type) {
    SimpleSimulation::generateAgents(num, birth, "Economic");
    qDebug() << "Economic Agents Generated";

    // Grab all the agents
    QMutexLocker lock(getAgentsLock());
    std::vector<Agent*> agents = getAgents();

    // Distribute economic value to agents based on their age
    if (!birth) {

        // Agents start with 50% of the total value
        int agentValue = totalEconomicValue / 2;

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
            } else if (age < 45) {
                agent->incrementValue(agentValue * 0.0906 / ageCounts[2]);
            } else if (age < 55) {
                agent->incrementValue(agentValue * 0.1673 / ageCounts[3]);
            } else if (age < 65) {
                agent->incrementValue(agentValue * 0.2109 / ageCounts[4]);
            } else if (age < 75) {
                agent->incrementValue(agentValue * 0.2644 / ageCounts[5]);
            } else {
                agent->incrementValue(agentValue * 0.2529 / ageCounts[6]);
            }
        }

        // Update the overall Agent value total
        this->agentEconomicValue = agentValue;
    }

}


//******************************************************************************


int EconomicSimulation::getCurrentValue() {
    return this->totalEconomicValue;
}


//******************************************************************************


void EconomicSimulation::renderChartUpdates(QString which, bool newChartView) {
    int graphView = getChartView(which);
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
    }
}


//******************************************************************************


void EconomicSimulation::updateEconomicCharts() {
    renderCharts("AGENT VALUE", false);
    renderCharts("BUSINESS VALUE", false);
    renderCharts("TOTAL VALUE", false);
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
    if (which == "ALL") {
        Simulation::renderChartUpdates("AGE", newChartView);
        Simulation::renderChartUpdates("BEHAVIOR", newChartView);
        Simulation::renderChartUpdates("DESTINATION", newChartView);
        renderChartUpdates("AGENT VALUE", newChartView);
        renderChartUpdates("BUSINESS VALUE", newChartView);
        renderChartUpdates("TOTAL VALUE", newChartView);
    } else {
        renderChartUpdates(which, newChartView);
    }
 }
