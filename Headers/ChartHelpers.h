#ifndef CHARTHELPERS_H
#define CHARTHELPERS_H

#include "QChart"
#include "QBarSet"
#include "QBarSeries"
#include "QBarCategoryAxis"

#include "Agent.h"

/**
 * @brief The AgeChartHelper class
 * Helper class that handles dynamic memory for the Age Distribution Graph View.
 * Offers a singleton abstraction for all dynamically allocated components of
 * the graph, in order to prevent memory leaks
 */
class AgeChartHelper {

private:

    /** Pointers to each dynamic object in this graph view*/
    QtCharts::QBarSet* barSet;
    QtCharts::QBarSeries* series;
    QtCharts::QChart* chart;
    QtCharts::QBarCategoryAxis* xAxis;

    /**
     * @brief getNewBarSet \n
     * Create a new QBarSet if one hasn't been initialized, otherwise reset the
     * existing object
     * @return a pointer to an unused QBarSet
     */
    QtCharts::QBarSet* getNewBarSet();

    /**
     * @brief getNewBarSeries \n
     * Create a new QBarSeries if one hasn't been initialized, otherwise reset
     * the existing object
     * @return a pointer to an unused QBarSeries
     */
    QtCharts::QBarSeries* getNewBarSeries();

    /**
     * @brief getNewChart \n
     * Create a new QChart if one hasn't been initialized, otherwise reset
     * the existing object
     * @return a pointer to an unusued QChart
     */
    QtCharts::QChart* getNewChart();

    /**
     * @brief getNewAxis \n
     * Create a new QBarCategoryAxis if one hasn't been initalized, otherwise
     * reset the existing object
     * @return a pointer ot an unused QBarCategoryAxis
     */
    QtCharts::QBarCategoryAxis* getNewAxis();

public:

    /**
     * @brief AgeChartHelper \n
     * Constructor for the AgeChartHelper class. Sets all pointers to nullptr.
     */
    AgeChartHelper();

    /** Destructor for the AgeChartHelper Class. Cleans up all dynamic memory */
    virtual ~AgeChartHelper();

    /**
     * @brief getAgeChart \n
     * Returns a current pointer to the AgeChart. If the chart has not been
     * created yet, initialize a new one and return it. Utilized to pass
     * ownership of the chart to a new chartView
     * @param agents: the current set of agents in the simulation
     * @return a QChart* that points to the current AgeChart
     */
    QtCharts::QChart* getAgeChart(std::vector<Agent*> *agents);

    /**
     * @brief updateAgeChart \n
     * Updates the bars in the AgeChart to reflect the new Age Distribution
     * of the agents. AgeChart must be initialized using getAgeChart() before
     * this function can be called.
     * @param agents: the current set of agents in the simulation
     */
    void updateAgeChart(std::vector<Agent*> *agents);

};


/**
 * @brief The BehaviorChartHelper class
 * Helper class that handles dynamic memory for the Behavior Distribution Graph View.
 * Offers a singleton abstraction for all dynamically allocated components of
 * the graph, in order to prevent memory leaks
 */
class BehaviorChartHelper {

private:

    /** Pointers to each of the dynamic componenets in this chart*/
    std::vector<QtCharts::QBarSet*>* barCounts;
    QtCharts::QBarSeries* series;
    QtCharts::QChart* chart;
    QtCharts::QBarCategoryAxis* xAxis;

    /**
     * @brief getNewBarSets \n
     * Creates a new vector of QBarSets if one has not been initialized,
     * otherwise it returns the existing vector after clearing it
     * @param maxNumBehaviors: highest number of behavior charts per category, either adult or child
     * @return a pointer to an unused vector of QBarSets
     */
    std::vector<QtCharts::QBarSet*>* getNewBarSets(int maxNumBehaviors);

    /**
     * @brief getNewBarSeries \n
     * Creates a new QBarSeries if one has not been initialized, otherwise it
     * returns the existing object
     * @return a pointer to an QBarSeries containing each QBarSet in BarSets
     */
    QtCharts::QBarSeries* getNewBarSeries();

    /**
     * @brief getNewChart \n
     * Creates a new QChart if one has not been initialized, otherwise it
     * returns the existing object
     * @return a pointer to an QChart containing the bars in Series and the axes in xAxis
     */
    QtCharts::QChart* getNewChart();

    /**
     * @brief getNewAxis \n
     * Creates a new QBarCategoryAxis if one has not been initialized, otherwise it
     * returns the existing object
     * @return a pointer to a QBarCategoryAxis with each behavior category
     */
    QtCharts::QBarCategoryAxis* getNewAxis();

public:

    /**
     * @brief BehaviorChartHelper \n
     * Constructor for the BehaviorChartHelper class. Initializes all internal
     * pointers to nullptr
     */
    BehaviorChartHelper();

    /** Destructor for BehaviorChartHelper class. Deletes all dynamic memory
    and gives each ChartView a new default QChart to prevent double deletes */
    ~BehaviorChartHelper();

    /**
     * @brief getBehaviorChart \n
     * Returns a pointer to a completed BehaviorGraph that can be added to a
     * QChartView. If a BehaviorGraph view has not yet been initialized,
     * it will initialize one. Otherwise, it will return the existing one that
     * has been updated
     * @param agents: the current set of agents in the Simulation
     * @param numAdultBehaviors: number of unique Adult Behaviors
     * @param numChildBehaviors: number of unique Child Behaviors
     * @return
     */
    QtCharts::QChart* getBehaviorChart(std::vector<Agent*> *agents, int numAdultBehaviors, int numChildBehaviors);

    /**
     * @brief updateBehaviorChart \n
     * Updates the BehaviorGraph view without returning a pointer to it. Causes
     * the QChart rendering to be updated on the screen, in whatever QChartView
     * it has been assigned to
     * @param agents: the current set of Agents in the Simulation
     * @param numAdultBehaviors: number of unique Adult Behaviors
     * @param numChildBehaviors: number of unique Child Behaviors
     */
    void updateBehaviorChart(std::vector<Agent*> *agents, int numAdultBehaviors, int numChildBehaviors);


};

#endif // CHARTHELPERS_H
