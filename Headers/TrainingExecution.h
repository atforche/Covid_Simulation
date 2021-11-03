#ifndef TRAININGEXECUTION_H
#define TRAININGEXECUTION_H

#include "windows.h"
#include "QObject"
#include "QThread"
#include "QTimer"

#include "Simulation.h"
#include "Network.h"
#include "mainwindow.h"

// Special include for torch
#undef slots
#include "torch/torch.h"
#define slots Q_SLOTS

// Forward Declarations
class DualSimulation;

/**
 * @brief The TrainingWorker class \n
 * Class that handles the execution of training on a thread. A Worker
 * is dispatched by a TrainingController
 */
class TrainingWorker : public QObject {
    Q_OBJECT

public:

    /**
     * @brief TrainingWorker \n
     * Constructor for the TrainingWorker class. A TrainingWorker executes the
     * training of a Neural Network to control the Simulation
     */
    TrainingWorker(Ui::MainWindow* ui);

public slots:

    /**
     * @brief evaluateEpisode \n
     * Slot that handled the evaluation of one episode of the Simulation.
     */
    void evaluateEpisode(int episodeLength, DualSimulation* sim,
                         PolicyNetwork* network, QString policy = "Pandemic",
                         bool visualize = false);

signals:

    /** Signal to indicate to the Controller that the current episode has finished */
    void episodeFinished(torch::Tensor states, torch::Tensor output,
                         torch::Tensor policies, torch::Tensor rewards);

    /** Signal to indicate that the Training function wants to print a message */
    void printMessage(const QString &message);

    /** Signal to try and force the Window to update*/
    void forceUpdate();

private:

    // Set up the Index to Policy Mapping
    std::unordered_map<int, QCheckBox*> indexPolicyMapping;

};


//******************************************************************************


/**
 * @brief The TrainingController class \n
 * Class that controls the Training of the network and delegates it to another
 * thread
 */
class TrainingController : public QObject {
    Q_OBJECT

private:

    /** Second thread on which the simulation will be executed*/
    QThread workerThread;

    /** UI the controller is controlling */
    Ui::MainWindow* ui;

    /** Pointer to the worker controlling execution on the second thread */
    TrainingWorker* worker;

    /** Pointer to the Simulation that drives the training */
    DualSimulation* sim;

    /** Pointer to the Network controlling the decisions */
    PolicyNetwork network;

    /** Weights of the best network so far */
    PolicyNetwork optimalNetwork;

    /** Loss of the best network so far */
    double optimalLoss;

    /** Current Episode that the training is on */
    int currentEpisode;

    /** Rewards for every episode run */
    std::vector<double> episodeLosses;

    /** Optimizer for the Network */
    torch::optim::Adam optimizer;

    /** Length of each Episode to be run */
    int episodeLength;

    /** Total number of episodes to run */
    int numEpisodes;

    /** Reward rules */
    QString policy;

public:

    /** Device that all tensors should be used on */
    static torch::Device tensorDevice;

    /**
     * @brief TrainingController \n
     * Constructor for the Training Controller class
     */
    TrainingController(Ui::MainWindow* ui, MainWindow* window,
                       int episodeLength, int numEpisodes,
                       int networkSize);

    /**
     * @brief startTraining \n
     * Function to emit a signal to the TrainingWorker thread to begin
     * execution of the Simulation on a separate thread
     */
    void trainNetwork(bool visualize);

    /**
     * @brief loadNetwork \n
     * Function that enables the user to load a network continue training.
     * @param network: the as loaded in from file
     * @param currentStep: the last step at which the network was trained
     */
    void loadNetwork(PolicyNetwork network, int currentStep);

public slots:

    /**
     * @brief processEpisodeResults \n
     * Function that processes and updates the network according to the
     * results of the most recent episode.
     * @param states: the Tensor of states from the episode
     * @param output: the Tensor of network output from the episode
     * @param policies: the Tensor of policies chosen throughout the episode
     * @param rewards: the Tensor of rewards from the episode
     */
    void processEpisodeResults(torch::Tensor states, torch::Tensor output,
                               torch::Tensor policies, torch::Tensor rewards);

signals:


     /** Signal that enables the SimulationWorker to begin performing  */
    void evaluateEpisode(int episodeLength, DualSimulation* sim, PolicyNetwork* network,
                         QString policy = "Pandemic", bool visualize = false);

    /** Signal to indicate that the Training function wants to print a message */
    void printMessage(const QString &message);

    /** Signal to indicate that the training process has finished */
    void trainingFinished();

};

#endif // TRAININGEXECUTION_H
