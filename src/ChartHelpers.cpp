#include "Headers/ChartHelpers.h"


QtCharts::QBarSet* AgeChartHelper::getNewBarSet() {

    if (barSet == nullptr) {
        barSet = new QtCharts::QBarSet("Ages");
    } else {
        while (barSet->count() > 0) {
            barSet->remove(0);
        }
    }

    return barSet;
}


//******************************************************************************


QtCharts::QBarSeries* AgeChartHelper::getNewBarSeries() {

    if (series == nullptr) {
        series = new QtCharts::QBarSeries();
        series->append(barSet);
        series->attachAxis(xAxis);
    }

    return series;
}


//******************************************************************************


QtCharts::QChart* AgeChartHelper::getNewChart() {

    if (chart == nullptr) {
        chart = new QtCharts::QChart();
        chart->createDefaultAxes();
        chart->addSeries(series);
        chart->setTitle("Age Distribution");
        chart->addAxis(xAxis, Qt::AlignBottom);
    }

    return chart;
}


//******************************************************************************


QtCharts::QBarCategoryAxis* AgeChartHelper::getNewAxis() {

    if (xAxis == nullptr) {
        xAxis = new QtCharts::QBarCategoryAxis();
        xAxis->append("1-10");
        xAxis->append("11-20");
        xAxis->append("21-30");
        xAxis->append("31-40");
        xAxis->append("41-50");
        xAxis->append("51-60");
        xAxis->append("61-70");
        xAxis->append("71-80");
        xAxis->append("81-90");
        xAxis->append("91-100");
    }

    return xAxis;
}


//******************************************************************************


AgeChartHelper::AgeChartHelper() {

    barSet = nullptr;
    series = nullptr;
    chart = nullptr;
    xAxis = nullptr;
}


//******************************************************************************


AgeChartHelper::~AgeChartHelper() {

    delete barSet;
    delete series;
    delete xAxis;
    delete chart;
}


//******************************************************************************


QtCharts::QChart* AgeChartHelper::getAgeChart(std::vector<Agent *> *agents) {

    // Create an updated BarSet with counts
    updateAgeChart(agents);

    // Initialize a QBarSeries to hold a group of QBarSets
    getNewBarSeries();

    // Create an xAxis that includes each age bucket
    getNewAxis();

    // Initialize the QChart and give it a series
    QtCharts::QChart* chart = getNewChart();

    // Create the legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    return chart;
}


//******************************************************************************


void AgeChartHelper::updateAgeChart(std::vector<Agent *> *agents) {
    // Iniailize a vector to store the count in each bucket
    std::vector<int> counts(10, 0);

    // Move through list of agents and increment the appriopriate count
    for (size_t i = 0; i < agents->size(); ++i) {
        int age = agents->at(i)->getAge();
        int index = age / 10;
        index = std::min(index, 9); // Remove this and make agents die when turning 100
        counts[index]++;
    }

    // Initialize a QBarSet to store the set of bars for the graph
    QList<qreal> count(counts.begin(), counts.end());
    QtCharts::QBarSet* barSet = getNewBarSet();
    barSet->append(count);
}


//******************************************************************************


std::vector<QtCharts::QBarSet*>* BehaviorChartHelper::getNewBarSets(int maxNumBehaviors) {

    if (barCounts == nullptr) {
        barCounts = new std::vector<QtCharts::QBarSet*>(maxNumBehaviors, nullptr);

        for (size_t i = 0; i < barCounts->size(); ++i) {
            barCounts->at(i) = new QtCharts::QBarSet(QString::number(i));
        }
    } else {
        for (size_t i = 0; i < barCounts->size(); ++i) {
            while (barCounts->at(i)->count() > 0) {
                barCounts->at(i)->remove(0);
            }
        }
    }

    return barCounts;
}


//******************************************************************************


QtCharts::QBarSeries* BehaviorChartHelper::getNewBarSeries() {

    if (series == nullptr) {
        series = new QtCharts::QBarSeries();
        for (size_t i = 0; i < barCounts->size(); ++i) {
            series->append(barCounts->at(i));
        }
        series->attachAxis(xAxis);
    }

    return series;
}


//******************************************************************************


QtCharts::QChart* BehaviorChartHelper::getNewChart() {

    if (chart == nullptr) {
        chart = new QtCharts::QChart();
        chart->createDefaultAxes();
        chart->addSeries(series);
        chart->setTitle("Behavior Chart Distribution");
        chart->addAxis(xAxis, Qt::AlignBottom);
    }

    return chart;
}


//******************************************************************************


QtCharts::QBarCategoryAxis* BehaviorChartHelper::getNewAxis() {

    if (xAxis == nullptr) {
        xAxis = new QtCharts::QBarCategoryAxis();
        xAxis->append("Adults");
        xAxis->append("Children");
    }

    return xAxis;
}


//******************************************************************************


BehaviorChartHelper::BehaviorChartHelper() {

    barCounts = nullptr;
    series = nullptr;
    chart = nullptr;
    xAxis = nullptr;
}


//******************************************************************************


BehaviorChartHelper::~BehaviorChartHelper() {

    while (barCounts != nullptr && barCounts->size() > 0) {
        QtCharts::QBarSet* victim = barCounts->back();
        barCounts->pop_back();
        delete victim;
    }
    delete barCounts;
    delete series;
    delete xAxis;
    delete chart;
}


//******************************************************************************


QtCharts::QChart* BehaviorChartHelper::getBehaviorChart(std::vector<Agent *> *agents, int numAdultBehaviors, int numChildBehaviors) {

    // Create a new vector of QBarSets with the counts
    updateBehaviorChart(agents, numAdultBehaviors, numChildBehaviors);

    // Create a QBarSeries to group the bars together
    getNewBarSeries();

    // Create a category axis
    getNewAxis();

    // Create the chart to visualize the data
    QtCharts::QChart* chart = getNewChart();

    // Create the chart legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    return chart;
}


//******************************************************************************


void BehaviorChartHelper::updateBehaviorChart(std::vector<Agent *> *agents, int numAdultBehaviors, int numChildBehaviors) {
    // Initialize vectors to store the count of each behavior
    std::vector<int> adultCounts(numAdultBehaviors, 0);
    std::vector<int> childCounts(numChildBehaviors, 0);

    // Count the number of agents that have each behavior
    for (size_t i = 0; i < agents->size(); ++i) {
        if (agents->at(i)->isAdult()) {
            adultCounts[agents->at(i)->getBehavior()]++;
        } else {
            childCounts[agents->at(i)->getBehavior()]++;
        }
    }

    // Create a QBarSet for each the adult and child counts
    std::vector<QtCharts::QBarSet*>* barCounts = getNewBarSets(std::max(numAdultBehaviors, numChildBehaviors));
    for (size_t i = 0; i < barCounts->size(); ++i) {

        if (static_cast<int>(i) < numAdultBehaviors) {
            barCounts->at(i)->append(adultCounts[i]);
        } else {
            barCounts->at(i)->append(0);
        }

        if (static_cast<int>(i) < numChildBehaviors) {
            barCounts->at(i)->append(childCounts[i]);
        } else {
            barCounts->at(i)->append(0);
        }

    }
}


//******************************************************************************


QtCharts::QBarSet* DestinationChartHelper::getNewBarSet() {
    if (barSet == nullptr) {
        barSet = new QtCharts::QBarSet("Counts");
    } else {
        while (barSet->count() > 0) {
            barSet->remove(0);
        }
    }

    return barSet;
}


//******************************************************************************


QtCharts::QBarSeries* DestinationChartHelper::getNewBarSeries() {
    if (series == nullptr) {
        series = new QtCharts::QBarSeries();
        series->append(barSet);
        series->attachAxis(xAxis);
    }

    return series;
}


//******************************************************************************


QtCharts::QChart* DestinationChartHelper::getNewChart() {
    if (chart == nullptr) {
        chart = new QtCharts::QChart();
        chart->createDefaultAxes();
        chart->addSeries(series);
        chart->setTitle("Destination Assignments");
        chart->addAxis(xAxis, Qt::AlignBottom);
    }

    return chart;
}


//******************************************************************************


QtCharts::QBarCategoryAxis* DestinationChartHelper::getNewAxis() {
    if (xAxis == nullptr) {
        xAxis = new QtCharts::QBarCategoryAxis();
        xAxis->append("Home");
        xAxis->append("Work");
        xAxis->append("School");
        xAxis->append("Leisure");
    }

    return xAxis;
}


//******************************************************************************


DestinationChartHelper::DestinationChartHelper() {

    barSet = nullptr;
    series = nullptr;
    chart = nullptr;
    xAxis = nullptr;
}


//******************************************************************************


DestinationChartHelper::~DestinationChartHelper() {

    delete barSet;
    delete series;
    delete xAxis;
    delete chart;
}


//******************************************************************************


QtCharts::QChart* DestinationChartHelper::getDestinationChart(std::vector<Agent *> *agents) {
    // Create an updated BarSet with counts
    updateDestinationChart(agents);

    // Initialize a QBarSeries to hold a group of QBarSets
    getNewBarSeries();

    // Create an xAxis that includes each age bucket
    getNewAxis();

    // Initialize the QChart and give it a series
    QtCharts::QChart* chart = getNewChart();

    // Create the legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    return chart;
}


//******************************************************************************


void DestinationChartHelper::updateDestinationChart(std::vector<Agent *> *agents) {

    // Initialize a map to track the counts for each destination
    std::unordered_map<QString, int> destinationCounts;
    destinationCounts["Home"] = 0;
    destinationCounts["Work"] = 0;
    destinationCounts["School"] = 0;
    destinationCounts["Leisure"] = 0;

    // Update the count for each agent
    for (size_t i = 0; i < agents->size(); ++i) {
        QString destination = agents->at(i)->getDestinationString();
        destinationCounts[destination]++;
    }

    // Update the QBarSet
    QtCharts::QBarSet* bars = getNewBarSet();
    bars->append(destinationCounts["Home"]);
    bars->append(destinationCounts["Work"]);
    bars->append(destinationCounts["School"]);
    bars->append(destinationCounts["Leisure"]);
}





