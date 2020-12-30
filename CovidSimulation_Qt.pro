QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/Agent.cpp \
    src/AgentGenerator.cpp \
    src/Coordinate.cpp \
    src/Location.cpp \
    src/LocationGenerator.cpp \
    src/Region.cpp \
    src/SimpleSimulation.cpp \
    src/Simulation.cpp \
    src/SquareRegion.cpp \
    src/ThreadExecution.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    Headers/Agent.h \
    Headers/AgentGenerator.h \
    Headers/Coordinate.h \
    Headers/Location.h \
    Headers/LocationGenerator.h \
    Headers/Region.h \
    Headers/SimpleSimulation.h \
    Headers/Simulation.h \
    Headers/SquareRegion.h \
    Headers/ThreadExecution.h \
    Headers/mainwindow.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
