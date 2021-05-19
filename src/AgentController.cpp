#include "Headers/AgentController.h"

AgentController::AgentController() {

    // Read in the behaviors from the bin and filter into adult and child behaviors
    QDir directory(":/bin/behaviors");
    QStringList behaviors = directory.entryList();
    QStringList adult = behaviors.filter("adult_");
    QStringList child = behaviors.filter("child_");

    // Default initialize the behavior vectors
    this->adultBehaviors = std::vector<QJsonObject>(adult.size());
    this->childBehaviors = std::vector<QJsonObject>(child.size());

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
    }
}


int AgentController::getNumAdultBehaviors() {
    return static_cast<int>(adultBehaviors.size());
}


int AgentController::getNumChildBehaviors() {
    return static_cast<int>(childBehaviors.size());
}

void AgentController::updateAgentDestination(std::vector<Agent *> agents, int hour) {

    // Loop through every agent
    for (size_t i = 0; i < agents.size(); ++i) {

        // Determine to where the agent will be assigned
        QString destinationString = getDestinationAssignment(agents[i], hour);

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


QString AgentController::getStartingDestination(int behaviorChart,
                                               bool isAdult) {
    // Generate an arbitrary age depending on whether the agent is an adult
    int age = isAdult ? 50 : 10;

    // Create a dummy agent with the specified behavior chart
    Location temp2(0,0);
    Agent temp(age, &temp2, behaviorChart);

    // Return the destination assignment of the agent, which is its starting location
    return getDestinationAssignment(&temp, 0);
}


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
            return keys.at(i);
        }
    }

    // Default return case
    return keys.at(0);
}


QJsonValue AgentController::extractDestinationAssignment(int behavior,
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


QString AgentController::getDestinationAssignment(Agent *agent, int hour) {

    // Initialize local variables
    QString destinationString;

    // Get the agents behavior assignment and whether they are an adult
    int behavior = agent->getBehavior();
    bool adult = agent->isAdult();

    // Pull the Agent's destination from the appriopriate behavior chart
    QJsonValue destination = extractDestinationAssignment(behavior, hour,
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
