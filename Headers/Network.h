#ifndef NETWORK_H
#define NETWORK_H

// Special include for torch
#undef slots
#include "torch/torch.h"
#define slots Q_SLOTS

#include "QString"
#include "QCheckBox"
#include "QDir"
#include "QDebug"
#include "ui_mainwindow.h"

/**
 * @brief The PolicyNetworkImpl struct
 * Struct that contains the Implementation details and parameters for the
 * PolicyNetwork Neural Network class. The PolicyNetwork will take in the
 * state of the Simulation and determine the set of policies to apply to best
 * manage the Pandemic according to the specified objective
 */
struct PolicyNetworkImpl : torch::nn::Module {

    // Constructor
    PolicyNetworkImpl(int networkSize);

    // Forward propogation method
    torch::Tensor forward(torch::Tensor x);

    // Member variable declaration
    torch::nn::Linear linear1{nullptr};
//    torch::nn::Linear linear2{nullptr};
//    torch::nn::Linear linear3{nullptr};
};

// LibTorch Macro to create Module
TORCH_MODULE(PolicyNetwork);


/**
 * @brief saveNetwork \n
 * Function to write the given network out to a file.
 * @param network: the PolicyNetwork to save in a file
 * @param filename: the name of the file to save the network to
 */
void saveNetwork(PolicyNetwork& network, std::string filename, std::string dirname);

/**
 * @brief loadNetwork \n
 * Function to load a network from a given file.
 * @param filename: the name of the file containing a Policy Network
 * @return the network from the file
 */
PolicyNetwork loadNetwork(std::string filename);

/**
 * @brief discountRewards \n
 * Discount the rewards of an episode according to gamma and propogate future
 * rewards back to the initial actions that enabled them
 * @param rewards: the vector of rewards over the entire episode
 * @param gamma: the discount factor for future rewards
 */
void discountRewards(std::vector<double> &rewards, double gamma = 0.999);

/**
 * @brief calculateReward \n
 * Calculate the reward of the current state according to the optimization
 * policy.
 * @param state: the current state of the Simulation
 * @param policy: the optimization policy, either "Pandemic", "Economic", or "Dual"
 * @return the reward for the current state
 */
double calculateReward(torch::Tensor state, QString policy);

/**
 * @brief calculateLoss \n
 * Function to calculate the discounted loss of the entire episode
 * @param network: the network to be trained
 * @param states: the states of the Simulation
 * @param actions: the actions applied throughout the Simulation
 * @return the loss as a Tensor
 */
torch::Tensor calculateLoss(PolicyNetwork network, torch::Tensor states,
                            torch::Tensor actions, torch::Tensor rewards);

/**
 * @brief applyPolicies \n
 * Function to choose and apply the policies based on the output of the network.
 * @param probabilities: output of the network
 * @param indexPolicyMapping: mapping of the output indexes to the policies
 */
torch::Tensor applyPolicies(torch::Tensor probabilities,
                   std::unordered_map<int,QCheckBox*> &indexPolicyMapping);

/**
 * @brief tensorToString \n
 * Function to convert a Tensor to a std::string to they may be printed out.
 * @param tensor
 * @return
 */
std::string tensorToString(torch::Tensor tensor);


/**
 * @brief evaluateNetwork \n
 * Evaluate the performance of an already trained network on multiple runs.
 * @param network
 * @param ui
 * @param episodeLength
 * @param numEpisodes
 */
void evaluateNetwork(PolicyNetwork network, Ui::MainWindow* ui, int episodeLength = 50,
                     int numEpisodes = 100);


#endif // NETWORK_H
