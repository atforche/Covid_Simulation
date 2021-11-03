#include "Headers/DualSimulation.h"
#include "Headers/Network.h"
#include "Headers/TrainingExecution.h"

DualSimulation::DualSimulation(int lagPeriod, int initialInfected, int initialValue,
                               int numAgents, Ui::MainWindow* ui,
                               std::map<std::string, bool> debug, PolicyNetwork* network) :
    PandemicSimulation(lagPeriod, initialInfected, initialValue, numAgents, ui, debug),
    indexPolicyMapping({
                       {0, ui->quarantineWhenInfected},
                       {1, ui->totalLockdown},
                       {2, ui->weak},
                       {3, ui->moderate},
                       {4, ui->strong},
                       {5, ui->weakLockdown},
                       {6, ui->moderateLockdown},
                       {7, ui->strongLockdown},
                       {8, ui->weakAssistance},
                       {9, ui->moderateAssistance},
                       {10, ui->strongAssistance},
                       {11, ui->weakGuidelines},
                       {12, ui->moderateGuidelines},
                       {13, ui->strongGuidelines},
                       {14, ui->weakCommerce},
                       {15, ui->moderateCommerce},
                       {16, ui->strongCommerce}
                   }) {

    // Update the Agent Controller to a Dual Controller
    PandemicSimulation::setAgentController(new DualController(static_cast<PandemicSimulation*>(this)));

    this->network = network;
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

        // If a network is provided, grab the state and update the Policies
        if (network != nullptr) {
            torch::Tensor state = getState().to(TrainingController::tensorDevice);
            torch::Tensor probabilities = (*network)->forward(state);
            torch::Tensor policies = applyPolicies(probabilities, this->indexPolicyMapping);
        }
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


torch::Tensor DualSimulation::getState() {
    // Initialize variables
    const int inputSize = 12;
    torch::Tensor output = torch::zeros({inputSize});

    std::vector<double> economicState = dynamic_cast<EconomicController*>(getController())->getState();
    std::vector<double> totalState = dynamic_cast<PandemicController*>(getController())->getState();
    totalState.insert(totalState.end(), economicState.begin(), economicState.end());

    torch::Tensor retVal = torch::tensor(totalState, torch::requires_grad());
    retVal.to(TrainingController::tensorDevice);
    return retVal;
}


//******************************************************************************


bool DualSimulation::takeStep(int episodeLength) {
    for (int i = 0; i < FRAMES_PER_HOUR; ++i) {
        execute();
    }

    if (getDay() == episodeLength) {
        return true;
    }
    return false;
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

    if (getUI()->headlessMode->checkState() == Qt::CheckState::Checked) {
        return;
    }

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
