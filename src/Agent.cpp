#include <Headers/Agent.h>

Agent::Agent(int age) {
    this->age = age;
    this->position = new Coordinate(rand() % 800, rand() % 800);
    this->rect = new QGraphicsRectItem(position->getX(), position->getY(),
                           AGENT_WIDTH, AGENT_WIDTH);
}


QGraphicsRectItem* Agent::renderAgent() {
    return rect;
}


void Agent::update() {
    position->setX(position->getX() + 1);
    position->setY(position->getY() + 1);
    rect->setRect(position->getX(), position->getY(),
                  AGENT_WIDTH, AGENT_WIDTH);
}


Agent::~Agent() {
    delete this->position;
    this->position = nullptr;
}
