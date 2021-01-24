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


int AgentController::numAdultBehaviors() {
    return static_cast<int>(adultBehaviors.size());
}


int AgentController::numChildBehaviors() {
    return static_cast<int>(childBehaviors.size());
}

void AgentController::assignAgentDestinations(std::vector<Agent*> agents, int hour) {
    for (size_t i = 0; i < agents.size(); ++i) {
        // Get the agents behavior assignment and whether they are an adult
        int behavior = agents[i]->getBehavior();
        bool adult = agents[i]->isAdult();

        // Pull the Agent's destination from the appriopriate behavior chart
        QString destination;
        if (adult) {
            destination = adultBehaviors[behavior].value(
                        QString::number(hour)).toString();
        } else {
            destination = childBehaviors[behavior].value(
                        QString::number(hour)).toString();
        }

        // Updates the Agent's destination
        if (destination == "Home") {
            agents[i]->setDestination(agents[i]->getLocation(Agent::HOME));
        } else if (destination == "School") {
            agents[i]->setDestination(agents[i]->getLocation(Agent::SCHOOL));
        } else if (destination == "Work") {
            agents[i]->setDestination(agents[i]->getLocation(Agent::WORK));
        } else if (destination == "Leisure") {
            agents[i]->setDestination(agents[i]->getLocation(Agent::LEISURE));
        } else {
            // Throw an exception if an invalid behavior is loaded
            throw "Invalid Behavior File Loaded";
        }
    }
}


QString AgentController::getDestinationAssignment(int behaviorChart,
                                                  int hour, bool isAdult) {
    if (isAdult) {
        return adultBehaviors[behaviorChart].value(
                    QString::number((hour))).toString();
    }
    return childBehaviors[behaviorChart].value(
                QString::number(hour)).toString();
}
