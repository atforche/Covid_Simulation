#include "Headers/AgentController.h"

QString AgentController::evaluateDestinationProbabilities(QStringList &keys,
                                                          std::vector<double> &probabilities) {
    // Generate a cumulative weighted total for each key
    int weightedSum = 0;
    for (size_t i = 0; i < probabilities.size(); ++i) {
        probabilities[i] = 100 * probabilities[i] + weightedSum;
        weightedSum = probabilities[i];
    }

    // Randomly sample a value in [0,weightedSum). Return the first key for
    // which this random value is less than the weighted sum
    int random = rand() % weightedSum;
    for (size_t i = 0; i < probabilities.size(); ++i) {
        if (random < probabilities[i]) {
            return keys.at(static_cast<int>(i));
        }
    }

    // Default return case
    return keys.at(0);
}


//******************************************************************************


QJsonValue AgentController::readAssignmentFromJSON(int behavior,
                                                   int hour, bool adult) {
    // Initialize local variables
    QJsonValue destination;
    QJsonObject::iterator key;

    if (adult) {
        // Check if current hour exists in the specific adult chart
        key = adultBehaviors[behavior].find(
                    QString::number(hour));

        // If it does, update the agent's destination to the new location
        if (key != adultBehaviors[behavior].end()) {
            destination = key.value();
        } else {
            destination = QJsonValue("No Change");
        }

    } else {
        // Check if current hour exists in the specific child chart
        key = childBehaviors[behavior].find(
                    QString::number(hour));

        // If it does, update the agent's destination to the new location
        if (key != childBehaviors[behavior].end()) {
            destination = key.value();
        } else {
            destination = QJsonValue("No Change");
        }
    }

    // Return the updated destination for the agent
    return destination;
}


//******************************************************************************


QString AgentController::getAgentDestination(Agent *agent, int hour) {

    // Initialize local variables
    QString destinationString;

    // Get the agents behavior assignment and whether they are an adult
    int behavior = agent->getBehavior();
    bool adult = agent->isAdult();

    // Pull the Agent's destination from the appriopriate behavior chart
    QJsonValue destination = readAssignmentFromJSON(behavior, hour,
                                                    adult);

    // Destination can either be a JSON string or object
    if (destination.isString()) {
        // A JSON string just contains the destination in text
        destinationString = destination.toString();
    } else {
        // A JSON object has set of locations and probabilities. Grab the locations
        // and their probabilities
        QJsonObject object = destination.toObject();
        QStringList keys = object.keys();
        std::vector<double> probabilities(keys.size());
        for (int i = 0; i < keys.size(); ++i) {
            probabilities[i] = object.value(keys[i]).toDouble();
        }

        // Use weighted select to pick the next destination
        destinationString = evaluateDestinationProbabilities(keys,
                                                             probabilities);
    }

    // Return the new destination
    return destinationString;
}


//******************************************************************************


AgentController::AgentController() {

    // Initialize variables
    QJsonObject::iterator probability;
    adultWeightedSum = 0;
    childWeightedSum = 0;

    // Read in the behaviors from the bin and filter into adult and child behaviors
    QDir directory(":/bin/behaviors");
    QStringList behaviors = directory.entryList();
    QStringList adult = behaviors.filter("adult_");
    QStringList child = behaviors.filter("child_");

    // Default initialize the behavior and probability vectors
    this->adultBehaviors = std::vector<QJsonObject>(adult.size());
    this->adultProbabilities = std::vector<int>(adult.size());
    this->childBehaviors = std::vector<QJsonObject>(child.size());
    this->childProbabilities = std::vector<int>(child.size());

    // Read in the JSON for each adult behavior
    QFile file;
    for (int i = 0; i < adult.size(); ++i) {

        // Select a behavior file from the directory
        file.setFileName(QString::fromStdString(":/bin/behaviors/") +=
                adult.at(i));

        // Open the file, read in the contents, and close the file
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString val = file.readAll();
        file.close();

        // Populate the vector with the JSON object from the file
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        adultBehaviors[i] = doc.object();

        // Read in the probability and populate the Probabilites vector
        probability = adultBehaviors[i].find("Probability");
        adultWeightedSum += 100 * probability.value().toDouble();
        adultProbabilities[i] = adultWeightedSum;
    }

    // Read in the JSON for each child behavior
    for (int i = 0; i < child.size(); ++i) {

        // Select a behavior file from the directory
        file.setFileName(QString::fromStdString(":/bin/behaviors/") +=
                child.at(i));

        // Open the file, read in the contents, and close the file
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString val = file.readAll();
        file.close();

        // Populate the vector with the JSON object from the file
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        childBehaviors[i] = doc.object();

        // Read in the probability and population the Probabilities vector
        probability = childBehaviors[i].find("Probability");
        childWeightedSum += 100 * probability.value().toDouble();
        childProbabilities[i] = childWeightedSum;
    }
}


//******************************************************************************


int AgentController::getNumAdultBehaviors() {
    return static_cast<int>(adultBehaviors.size());
}


//******************************************************************************


int AgentController::getAdultBehavior() {
    // Generate a random number in the range [0, adultWeightedSum)
    int random_num = rand() % adultWeightedSum;

    for (size_t i = 0; i < adultProbabilities.size(); ++i) {
        if (random_num < adultProbabilities[i]) {
            return static_cast<int>(i);
        }
    }

    return static_cast<int>(adultProbabilities.size() - 1);
}


//******************************************************************************


int AgentController::getNumChildBehaviors() {
    return static_cast<int>(childBehaviors.size());
}


//******************************************************************************


int AgentController::getChildBehavior() {
    // Generate a random number in the range [0, adultWeightedSum)
    int random_num = rand() % childWeightedSum;

    for (size_t i = 0; i < childProbabilities.size(); ++i) {
        if (random_num < childProbabilities[i]) {
            return static_cast<int>(i);
        }
    }

    return static_cast<int>(childProbabilities.size() - 1);
}


//******************************************************************************


void AgentController::updateAgentDestinations(std::vector<Agent *> &agents, int hour) {

    // Loop through every agent
    for (size_t i = 0; i < agents.size(); ++i) {

        // Determine to where the agent will be assigned
        QString destinationString = getAgentDestination(agents[i], hour);

        // Updates the Agent's destination
        if (destinationString == "No Change") {
            continue;
        } else if (destinationString == "Home") {
            agents[i]->setDestination(agents[i]->getLocation(Agent::HOME));
        } else if (destinationString == "School") {
            agents[i]->setDestination(agents[i]->getLocation(Agent::SCHOOL));
        } else if (destinationString == "Work") {
            agents[i]->setDestination(agents[i]->getLocation(Agent::WORK));
        } else if (destinationString == "Leisure") {
            agents[i]->setDestination(agents[i]->getLocation(Agent::LEISURE));
        } else {
            // Throw an exception if an invalid behavior is loaded
            throw "Invalid Behavior File Loaded";
        }
    }
}


//******************************************************************************


QString AgentController::getStartingDestination(int behaviorChart,
                                               bool isAdult) {
    // Generate an arbitrary age depending on whether the agent is an adult
    int age = isAdult ? 50 : 10;

    // Create a dummy agent with the specified behavior chart
    Location temp2(0,0);
    Agent temp(age, &temp2, behaviorChart);

    // Return the destination assignment of the agent, which is its starting location
    return getAgentDestination(&temp, 0);
}


//******************************************************************************


int AgentController::sampleAgentAge() {

    // Hardcode the ages and probabilities associated with each age
    static std::vector<int> ages = {0, 18, 24, 44, 64, 100};
    static std::vector<int> probabilities = {25, 10, 30, 22, 13};

    // Hardcode the weighted sum for each age
    int weightedTotal = 100;
    static std::vector<int> ageWeightedSums = {25, 35, 65, 87, 100};

    // Randomly sample a number in the range of [0, weightedTotal)
    int randNum = rand() % weightedTotal;

    // Find the first weighted sum that is greater than the randNum
    int index = 0;
    for (size_t i = 0; i < ageWeightedSums.size(); ++i) {
        if (randNum < ageWeightedSums[i]) {
            index = static_cast<int>(i);
            break;
        }
    }

    // Randomly sample an age from within the sampled age range
    int rangeSize = ages[index + 1] - ages[index];
    randNum = rand() % rangeSize;
    return ages[index] + randNum;
}

