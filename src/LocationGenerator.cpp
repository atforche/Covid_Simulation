#include "Headers/LocationGenerator.h"

LocationGenerator::LocationGenerator() {

}


std::vector<Location*> LocationGenerator::generate(int num,
                                                   QGraphicsItem *region) {
    std::vector<Location*> locations;
    locations.reserve(num);
    xdist = std::uniform_int_distribution<int>(10,
                                               region->boundingRect().width() - 10);
    ydist = std::uniform_int_distribution<int>(10,
                                               region->boundingRect().height() - 10);
    // For each location, random sample points within the boundingRect until the
    // point lies within the shape
    for (int i = 0; i < num; ++i) {
        while(true) {
            int randx = xdist(rand);
            int randy = ydist(rand);

            QPointF check(randx + region ->boundingRect().topLeft().x(),
                          randy + region->boundingRect().topLeft().y());
            if (region->contains(check)) {
                locations.push_back(new Location(check.x(), check.y()));
                break;
            }
        }
    }
    return locations;
}
