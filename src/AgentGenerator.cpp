#include "Headers/AgentGenerator.h"

AgentGenerator::AgentGenerator (Region* home,
                                Region* school,
                                Region* work,
                                Region* leisure) {
    this->home = home;
    this->school = school;
    this->work = work;
    this->leisure = leisure;
}


std::vector<Agent*> AgentGenerator::generate(int num, bool visualize) {


    std::vector<Location*> homeLocations = home->getLocations();
    std::vector<Location*> schoolLocations = school->getLocations();
    std::vector<Location*> workLocations = work->getLocations();
    std::vector<Location*> leisureLocations = leisure->getLocations();

    std::vector<Agent*> agents;
    agents.reserve(num);

    for (int i = 0; i < num; ++i) {
        int homeIndex = rand() % homeLocations.size();
        Coordinate* homePosition = homeLocations[homeIndex]->getPosition();
        Agent* agent = new Agent(rand() % 99,
                                 new Coordinate(homePosition->getX() + rand() % 10,
                                            homePosition->getY() + rand() % 10),
                                 visualize);

        int schoolIndex = rand() % schoolLocations.size();
        int workIndex = rand() % workLocations.size();
        int leisureIndex = rand() % leisureLocations.size();

        agent->setHome(homeLocations[homeIndex]);
        agent->setSchool(schoolLocations[schoolIndex]);
        agent->setWork(workLocations[workIndex]);
        agent->setLeisure(leisureLocations[leisureIndex]);

        if (visualize) {
            agent->initVisualize();
        }

        agents.push_back(agent);
    }

    return agents;
}
