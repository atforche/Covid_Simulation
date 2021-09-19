#include "Headers/DualAgent.h"

DualAgent::DualAgent(int initialValue, int age, Location* startingLocation,
                     QString startingLocationString, int behavior) :
    Agent(age, startingLocation, startingLocationString, behavior),
    PandemicAgent(age, startingLocation, startingLocationString, behavior),
    EconomicAgent(initialValue, age, startingLocation, startingLocationString, behavior) {};


//******************************************************************************
