#include <Headers/Agent.h>

Agent::Agent(int age, Coordinate position, bool visualize) {
    this->age = age;
    this->position = &position;
    this->rect = new QGraphicsRectItem(this->position->getX(),
                                       this->position->getY(),
                                       AGENT_WIDTH, AGENT_WIDTH);
    this->visualize = visualize;

    this->schoolLine = nullptr;
    this->workLine = nullptr;
    this->homeLine = nullptr;
    this->leisureLine = nullptr;

    this->home = nullptr;
    this->school = nullptr;
    this->work = nullptr;
    this->leisure = nullptr;
}


QGraphicsRectItem* Agent::renderAgent() {
    return rect;
}


void Agent::update() {
    position->setX(position->getX() + 1);
    position->setY(position->getY() + 1);
    rect->setRect(position->getX(), position->getY(),
                  AGENT_WIDTH, AGENT_WIDTH);

    if (visualize) {
        if (homeLine != nullptr) {
            homeLine->setLine(getPosition()->getX(),
                              getPosition()->getY(),
                              getHome()->getPosition()->getX(),
                              getHome()->getPosition()->getY());
        }
        if (schoolLine != nullptr) {
            schoolLine->setLine(getPosition()->getX(),
                                getPosition()->getY(),
                                getSchool()->getPosition()->getX(),
                                getSchool()->getPosition()->getY());
        }
        if (workLine != nullptr) {
            workLine->setLine(getPosition()->getX(),
                              getPosition()->getY(),
                              getWork()->getPosition()->getX(),
                              getWork()->getPosition()->getY());
        }
        if (leisureLine != nullptr) {
            leisureLine->setLine(getPosition()->getX(),
                                 getPosition()->getY(),
                                 getLeisure()->getPosition()->getX(),
                                 getLeisure()->getPosition()->getY());
        }
    }
}


void Agent::setHome(Location *home) {
    this->home = home;
}


Location* Agent::getHome() {
    return this->home;
}


void Agent::setSchool(Location *school) {
    this->school = school;
}


Location* Agent::getSchool() {
    return this->school;
}


void Agent::setWork(Location *work) {
    this->work = work;
}


Location* Agent::getWork() {
    return this->work;
}


void Agent::setLeisure(Location *leisure) {
    this->leisure = leisure;
}


Location* Agent::getLeisure() {
    return this->leisure;
}


Coordinate* Agent::getPosition() {
    return this->position;
}


bool Agent::getVisualize() {
    return this->visualize;
}


void Agent::initVisualize() {
    QPen pen;
    pen.setWidth(1);
    if (this->home != nullptr) {
        this->homeLine = new QGraphicsLineItem(getPosition()->getX(),
                                               getPosition()->getY(),
                                               getHome()->getPosition()->getX(),
                                               getHome()->getPosition()->getY());
        this->homeLine->setPen(pen);
    }
    if (this->school != nullptr) {
        this->schoolLine = new QGraphicsLineItem(getPosition()->getX(),
                                                 getPosition()->getY(),
                                                 getSchool()->getPosition()->getX(),
                                                 getSchool()->getPosition()->getY());
        this->schoolLine->setPen(pen);
    }
    if (this->work != nullptr) {
        this->workLine = new QGraphicsLineItem(getPosition()->getX(),
                                               getPosition()->getY(),
                                               getWork()->getPosition()->getX(),
                                               getWork()->getPosition()->getY());
        this->workLine->setPen(pen);
    }
    if (this->leisure != nullptr) {
        this->leisureLine = new QGraphicsLineItem(getPosition()->getX(),
                                                  getPosition()->getY(),
                                                  getLeisure()->getPosition()->getX(),
                                                  getLeisure()->getPosition()->getY());
        this->leisureLine->setPen(pen);
    }
}


std::vector<QGraphicsItem*> Agent::renderVisualize() {
    std::vector<QGraphicsItem*> lines;
    if (workLine != nullptr) {
        lines.push_back(workLine);
    }
    if (homeLine != nullptr) {
        lines.push_back(homeLine);
    }
    if (schoolLine != nullptr) {
        lines.push_back(schoolLine);
    }
    if (leisureLine != nullptr) {
        lines.push_back(leisureLine);
    }
    return lines;
}


Agent::~Agent() {
    delete this->position;
    this->position = nullptr;
}
