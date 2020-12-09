#ifndef LOCATIONGENERATOR_H
#define LOCATIONGENERATOR_H

#include <random>
#include "Location.h"
#include "QGraphicsItem"

/**
 * @brief The LocationGenerator class
 * Generic Class that handles generating a random sample of locations for a
 * region regardless of the shape of the region
 */

class LocationGenerator {

private:

    /** Random device to provide additional random seeding */
    std::random_device rand;

    /** Random distrubution from which to draw random x coordinates */
    std::uniform_int_distribution<int> xdist;

    /** Random distribution from which to draw random y coordinates */
    std::uniform_int_distribution<int> ydist;

public:

    /**
     * @brief LocationGenerator
     * Default constructor for the Location Generator class
     */
    LocationGenerator();

    /**
     * @brief generate
     * Function to generate a set of random Locations that lie within
     * any shaped region.
     * @param num: the number of locations to generate
     * @param region: the bounding region in which the locations need
     * to reside
     * @return a std::vector of all the locations generated
     */
    std::vector<Location*> generate(int num, QGraphicsItem* region);

};

#endif // LOCATIONGENERATOR_H
