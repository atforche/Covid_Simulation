QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/AgentController.cpp \
    src/ChartHelpers.cpp \
    src/EconomicAgent.cpp \
    src/EconomicController.cpp \
    src/EconomicLocation.cpp \
    src/EconomicSimulation.cpp \
    src/PandemicAgent.cpp \
    src/PandemicController.cpp \
    src/PandemicLocation.cpp \
    src/PandemicSimulation.cpp \
    src/mainwindow.cpp \
    src/Agent.cpp \
    src/Coordinate.cpp \
    src/Location.cpp \
    src/Region.cpp \
    src/SimpleSimulation.cpp \
    src/Simulation.cpp \
    src/SquareRegion.cpp \
    src/ThreadExecution.cpp \
    src/main.cpp \

HEADERS += \
    Headers/AgentController.h \
    Headers/EconomicAgent.h \
    Headers/EconomicController.h \
    Headers/EconomicLocation.h \
    Headers/EconomicSimulation.h \
    Headers/PandemicAgent.h \
    Headers/PandemicController.h \
    Headers/PandemicLocation.h \
    Headers/PandemicSimulation.h \
    Headers/mainwindow.h \
    Headers/Agent.h \
    Headers/Coordinate.h \
    Headers/Location.h \
    Headers/Region.h \
    Headers/SimpleSimulation.h \
    Headers/Simulation.h \
    Headers/SquareRegion.h \
    Headers/ThreadExecution.h \
    Headers/ChartHelpers.h


FORMS += \
    mainwindow.ui

RESOURCES = resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
