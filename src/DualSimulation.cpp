#include "Headers/DualSimulation.h"

DualSimulation::DualSimulation(int lagPeriod, int initialInfected, int initialValue,
                               int numAgents, Ui::MainWindow* ui,
                               std::map<std::string, bool> debug) :
    PandemicSimulation(lagPeriod, initialInfected, initialValue, numAgents, ui, debug) {

    // Update the Agent Controller to a Dual Controller
    PandemicSimulation::setAgentController(new DualController(static_cast<PandemicSimulation*>(this)));
}


//******************************************************************************


void DualSimulation::execute() {
    // Static counter to determine if an hour has passed
    static int numFrames = 0;

    // Run the Pandemic Simulation's execute function
    PandemicSimulation::execute();

    // Update the overall values
    EconomicController* econController = dynamic_cast<EconomicController*>(getController());
    setBusinessEconomicValue(econController->getTotalBusinessValue());
    setTotalEconomicValue(econController->getTotalBusinessValue() + econController->getTotalAgentValue());

    // Update the current value displayed on the screen
    getUI()->currentValue->setText(QString::number(getTotalEconomicValue()));

    // Every hour update the Economic Charts
    if (numFrames == FRAMES_PER_HOUR) {
        numFrames = 0;
        // Update the economic value in each chart
        updateEconomicCharts();
    }
    // Update the Frame Counter
    numFrames++;
}


//***************************************************************`***************


void DualSimulation::init(std::string type) {
    // EconomicSimulation::init takes care of everything needed for
    // the Dual simulation
    EconomicSimulation::init(type);
}


//******************************************************************************


void DualSimulation::generateAgents(int num, bool birth, std::string) {
    // EconomicSimulation::generateAgents takes care of everything needed
    // for the Dual Simulation
    EconomicSimulation::generateAgents(num, birth, "Dual");
}


//******************************************************************************


void DualSimulation::renderChartUpdates(QString which, bool newChartView) {
    if (which == "SEIR" || which == "DAILY TRACKER") {
        PandemicSimulation::renderChartUpdates(which, newChartView);
    } else {
        EconomicSimulation::renderChartUpdates(which, newChartView);
    }
}


//******************************************************************************


void DualSimulation::renderAgentUpdate() {
    // Render and color the agents depending on the selection made by the User
    if (PandemicSimulation::checkDebug("pandemic agent coloring")) {
        PandemicSimulation::renderAgentUpdate();
    } else {
        EconomicSimulation::renderAgentUpdate();
    }
}


//******************************************************************************


void DualSimulation::renderCharts(const QString &which, bool newChartView) {
    if (which == "ALL") {
        PandemicSimulation::renderChartUpdates("AGE", newChartView);
        PandemicSimulation::renderChartUpdates("BEHAVIOR", newChartView);
        PandemicSimulation::renderChartUpdates("DESTINATION", newChartView);
        EconomicSimulation::renderChartUpdates("AGENT VALUE", newChartView);
        EconomicSimulation::renderChartUpdates("BUSINESS VALUE", newChartView);
        EconomicSimulation::renderChartUpdates("TOTAL VALUE", newChartView);
        EconomicSimulation::renderChartUpdates("ECONOMIC STATUS", newChartView);
        PandemicSimulation::renderChartUpdates("SEIR", newChartView);
        PandemicSimulation::renderChartUpdates("DAILY TRACKER", newChartView);
    } else {
        renderChartUpdates(which, newChartView);
    }
}


//******************************************************************************
