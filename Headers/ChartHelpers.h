#ifndef CHARTHELPERS_H
#define CHARTHELPERS_H

#include <unordered_map>

#include "QChart"
#include "QBarSet"
#include "QBarSeries"
#include "QBarCategoryAxis"

#include "Agent.h"
#include "Location.h"


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
     * @brief getChart \n
     * Returns a current pointer to the AgeChart. If the chart has not been
     * created yet, initialize a new one and return it. Utilized to pass
     * ownership of the chart to a new chartView
     * @param agents: the current set of agents in the simulation
     * @return a QChart* that points to the current AgeChart
     */
    QtCharts::QChart* getChart(std::vector<Agent*> *agents);

    /**
     * @brief updateChart \n
     * Updates the bars in the AgeChart to reflect the new Age Distribution
     * of the agents. AgeChart must be initialized using getAgeChart() before
     * this function can be called.
     * @param agents: the current set of agents in the simulation
     */
    void updateChart(std::vector<Agent*> *agents);

};


//******************************************************************************


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

    /** Destructor for BehaviorChartHelper class. Deletes all dynamic memory */
    ~BehaviorChartHelper();

    /**
     * @brief getChart \n
     * Returns a pointer to a completed BehaviorGraph that can be added to a
     * QChartView. If a BehaviorGraph view has not yet been initialized,
     * it will initialize one. Otherwise, it will return the existing one that
     * has been updated
     * @param agents: the current set of agents in the Simulation
     * @param numAdultBehaviors: number of unique Adult Behaviors
     * @param numChildBehaviors: number of unique Child Behaviors
     * @return
     */
    QtCharts::QChart* getChart(std::vector<Agent*> *agents, int numAdultBehaviors, int numChildBehaviors);

    /**
     * @brief updateChart \n
     * Updates the BehaviorGraph view without returning a pointer to it. Causes
     * the QChart rendering to be updated on the screen, in whatever QChartView
     * it has been assigned to
     * @param agents: the current set of Agents in the Simulation
     * @param numAdultBehaviors: number of unique Adult Behaviors
     * @param numChildBehaviors: number of unique Child Behaviors
     */
    void updateChart(std::vector<Agent*> *agents, int numAdultBehaviors, int numChildBehaviors);


};


//******************************************************************************


/**
 * @brief The DestinationChartHelper class
 * Helper class that handles dynamic memory for the Destionation Distribution Graph View.
 * Offers a singleton abstraction for all dynamically allocated components of
 * the graph, in order to prevent memory leaks
 */
class DestinationChartHelper {

private:

    /** Pointers to each dynamic object in this graph view*/
    QtCharts::QBarSet* barSet;
    QtCharts::QBarSeries* series;
    QtCharts::QChart* chart;
    QtCharts::QBarCategoryAxis* xAxis;

    /**
     * @brief getNewBarSet \n
     * Creates a new QBarSet if one has not been initialized, otherwise
     * returns the existing object
     * @return a pointer to an unused QBarSet
     */
    QtCharts::QBarSet* getNewBarSet();

    /**
     * @brief getNewBarSeries \n
     * Creates a new QBarSeries if one has not been initialized, otherwise
     * returns the existing object
     * @return a pointer to an ununused QBarSeries
     */
    QtCharts::QBarSeries* getNewBarSeries();

    /**
     * @brief getNewChart \n
     * Creates a new QChart if one has not been initialized, otherwise
     * returns the existing object
     * @return a pointer to an unusued QChart
     */
    QtCharts::QChart* getNewChart();

    /**
     * @brief getNewAxis \n
     * Creates a new QBarCategoryAxis is one has not been initialized, otherwise
     * returns the existing object
     * @return a pointer to an initialized QBarCategoryAxis
     */
    QtCharts::QBarCategoryAxis* getNewAxis();


public:

    /**
     * @brief DestinationChartHelper \n
     * Constructor for the DestinationChartHelper class. Initializes each
     * of the internal pointers to nullptr
     */
    DestinationChartHelper();

    /** Destructor for the DestinationChartHelper class. Deletes all of the
    dynamic memory used */
    ~DestinationChartHelper();

    /**
     * @brief getChart \n
     * Returns a QChart* pointer to a completed DestinationChart that can
     * be added to a QChartView. Initializes the Chart if one has not been
     * initialized, otherwise updates and returns the existing chart
     * @param agents: the current set of agents in the Simulation
     * @return
     */
    QtCharts::QChart* getChart(std::vector<Agent*> *agents);

    /**
     * @brief updateChart \n
     * Updates the DestinationChart to reflect the updated state of the Agents
     * in the Simulation. DestinationChart must be initialized using
     * getDestinationChart first, before this can be called.
     * @param agents
     */
    void updateChart(std::vector<Agent*> *agents);
};


//******************************************************************************


/**
 * @brief roundToNearestHundred \n
 * Rounds the provided number to the nearest hundred value
 * @param n: number to be rounded
 * @return the rounded number as an integer
 */
int roundToNearestHundred(double n);


//******************************************************************************


/**
 * @brief The AgentValueChartHelper class
 * Helper class that handles dynamic memory for the Agent Value Graph View.
 * Offers a singleton abstraction for all dynamically allocated components of
 * the graph, in order to prevent memory leaks
 */
class AgentValueChartHelper {

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

    /** Vector to track the bucket labels */
    std::vector<QString> labels;

    /** Vector to track the bucket thresholds */
    std::vector<int> thresholds;

public:

    /**
     * @brief AgentValueChartHelper \n
     * Constructor for the AgentValueChartHelper class. Sets all pointers to nullptr.
     * @param initialValue: initial integer value the Simulation starts with
     */
    AgentValueChartHelper(int initialValue);

    /** Destructor for the AgentValueChartHelper Class. Cleans up all dynamic memory */
    virtual ~AgentValueChartHelper();

    /**
     * @brief getChart \n
     * Returns a current pointer to the AgentValueChart. If the chart has not been
     * created yet, initialize a new one and return it. Utilized to pass
     * ownership of the chart to a new chartView
     * @param agents: the current set of agents in the simulation
     * @return a QChart* that points to the current AgeChart
     */
    QtCharts::QChart* getChart(std::vector<Agent*> *agents);

    /**
     * @brief updateAgeChart \n
     * Updates the bars in the AgentValueChart to reflect the new value distribution
     * of the agents. AgentValueChart must be initialized using getChart() before
     * this function can be called.
     * @param agents: the current set of agents in the simulation
     */
    void updateChart(std::vector<Agent*> *agents);

    /**
     * @brief getThresholds \n
     * Getter function for the different thresholds for the different buckets on
     * the graph. Enables the simulation to dynamically color agents based on
     * their economic value.
     * @return
     */
    std::vector<int>& getThresholds();

};


//******************************************************************************


/**
 * @brief The WorkValueChartHelper class
 * Helper class that handles dynamic memory for the Age Distribution Graph View.
 * Offers a singleton abstraction for all dynamically allocated components of
 * the graph, in order to prevent memory leaks
 */
class WorkValueChartHelper {

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

    /** Vector to track the bucket labels */
    std::vector<QString> labels;

    /** Vector to track the bucket thresholds */
    std::vector<int> thresholds;

public:

    /**
     * @brief WorkValueCharHelper \n
     * Constructor for the WorkValueCharHelper class. Sets all pointers to nullptr.
     * @param initialValue: initial integer value the simulation starts with
     */
    WorkValueChartHelper(int initialValue);

    /** Destructor for the WorkValueCharHelper Class. Cleans up all dynamic memory */
    virtual ~WorkValueChartHelper();

    /**
     * @brief getChart \n
     * Returns a current pointer to the WorkValueChart. If the chart has not been
     * created yet, initialize a new one and return it. Utilized to pass
     * ownership of the chart to a new chartView
     * @param agents: the current set of agents in the simulation
     * @return a QChart* that points to the current WorkValueChart
     */
    QtCharts::QChart* getChart(std::vector<Location*> *locations);

    /**
     * @brief updateChart \n
     * Updates the bars in the WorkValue Chart to reflect the new value
     * distribution among work locations. Chart must be initialized using
     * getChart() before this function can be called.
     * @param locations: the current vector of locations in the simulation
     */
    void updateChart(std::vector<Location*> *locations);

    /**
     * @brief getThresholds \n
     * Getter function for the different thresholds for the different buckets on
     * the graph. Enables the simulation to dynamically color locations based on
     * their economic value.
     * @return
     */
    std::vector<int>& getThresholds();

};


//******************************************************************************


/**
 * @brief The TotalValueChartHelper class
 * Helper class that handles dynamic memory for the Total Value Graph View.
 * Offers a singleton abstraction for all dynamically allocated components of
 * the graph, in order to prevent memory leaks
 */
class TotalValueChartHelper {

private:

    /** Pointers to each dynamic object in this graph view*/
    QtCharts::QLineSeries* lineSeries;
    QtCharts::QChart* chart;
    QtCharts::QValueAxis* xAxis;
    QtCharts::QValueAxis* yAxis;

    /** Maximum and minimum values of the yAxis */
    int maximum;
    int minimum;

    /**
     * @brief getNewLineSeries \n
     * Creates a new QLineSeries object if one hasn't been initialized, otherwise
     * just returns a pointer the existing one.
     * @return a pointer to a QLineSeries
     */
    QtCharts::QLineSeries* getNewLineSeries();

    /**
     * @brief getNewChart \n
     * Creates a new QChart object if one hasn't been initialized, otherwise
     * just returns a pointer to the existing one.
     * @return a pointer to a QChart
     */
    QtCharts::QChart* getNewChart();

    /**
     * @brief getNewAxis \n
     * Creates a new QValueAxis object if the specified one hasn't been
     * initialized, otherwise just returns a pointer to the existing one
     * @param which: "y" for the yAxis, "x" for the xAxis
     * @return a pointer to a QValueAxis
     */
    QtCharts::QValueAxis* getNewAxis(std::string which);

public:

    /**
     * @brief TotalValueChartHelper \n
     * Constructor for the TotalValueChartHelper class. Initializes all pointers
     * within the class to nullptr.
     */
    TotalValueChartHelper();

    /** Destructor for the TotalValueChart class */
    virtual ~TotalValueChartHelper();

    /**
     * @brief getChart \n
     * Returns a current pointer to the TotalValueChart. If the chart has not been
     * created yet, initialize a new one and return it. Utilized to pass
     * ownership of the chart to a new chartView
     * @param totalValue: the current total value in the simulation
     * @return a pointer to the Total Value Chart
     */
    QtCharts::QChart* getChart(int totalValue);

    /**
     * @brief updateChart
     * Updates the bars in the Total Value Chart to reflect the new value
     * across the entire simulation. Chart must be initialized using
     * getChart() before this function can be called.
     * @param totalValue: the current total value in the simulation
     */
    void updateChart(int totalValue);

};


//******************************************************************************


/**
 * @brief The EconomicStatusChartHelper class
 * Helper class that handles dynamic memory for the Economic Status Graph View.
 * Offers a singleton abstraction for all dynamically allocated components of
 * the graph, in order to prevent memory leaks
 */
class EconomicStatusChartHelper {

private:

    /** Pointers to each dynamic object in this graph view*/
    QtCharts::QLineSeries* employmentLineSeries;
    QtCharts::QLineSeries* homelessLineSeries;
    QtCharts::QChart* chart;
    QtCharts::QValueAxis* xAxis;
    QtCharts::QValueAxis* yAxis;

    /** Maximum and minimum values of the yAxis */
    int maximum;
    int minimum;

    /**
     * @brief getHomelessLineSeries \n
     * Creates a new QLineSeries object if one hasn't been initialized, otherwise
     * just returns a pointer the existing one.
     * @return a pointer to a QLineSeries
     */
    QtCharts::QLineSeries* getHomelessLineSeries();

    /**
     * @brief getUnemployedLineSeries \n
     * Creates a new QLineSeries object if one hasn't been initialized, otherwise
     * just returns a pointer the existing one.
     * @return a pointer to a QLineSeries
     */
    QtCharts::QLineSeries* getUnemployedLineSeries();

    /**
     * @brief getNewChart \n
     * Creates a new QChart object if one hasn't been initialized, otherwise
     * just returns a pointer to the existing one.
     * @return a pointer to a QChart
     */
    QtCharts::QChart* getNewChart();

    /**
     * @brief getNewAxis \n
     * Creates a new QValueAxis object if the specified one hasn't been
     * initialized, otherwise just returns a pointer to the existing one
     * @param which: "y" for the yAxis, "x" for the xAxis
     * @return a pointer to a QValueAxis
     */
    QtCharts::QValueAxis* getNewAxis(std::string which);

public:

    /**
     * @brief TotalValueChartHelper \n
     * Constructor for the TotalValueChartHelper class. Initializes all pointers
     * within the class to nullptr.
     */
    EconomicStatusChartHelper();

    /** Destructor for the EconomicStatusChartHelper class */
    virtual ~EconomicStatusChartHelper();

    /**
     * @brief getChart \n
     * Returns a current pointer to the TotalValueChart. If the chart has not been
     * created yet, initialize a new one and return it. Utilized to pass
     * ownership of the chart to a new chartView
     * @param totalValue: the current total value in the simulation
     * @return a pointer to the Total Value Chart
     */
    QtCharts::QChart* getChart(double homelessCount, double unemployedCount, int numAgents);

    /**
     * @brief updateChart
     * Updates the bars in the Total Value Chart to reflect the new value
     * across the entire simulation. Chart must be initialized using
     * getChart() before this function can be called.
     * @param totalValue: the current total value in the simulation
     */
    void updateChart(double homelessCount, double unemployedCount, int numAgents);

};


//******************************************************************************


/**
 * @brief The SEIRChartHelper class
 * Helper class that handles dynamic memory for the SEIR Graph View.
 * Offers a singleton abstraction for all dynamically allocated components of
 * the graph, in order to prevent memory leaks
 */
class SEIRChartHelper {

private:

    /** Pointers to each dynamic object in this graph view*/
    QtCharts::QLineSeries* susceptibleLineSeries;
    QtCharts::QLineSeries* exposedLineSeries;
    QtCharts::QLineSeries* infectedLineSeries;
    QtCharts::QLineSeries* recoveredLineSeries;

    QtCharts::QChart* chart;
    QtCharts::QValueAxis* xAxis;
    QtCharts::QValueAxis* yAxis;

    /** Maximum and minimum values of the yAxis */
    int maximum;
    int minimum;

    /**
     * @brief getLineSeries \n
     * Creates a new QLineSeries object if one hasn't been initialized, otherwise
     * just returns a pointer the existing one.
     * @param which: string to represent which LineSeries to retrieve
     * @return a pointer to a QLineSeries
     */
    QtCharts::QLineSeries* getLineSeries(QString which);

    /**
     * @brief getNewChart \n
     * Creates a new QChart object if one hasn't been initialized, otherwise
     * just returns a pointer to the existing one.
     * @return a pointer to a QChart
     */
    QtCharts::QChart* getNewChart();

    /**
     * @brief getNewAxis \n
     * Creates a new QValueAxis object if the specified one hasn't been
     * initialized, otherwise just returns a pointer to the existing one
     * @param which: "y" for the yAxis, "x" for the xAxis
     * @return a pointer to a QValueAxis
     */
    QtCharts::QValueAxis* getNewAxis(std::string which);

public:

    /**
     * @brief SEIRChartHelper \n
     * Constructor for the SEIRChartHelper class. Initializes all pointers
     * within the class to nullptr.
     */
    SEIRChartHelper();

    /** Destructor for the SEIRChartHelper class */
    virtual ~SEIRChartHelper();

    /**
     * @brief getChart \n
     * Returns a current pointer to the SEIRChartHelper. If the chart has not been
     * created yet, initialize a new one and return it. Utilized to pass
     * ownership of the chart to a new chartView
     * @param SEIRAgents: vector having the count of each category of agent
     * @param numAgents: total number of agents in the simulation
     * @return a pointer to the SEIR Chart
     */
    QtCharts::QChart* getChart(std::vector<int> SEIRAgents, int numAgents);

    /**
     * @brief updateChart
     * Updates the lines in the SEIRChart to reflect the new pandemic situation
     * across the entire simulation. Chart must be initialized using
     * getChart() before this function can be called.
     * @param SEIRAgents: vector having the count of each category of agent
     * @param numAgents: total number of agents in the simulation
     */
    void updateChart(std::vector<int> SEIRAgents, int numAgents);

};


//******************************************************************************


/**
 * @brief The DailyTrackerChartHelper class
 * Helper class that handles dynamic memory for the Daily Tracker Graph View.
 * Offers a singleton abstraction for all dynamically allocated components of
 * the graph, in order to prevent memory leaks
 */
class DailyTrackerChartHelper {

private:

    /** Pointers to each dynamic object in this graph view*/
    QtCharts::QLineSeries* dailyCaseLineSeries;
    QtCharts::QLineSeries* dailyDeathLineSeries;
    QtCharts::QChart* chart;
    QtCharts::QValueAxis* xAxis;
    QtCharts::QValueAxis* yAxis;

    /** Maximum and minimum values of the yAxis */
    int maximum;
    int minimum;

    /**
     * @brief getLineSeries \n
     * Creates a new QLineSeries object if one hasn't been initialized, otherwise
     * just returns a pointer the existing one.
     * @param which: string representing which LineSeries to grab
     * @return a pointer to a QLineSeries
     */
    QtCharts::QLineSeries* getLineSeries(QString which);

    /**
     * @brief getNewChart \n
     * Creates a new QChart object if one hasn't been initialized, otherwise
     * just returns a pointer to the existing one.
     * @return a pointer to a QChart
     */
    QtCharts::QChart* getNewChart();

    /**
     * @brief getNewAxis \n
     * Creates a new QValueAxis object if the specified one hasn't been
     * initialized, otherwise just returns a pointer to the existing one
     * @param which: "y" for the yAxis, "x" for the xAxis
     * @return a pointer to a QValueAxis
     */
    QtCharts::QValueAxis* getNewAxis(std::string which);

public:

    /**
     * @brief DailyTrackerChartHelper \n
     * Constructor for the DailyTrackerChartHelper class. Initializes all pointers
     * within the class to nullptr.
     */
    DailyTrackerChartHelper();

    /** Destructor for the DailyTrackerChartHelper class */
    virtual ~DailyTrackerChartHelper();

    /**
     * @brief getChart \n
     * Returns a current pointer to the DailyTrackerChartHelper. If the chart has not been
     * created yet, initialize a new one and return it. Utilized to pass
     * ownership of the chart to a new chartView
     * @param newDailyCases: number of new Cases in the past day
     * @param newDailyDeaths: number of new Deaths in the past day
     * @return a pointer to the Total Value Chart
     */
    QtCharts::QChart* getChart(int newDailyCases, int newDailyDeaths);

    /**
     * @brief updateChart
     * Updates the lines in the DailyTrackerChartHelper to reflect the new value
     * across the entire simulation. Chart must be initialized using
     * getChart() before this function can be called.
     * @param newDailyCases: number of new Cases in the past day
     * @param newDailyDeaths: number of new Deaths in the past day
     */
    void updateChart(int newDailyCases, int newDailyDeaths);

};


//******************************************************************************


// Includes to prevent circular dependencies
#include "EconomicAgent.h"
#include "EconomicLocation.h"
#include "PandemicAgent.h"


#endif // CHARTHELPERS_H
