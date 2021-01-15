#include "Headers/AgentController.h"

AgentController::AgentController(int numBehaviors) {
    this->numBehaviors = numBehaviors;
    this->behaviors = std::vector<QJsonObject>(numBehaviors);

    QFile file;
    for (int i = 0; i < numBehaviors; ++i) {
        file.setFileName(QString::fromStdString(":/bin/behaviors/" +
                                                std::to_string(i+1) + ".json"));
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString val = file.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject behavior = doc.object();

        behaviors.push_back(behavior);
    }
}


int AgentController::getNumBehaviors() {
    return this->numBehaviors;
}


void AgentController::setDestinations(std::vector<Agent*> agents, int hour) {
    for (size_t i = 0; i < agents.size(); ++i) {
        int behavior = agents[i]->getBehavior();
        QString destination = behaviors[behavior].value(
                    QString::number(hour)).toString();
        if (destination == "Home") {
            agents[i]->setDestination(agents[i]->getHome());
        } else if (destination == "School") {
            agents[i]->setDestination(agents[i]->getSchool());
        } else if (destination == "Work") {
            agents[i]->setDestination(agents[i]->getWork());
        } else if (destination == "Leisure") {
            agents[i]->setDestination(agents[i]->getLeisure());
        }
    }
}
