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


QtCharts::QChart* AgeChartHelper::getChart(std::vector<Agent*> *agents) {

    // Create an updated BarSet with counts
    updateChart(agents);

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


void AgeChartHelper::updateChart(std::vector<Agent *> *agents) {
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


QtCharts::QChart* BehaviorChartHelper::getChart(std::vector<Agent *> *agents, int numAdultBehaviors, int numChildBehaviors) {

    // Create a new vector of QBarSets with the counts
    updateChart(agents, numAdultBehaviors, numChildBehaviors);

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


void BehaviorChartHelper::updateChart(std::vector<Agent *> *agents, int numAdultBehaviors, int numChildBehaviors) {
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


QtCharts::QChart* DestinationChartHelper::getChart(std::vector<Agent *> *agents) {
    // Create an updated BarSet with counts
    updateChart(agents);

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


void DestinationChartHelper::updateChart(std::vector<Agent *> *agents) {

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


//******************************************************************************


int roundToNearestHundred(double n) {
    return ((n + 50) / 100) * 100;
}


//******************************************************************************


QtCharts::QBarSet* AgentValueChartHelper::getNewBarSet() {
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


QtCharts::QBarSeries* AgentValueChartHelper::getNewBarSeries() {
    if (series == nullptr) {
        series = new QtCharts::QBarSeries();
        series->append(barSet);
        series->attachAxis(xAxis);
    }

    return series;
}


//******************************************************************************


QtCharts::QChart* AgentValueChartHelper::getNewChart() {
    if (chart == nullptr) {
        chart = new QtCharts::QChart();
        chart->createDefaultAxes();
        chart->addSeries(series);
        chart->setTitle("Agent Values");
        chart->addAxis(xAxis, Qt::AlignBottom);
    }

    return chart;
}


//******************************************************************************


QtCharts::QBarCategoryAxis* AgentValueChartHelper::getNewAxis() {
    if (xAxis == nullptr) {
        xAxis = new QtCharts::QBarCategoryAxis();
        xAxis->append(labels[0]);
        xAxis->append(labels[1]);
        xAxis->append(labels[2]);
        xAxis->append(labels[3]);
        xAxis->append(labels[4]);
    }

    return xAxis;
}


//******************************************************************************


AgentValueChartHelper::AgentValueChartHelper(int initialValue) {

    // Initialze pointers to nullptr
    barSet = nullptr;
    series = nullptr;
    chart = nullptr;
    xAxis = nullptr;

    // Establish the thresholds for each graph bucket
    int initialAgentValue = initialValue / 2;
    thresholds = std::vector<int>();
    thresholds.push_back(0); // Zero will always be the baseline
    thresholds.push_back(roundToNearestHundred(initialAgentValue * 0.025)); // 2.5% of initial business value
    thresholds.push_back(roundToNearestHundred(initialAgentValue * 0.05)); // 5% of initial business value
    thresholds.push_back(roundToNearestHundred(initialAgentValue * 0.100)); // 10% of initial business value

    // Convert each threshold into a label
    labels = std::vector<QString>();
    labels.push_back("$0");
    for (size_t i = 1; i < thresholds.size(); ++i) {
        labels.push_back("$" + QString::number(thresholds[i-1]) + "-$" +
                QString::number(thresholds[i]));
    }
    labels.push_back(">$" + QString::number(thresholds.back()));
}


//******************************************************************************


AgentValueChartHelper::~AgentValueChartHelper() {

    delete barSet;
    delete series;
    delete xAxis;
    delete chart;

}


//******************************************************************************


QtCharts::QChart* AgentValueChartHelper::getChart(std::vector<Agent *> *agents) {
    // Create an updated BarSet with counts
    updateChart(agents);

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


void AgentValueChartHelper::updateChart(std::vector<Agent *> *agents) {

    // Initialize a map to track the counts for each destination
    std::vector<int> valueCounts(5, 0);

    // Update the count for each agent
    for (size_t i = 0; i < agents->size(); ++i) {
        EconomicAgent* agent = dynamic_cast<EconomicAgent*>(agents->at(i));
        int agentValue = agent->getValue();

        if (agentValue  == 0) {
            valueCounts[0]++;
        } else if (agentValue < thresholds[1]) {
            valueCounts[1]++;
        } else if (agentValue < thresholds[2]) {
            valueCounts[2]++;
        } else if (agentValue < thresholds[3]) {
            valueCounts[3]++;
        } else {
            valueCounts[4]++;
        }
    }

    // Update the QBarSet
    QtCharts::QBarSet* bars = getNewBarSet();
    bars->append(valueCounts[0]);
    bars->append(valueCounts[1]);
    bars->append(valueCounts[2]);
    bars->append(valueCounts[3]);
    bars->append(valueCounts[4]);
}


//******************************************************************************


std::vector<int>& AgentValueChartHelper::getThresholds() {
    return this->thresholds;
}


//******************************************************************************


QtCharts::QBarSet* WorkValueChartHelper::getNewBarSet() {
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


QtCharts::QBarSeries* WorkValueChartHelper::getNewBarSeries() {
    if (series == nullptr) {
        series = new QtCharts::QBarSeries();
        series->append(barSet);
        series->attachAxis(xAxis);
    }

    return series;
}


//******************************************************************************


QtCharts::QChart* WorkValueChartHelper::getNewChart() {
    if (chart == nullptr) {
        chart = new QtCharts::QChart();
        chart->createDefaultAxes();
        chart->addSeries(series);
        chart->setTitle("Business Values");
        chart->addAxis(xAxis, Qt::AlignBottom);
    }

    return chart;
}


//******************************************************************************


QtCharts::QBarCategoryAxis* WorkValueChartHelper::getNewAxis() {
    if (xAxis == nullptr) {
        xAxis = new QtCharts::QBarCategoryAxis();
        xAxis->append(labels[0]);
        xAxis->append(labels[1]);
        xAxis->append(labels[2]);
        xAxis->append(labels[3]);
        xAxis->append(labels[4]);
    }

    return xAxis;
}


//******************************************************************************


WorkValueChartHelper::WorkValueChartHelper(int initialValue) {

    // Initialize pointers to nullptr
    barSet = nullptr;
    series = nullptr;
    chart = nullptr;
    xAxis = nullptr;

    // Establish the thresholds for each graph bucket
    int initialBusinessValue = initialValue / 2;
    thresholds = std::vector<int>();
    thresholds.push_back(0); // Zero will always be the baseline
    thresholds.push_back(roundToNearestHundred(initialBusinessValue * 0.025)); // 2.5% of initial business value
    thresholds.push_back(roundToNearestHundred(initialBusinessValue * 0.05)); // 5% of initial business value
    thresholds.push_back(roundToNearestHundred(initialBusinessValue * 0.100)); // 10% of initial business value

    // Convert each threshold into a label
    labels = std::vector<QString>();
    labels.push_back("$0");
    for (size_t i = 1; i < thresholds.size(); ++i) {
        labels.push_back("$" + QString::number(thresholds[i-1]) + "-$" +
                QString::number(thresholds[i]));
    }
    labels.push_back(">$" + QString::number(thresholds.back()));
}


//******************************************************************************


WorkValueChartHelper::~WorkValueChartHelper() {

    delete barSet;
    delete series;
    delete xAxis;
    delete chart;
}


//******************************************************************************


QtCharts::QChart* WorkValueChartHelper::getChart(std::vector<Location*> *locations) {
    // Create an updated BarSet with counts
    updateChart(locations);

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


void WorkValueChartHelper::updateChart(std::vector<Location*> *locations) {

    // Initialize a map to track the counts for each destination
    std::vector<int> valueCounts(5, 0);

    // Update the count for each agent
    for (size_t i = 0; i < locations->size(); ++i) {
        EconomicLocation* location = dynamic_cast<EconomicLocation*>(locations->at(i));
        int locationValue = location->getValue();

        if (locationValue  == 0) {
            valueCounts[0]++;
        } else if (locationValue < thresholds[1]) {
            valueCounts[1]++;
        } else if (locationValue < thresholds[2]) {
            valueCounts[2]++;
        } else if (locationValue < thresholds[3]) {
            valueCounts[3]++;
        } else {
            valueCounts[4]++;
        }
    }

    // Update the QBarSet
    QtCharts::QBarSet* bars = getNewBarSet();
    bars->append(valueCounts[0]);
    bars->append(valueCounts[1]);
    bars->append(valueCounts[2]);
    bars->append(valueCounts[3]);
    bars->append(valueCounts[4]);
}


//******************************************************************************


std::vector<int>& WorkValueChartHelper::getThresholds() {
    return this->thresholds;
}


//******************************************************************************


QtCharts::QLineSeries* TotalValueChartHelper::getNewLineSeries() {
    if (lineSeries == nullptr) {
        lineSeries = new QLineSeries();
    }
    return lineSeries;
}


//******************************************************************************


QtCharts::QChart* TotalValueChartHelper::getNewChart() {
    if (chart == nullptr) {
        chart = new QtCharts::QChart();
    }

    chart->addSeries(lineSeries);
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    chart->setTitle("Total Value");

    return chart;
}


//******************************************************************************


QtCharts::QValueAxis* TotalValueChartHelper::getNewAxis(std::string which) {
    if (which == "y") {
        if (yAxis == nullptr) {
            yAxis = new QValueAxis();
        }
        return yAxis;
    } else if (which == "x") {
        if (xAxis == nullptr) {
            xAxis = new QValueAxis();
        }
        return xAxis;
    }
    return nullptr;
}


//******************************************************************************


TotalValueChartHelper::TotalValueChartHelper() {
    // Initialize nullptrs for all dynamic memory objects
    lineSeries = nullptr;
    chart = nullptr;
    xAxis = nullptr;
    yAxis = nullptr;

    // Initialize maximum and minimum values
    maximum = INT_MIN;
    minimum = INT_MAX;
}


//******************************************************************************


QtCharts::QChart* TotalValueChartHelper::getChart(int totalValue) {
    // Create an updated BarSet with counts
    updateChart(totalValue);

    // Initialize a QBarSeries to hold a group of QBarSets
    getNewLineSeries();

    // Initialize the QChart and give it a series
    QtCharts::QChart* chart = getNewChart();

    // Create the legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    return chart;
}


//******************************************************************************


void TotalValueChartHelper::updateChart(int totalValue) {

    // Add the new point to the Line Series
    if (lineSeries == nullptr) {
        getNewLineSeries();
    }
    lineSeries->append(lineSeries->count(), totalValue);

    // Update the axes to show the entire graph
    QValueAxis* y = getNewAxis("y");
    QValueAxis* x = getNewAxis("x");

    // Update the minimum and maximum values of the Y-axis
    minimum = std::min(minimum, totalValue);
    maximum = std::max(maximum, totalValue);
    y->setRange(minimum - 100, maximum + 100);
    lineSeries->attachAxis(y);

    // Update the range of the X-axis
    x->setRange(0, lineSeries->count());
    lineSeries->attachAxis(x);

    // Add the new series to the chart
    if (chart != nullptr) {
        chart->addSeries(lineSeries);
    }

}

