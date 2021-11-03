#include "Headers/SimulationExecution.h"
#include "Headers/Simulation.h"
#include "Headers/SimpleSimulation.h"
#include "Headers/EconomicSimulation.h"
#include "Headers/Network.h"

SimulationWorker::SimulationWorker(Simulation* sim, Speed frameRate) {
    this->sim = sim;
    this->frameRate = frameRate;
    executeLoop = nullptr;
}


//******************************************************************************


SimulationWorker::~SimulationWorker() {
    delete executeLoop;
}


//******************************************************************************


void SimulationWorker::pauseSimulation() {
    // Prevents the timer triggering from causing any effect
    continueSimulation = false;
}


//******************************************************************************


void SimulationWorker::changeSpeed(Speed speedIn) {
    // Enables realtime changing of the timer timeout
    this->frameRate = speedIn;
}


//******************************************************************************


void SimulationWorker::startThread(const QString &) {
    // Ensure old timer is deleted to prevent two timers running
    delete executeLoop;

    // Allow the simulation to continue executing
    continueSimulation = true;

    // Create a new timer and connect it to the executeSimTimestep function
    executeLoop = new QTimer();
    connect(executeLoop, &QTimer::timeout, this,
            &SimulationWorker::executeSimTimestep);

    // Set the timeout milliseconds based on the simulation speed
    if (this->frameRate == UNLIMITED) {
        if (sim->checkDebug("headless mode")) {
            executeLoop->start(2);
        } else {
            executeLoop->start(5);
        }
    } else if (this->frameRate == FAST) {
        executeLoop->start(16); //60 FPS
    } else if (this->frameRate == NORMAL) {
        executeLoop->start(33); //30 FPS
    } else {
        executeLoop->start(65); //15 FPS
    }
}


//******************************************************************************


void SimulationWorker::executeSimTimestep() {
    // Execute timesetp and inform screen to update rendering
    if (continueSimulation) {
        sim->execute();
        emit timestepComplete("Done");
    } else {
        // Otherwise stop the timer and wait
        executeLoop->stop();
        delete executeLoop;
        executeLoop = nullptr;
    }
}


//******************************************************************************


SimulationController::SimulationController(Simulation* sim,
                                           SimulationWorker::Speed frameRate) {

    // Create a new simulation worker
    worker = new SimulationWorker(sim, frameRate);
    this->sim = sim;

    // Initialize the speed of the Simulation
    this->currentSpeed = frameRate;

    // Move the worker object onto a second thread
    worker->moveToThread(&workerThread);

    // Connect the QThread::finished signal to the necessary functions
    connect(&workerThread, &QThread::finished, sim, &Simulation::renderAgentUpdate);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    // Connect the SimulationController::operator signal to the worker's
    // SimulationWorker::startThread function
    connect(this, &SimulationController::beginSim, worker,
            &SimulationWorker::startThread);

    // Connect the timestepComplete signal to the updateScreen function
    connect(worker, &SimulationWorker::timestepComplete, this,
            &SimulationController::updateScreen);

    // Connect the updateAgeChart signal from the simulation to the renderCharts
    // member function to enable dynamic chart updating
    EconomicSimulation* simulation = dynamic_cast<EconomicSimulation*>(sim);
    if (simulation != nullptr) {
        connect(simulation, &Simulation::updateChart, simulation, &EconomicSimulation::renderCharts);
    } else {
        connect(sim, &Simulation::updateChart, sim, &Simulation::renderCharts);
    }

    // Start the worker thread, will wait
    workerThread.start();
}


//******************************************************************************


SimulationController::~SimulationController() {
    workerThread.quit();
    workerThread.wait();
}


//******************************************************************************


void SimulationController::startSimulation() {
    // Emits a signal that will be picked up by the SimulationWorker
    emit this->beginSim("Begin");
}


//******************************************************************************


void SimulationController::pauseSimulation() {
    // May cause a slight race condition (probably fine tho)
    this->worker->pauseSimulation();
}


//******************************************************************************


void SimulationController::changeSpeed(SimulationWorker::Speed newSpeed) {
    // Pause the sim, change the speed, and restart the sim
    pauseSimulation();
    this->worker->changeSpeed(newSpeed);
    this->currentSpeed = newSpeed;
    startSimulation();
}


//******************************************************************************


void SimulationController::updateScreen(const QString &) {
    static int i = 0;

    // Render every third frame at 60 FPS
    if (currentSpeed == SimulationWorker::FAST) {
        if (i < 2) {
            i++;
            return;
        } else {
            i = 0;
        }
    }

    // Render every 10th frame at Unlimited FPS
    if (currentSpeed == SimulationWorker::UNLIMITED) {
        if (i < 10) {
            i++;
            return;
        } else {
            i = 0;
        }
    }

    if (!sim->checkDebug("headless mode")) {
        sim->renderAgentUpdate();
    }
    return;
}


//******************************************************************************
