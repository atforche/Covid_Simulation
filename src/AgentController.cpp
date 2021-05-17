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
        file.setFileName(QString::fromStdString(":/bin/behaviors/") +=
                adult.at(i));
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString val = file.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());

        adultBehaviors[i] = doc.object();
    }

    // Read in the JSON for each child behavior
    for (int i = 0; i < child.size(); ++i) {
        file.setFileName(QString::fromStdString(":/bin/behaviors/") +=
                child.at(i));
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString val = file.readAll();
        file.close();
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
    int age = isAdult ? 50 : 10;
    Location temp2(0,0);
    Agent temp(age, &temp2, behaviorChart);
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
    QJsonValue destination;
    QJsonObject::iterator key;
    if (adult) {
        // Check if current hour exists in adult chart. If it does,
        // store the QJsonValue. Else, store a default QJsonValue
        key = adultBehaviors[behavior].find(
                    QString::number(hour));
        if (key != adultBehaviors[behavior].end()) {
            destination = key.value();
        } else {
            destination = QJsonValue("No Change");
        }

    } else {
        // Check if current hour exists in adult chart. If it does,
        // store the QJsonValue. Else, store a default QJsonValue
        key = childBehaviors[behavior].find(
                    QString::number(hour));
        if (key != childBehaviors[behavior].end()) {
            destination = key.value();
        } else {
            destination = QJsonValue("No Change");
        }
    }

    return destination;
}


QString AgentController::getDestinationAssignment(Agent *agent, int hour) {
    // Get the agents behavior assignment and whether they are an adult
    int behavior = agent->getBehavior();
    bool adult = agent->isAdult();

    // Pull the Agent's destination from the appriopriate behavior chart
    QJsonValue destination = extractDestinationAssignment(behavior, hour,
                                                          adult);
    QString destinationString;
    // Destination will either be a JSON string or object. Check which it is
    // and process it accordingly
    if (destination.isString()) {
        destinationString = destination.toString();
    } else {
        QJsonObject object = destination.toObject();
        QStringList keys = object.keys();
        std::vector<double> probabilities(keys.size());
        for (int i = 0; i < keys.size(); ++i) {
            probabilities[i] = object.value(keys[i]).toDouble();
        }
        destinationString = evaluateDestinationProbabilities(keys,
                                                             probabilities);
    }

    return destinationString;
}
