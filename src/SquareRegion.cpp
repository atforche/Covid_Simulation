#include "Headers/SquareRegion.h"

SquareRegion::SquareRegion(int numLocations, Coordinate start, int height,
                           QColor color, std::string name)
    : Region(numLocations, color, name),
      start(Coordinate(start.getX(), start.getY())) {
    this->height = height;
    generator = new LocationGenerator(this);
}


QGraphicsItem* SquareRegion::renderRegion() {
    QColor color = getColor();
    QPen pen = QPen(color);
    QBrush brush = QBrush(QColor(color.red(), color.green(), color.blue(), 10));
    QRect rect = QRect(start.getX(), start.getY(), height, height);
    QGraphicsRectItem* output = new QGraphicsRectItem(rect);
    output->setBrush(brush);
    output->setPen(pen);
    this->rect = output;
    return output;
}


QGraphicsItem* SquareRegion::renderName() {
    QGraphicsTextItem* label = new QGraphicsTextItem(QString::fromStdString(
                                                         getName()));
    label->setPos(start.getX(), start.getY());
    label->setScale(1.2);
    return label;
}


Coordinate SquareRegion::getStart() {
    return this->start;
}


int SquareRegion::getHeight() {
    return this->height;
}


void SquareRegion::generateLocations() {
    std::vector<Location*> locations;
    locations.reserve(getNumLocations());
    for (int i = 0; i < getNumLocations(); ++i) {
        Location* location = generator->generate();
        locations.push_back(location);
    }
    setLocations(locations);
}


std::vector<QGraphicsItem*> SquareRegion::renderLocations() {
    std::vector<Location*> locations = getLocations();
    std::vector<QGraphicsItem*> *rendered =
            new std::vector<QGraphicsItem*>();
    for(size_t i = 0; i < locations.size(); ++i) {
        rendered->push_back(locations[i]->renderLocation());
    }
    return *rendered;
}


SquareRegion::~SquareRegion() {
    delete generator;
}


SquareRegion::LocationGenerator::LocationGenerator(SquareRegion* parent) {
    this->parent = parent;
    dist = std::uniform_int_distribution<int>(10, parent->getHeight() - 10);
}


Location* SquareRegion::LocationGenerator::generate() {
    while(true) {
        int randx = dist(rand);
        int randy = dist(rand);

        QPointF check(randx + parent->getStart().getX(),
                      randy + parent->getStart().getY());
        if (parent->renderRegion()->contains(check)) {
            return new Location(check.x(), check.y());
        }
    }
}
