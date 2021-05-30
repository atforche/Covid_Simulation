#include "Headers/SquareRegion.h"

SquareRegion::SquareRegion(int numLocations, Coordinate start, int height,
                           QColor color_in, std::string name)
    : Region(numLocations, color_in, name),
      start(Coordinate(start.getCoord(Coordinate::X), start.getCoord(Coordinate::Y))) {
    this->height = height;
}


//******************************************************************************


SquareRegion::~SquareRegion() {}


//******************************************************************************


QGraphicsItem* SquareRegion::getGraphicsObject() {

    // REFACTOR (add to constructor and store pointer)
    QColor color = getColor();
    QPen pen = QPen(color);
    QBrush brush = QBrush(QColor(color.red(), color.green(), color.blue(), 10));
    QRect rect = QRect(start.getCoord(Coordinate::X),
                       start.getCoord(Coordinate::Y),
                       height, height
                       );
    QGraphicsRectItem* output = new QGraphicsRectItem(rect);
    output->setBrush(brush);
    output->setPen(pen);
    return output;
}


//******************************************************************************


QGraphicsItem* SquareRegion::getNameGraphicsObject() {

    // REFACTOR (add to constructor and store pointer)
    QGraphicsTextItem* text = new QGraphicsTextItem(QString::fromStdString(getName()));
    text->setPos(start.getCoord(Coordinate::X), start.getCoord(Coordinate::Y));
    text->setScale(1.2);
    return text;
}


//******************************************************************************


Coordinate* SquareRegion::getStartingCoordinate() {
    return &this->start;
}


//******************************************************************************


int SquareRegion::getHeight() {
    return this->height;
}


//******************************************************************************


std::vector<QGraphicsItem*> SquareRegion::getLocationsGraphicsObject() {
    std::vector<Location*> locations = getLocations();
    std::vector<QGraphicsItem*> *rendered =
            new std::vector<QGraphicsItem*>();
    for(size_t i = 0; i < locations.size(); ++i) {
        rendered->push_back(locations[i]->getGraphicsObject());
    }
    return *rendered;
}

