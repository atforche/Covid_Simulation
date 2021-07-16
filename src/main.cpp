#include "Headers/mainwindow.h"
#include "Headers/SimpleSimulation.h"
#include "Headers/EconomicSimulation.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
