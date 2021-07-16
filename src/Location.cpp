#include <Headers/Location.h>

Location::Location(double x, double y) {
    position = Coordinate(x, y);

    // Offset by (LOCATION_WIDTH/2) is necessary to allow locations to be
    // rendered so they are centered around their position, instead of the
    // position being the top left coordinate of the Location
    ellipse = new QGraphicsEllipseItem(
                position.getCoord(Coordinate::X) - (LOCATION_WIDTH/2),
                position.getCoord(Coordinate::Y) - (LOCATION_WIDTH/2),
                LOCATION_WIDTH, LOCATION_WIDTH
                );

    // Update the border of the location
    QPen temp = QPen(QColor(0,0,0));
    temp.setWidthF(locationBorderWidth);

    ellipse->setPen(temp);
}


//******************************************************************************


void Location::addAgent(Agent *agent) {
    agents.insert(agent);
}


//******************************************************************************


void Location::removeAgent(Agent* agent) {
    if(agents.find(agent) != agents.end()) {
        agents.erase(agent);
    } else {
        qDebug() << "Error removing agent from location";
    }
}


//******************************************************************************


QGraphicsEllipseItem* Location::getGraphicsObject() {
    return ellipse;
}


//******************************************************************************


Coordinate Location::getPosition() {
    return this->position;
}


//******************************************************************************


void Location::setColor(QColor color) {
    QPen temp = QPen(color);
    temp.setWidthF(locationBorderWidth);

    ellipse->setPen(temp);
}


//******************************************************************************


Location::~Location() {
    delete ellipse;
    ellipse = nullptr;
}
