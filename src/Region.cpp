#include "Headers/Region.h"

Region::Region(QColor color, std::string name) {
    this->numLocations = 0;
    locations = std::vector<Location*>();
    this->color = color;
    this->name = name;

    if (name == "Home") {
        type = Agent::HOME;
    } else if (name == "Leisure") {
        type = Agent::LEISURE;
    } else if (name == "School") {
        type = Agent::SCHOOL;
    } else if (name == "Work") {
        type = Agent::WORK;
    }
}


//******************************************************************************


std::vector<Location*>& Region::getLocations() {
    return this->locations;
}


//******************************************************************************


void Region::generateLocations(int num, std::string type) {
    // Get the boundary of the region and the GraphicsObject
    QRectF regionBound = getGraphicsObject()->boundingRect();
    QGraphicsItem* regionGraphics = getGraphicsObject();

    // Update the number of locations in the region
    this->numLocations = this->numLocations + num;

    // Initialize a vector with enough space for each location
    locations.reserve(this->numLocations);

    // Create random distributions in the range [10, regionBound-10)
    std::uniform_int_distribution<int> xdist(10,
                                            regionBound.width() - 10);
    std::uniform_int_distribution<int> ydist(10,
                                             regionBound.height() - 10);
    std::random_device rand;

    // Create num locations within the region
    for (int i = 0; i < num; ++i) {

        // Randomly sample point from the distribution until one is within the
        // boundaries of the GraphicsObject (Monte Carlo style)
        while(true) {
            int randx = xdist(rand);
            int randy = ydist(rand);

            QPointF check(randx + regionGraphics->boundingRect().topLeft().x(),
                          randy + regionGraphics->boundingRect().topLeft().y());
            if (regionGraphics->boundingRect().contains(check)) {
                if (type == "Simple") {
                    locations.push_back(new Location(check.x(), check.y()));
                } else if (type == "Economic") {
                    locations.push_back(new EconomicLocation(check.x(),
                                                             check.y(),
                                                             this->type));
                }
                break;
            }
        }
    }

}


//******************************************************************************


int Region::getNumLocations() {
    return static_cast<int>(locations.size());
}


//******************************************************************************


QColor Region::getColor() {
    return this->color;
}


//******************************************************************************


std::string Region::getName() {
    return this->name;
}

