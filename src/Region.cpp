#include "Headers/Region.h"
#include "Headers/Simulation.h"
#include "Headers/PandemicLocation.h"

Region::Region(Simulation* sim, QColor color, std::string name) {
    this->numLocations = 0;
    locations = std::vector<Location*>();
    this->color = color;
    this->name = name;
    this->sim = sim;

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

    // Update the number of locations in the region
    this->numLocations = this->numLocations + num;

    // Initialize a vector with enough space for each location
    locations.reserve(this->numLocations);

    // Create num locations within the region
    for (int i = 0; i < num; ++i) {

        Coordinate coord = getRandomCoordinate();
        if (type == "Simple") {
            locations.push_back(new Location(coord.getCoord(Coordinate::X),
                                             coord.getCoord(Coordinate::Y)));
        } else if (type == "Economic") {
            locations.push_back(new EconomicLocation(coord.getCoord(Coordinate::X),
                                                     coord.getCoord(Coordinate::Y),
                                                     this->type));
        } else if (type == "Pandemic") {
            locations.push_back(new PandemicLocation(coord.getCoord(Coordinate::X),
                                                     coord.getCoord(Coordinate::Y),
                                                     this->type));
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


//******************************************************************************


Location* Region::getRandomLocation() {
    if (locations.size() == 0) {
        return nullptr;
    }
    return locations[rand() % locations.size()];
}


//******************************************************************************


Coordinate Region::getRandomCoordinate() {

    // Get the boundary of the region and the GraphicsObject
    QRectF regionBound = getGraphicsObject()->boundingRect();
    QGraphicsItem* regionGraphics = getGraphicsObject();

    // Create random distributions in the range [10, regionBound-10)
    std::uniform_int_distribution<int> xdist(10,
                                            regionBound.width() - 10);
    std::uniform_int_distribution<int> ydist(10,
                                             regionBound.height() - 10);
    std::random_device rand;


    while(true) {
        int randx = xdist(rand);
        int randy = ydist(rand);

        QPointF check(randx + regionGraphics->boundingRect().topLeft().x(),
                      randy + regionGraphics->boundingRect().topLeft().y());
        if (regionGraphics->boundingRect().contains(check)) {
            return Coordinate(check.x(), check.y());
        }
    }
}


//******************************************************************************


void Region::removeLocation(Location *victim) {

    // Add the graphics object to the remove queue
    sim->addToRemoveQueue(victim->getGraphicsObject());

    // Swap the victim to the back and delete it
    auto it = std::find_if(locations.begin(), locations.end(), Location_Comparator(victim));
    std::iter_swap(it, locations.rbegin());
    locations.pop_back();

    delete victim;
}



