QT       += core gui
QT       += charts

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_CFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/AgentController.cpp \
    src/ChartHelpers.cpp \
    src/DualAgent.cpp \
    src/DualController.cpp \
    src/DualLocation.cpp \
    src/DualSimulation.cpp \
    src/EconomicAgent.cpp \
    src/EconomicController.cpp \
    src/EconomicLocation.cpp \
    src/EconomicSimulation.cpp \
    src/Network.cpp \
    src/PandemicAgent.cpp \
    src/PandemicController.cpp \
    src/PandemicLocation.cpp \
    src/PandemicSimulation.cpp \
    src/SimulationExecution.cpp \
    src/TrainingExecution.cpp \
    src/mainwindow.cpp \
    src/Agent.cpp \
    src/Coordinate.cpp \
    src/Location.cpp \
    src/Region.cpp \
    src/SimpleSimulation.cpp \
    src/Simulation.cpp \
    src/SquareRegion.cpp \
    src/main.cpp \

HEADERS += \
    Headers/AgentController.h \
    Headers/DualAgent.h \
    Headers/DualController.h \
    Headers/DualLocation.h \
    Headers/DualSimulation.h \
    Headers/EconomicAgent.h \
    Headers/EconomicController.h \
    Headers/EconomicLocation.h \
    Headers/EconomicSimulation.h \
    Headers/Network.h \
    Headers/PandemicAgent.h \
    Headers/PandemicController.h \
    Headers/PandemicLocation.h \
    Headers/PandemicSimulation.h \
    Headers/SimulationExecution.h \
    Headers/TrainingExecution.h \
    Headers/mainwindow.h \
    Headers/Agent.h \
    Headers/Coordinate.h \
    Headers/Location.h \
    Headers/Region.h \
    Headers/SimpleSimulation.h \
    Headers/Simulation.h \
    Headers/SquareRegion.h \
    Headers/ChartHelpers.h


FORMS += \
    mainwindow.ui

RESOURCES = resources.qrc

CUDA_INC_DIR = "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\include"
CUDA_LIB_DIR = "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\lib\x64"
CUDA_BIN_DIR = "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\bin"

QMAKE_LFLAGS += -INCLUDE:?warp_size@cuda@at@@YAHXZ
QMAKE_LFLAGS += -INCLUDE:?searchsorted_cuda@native@at@@YA?AVTensor@2@AEBV32@0_N1@Z
QMAKE_LFLAGS += /machine:x64

# Imports from Torch and CUDA
LIBS += "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\lib\x64\*.lib"
INCLUDEPATH += "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\include"

# RELEASE PYTORCH
LIBS += "C:\libtorch\lib\*.lib"
INCLUDEPATH += "C:\libtorch\include"
INCLUDEPATH += "C:\libtorch\include\torch\csrc\api\include"

# DEBUG PYTORCH
#LIBS += "C:\libtorch_debug\lib\*.lib"
#INCLUDEPATH += "C:\libtorch_debug\include"
#INCLUDEPATH += "C:\libtorch_debug\include\torch\csrc\api\include"


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
