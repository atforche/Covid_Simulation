#include "Headers/ThreadExecution.h"


SimulationWorker::SimulationWorker(Simulation* sim, Speed frameRate) {
    this->sim = sim;
    this->frameRate = frameRate;
    executeLoop = nullptr;
}


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
        executeLoop->start(1); //Unlimited FPS
    } else if (this->frameRate == FAST) {
        executeLoop->start(16); //60 FPS
    } else if (this->frameRate == NORMAL) {
        executeLoop->start(33); //30 FPS
    } else {
        executeLoop->start(65); //15 FPS
    }
}


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


void SimulationWorker::pauseSimulation() {
    // Prevents the timer triggering from causing any effect
    continueSimulation = false;
}


void SimulationWorker::changeSpeed(Speed speedIn) {
    // Enables realtime changing of the timer timeout
    this->frameRate = speedIn;
}


SimulationController::SimulationController(Simulation* sim,
                                           SimulationWorker::Speed frameRate) {

    // Create a new simulation worker
    worker = new SimulationWorker(sim, frameRate);
    this->sim = sim;

    // Move the worker object onto a second thread
    worker->moveToThread(&workerThread);

    // Connect the QThread::finished signal to the necessary functions
    connect(&workerThread, &QThread::finished, sim, &Simulation::renderAgentUpdate);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    // Connect the SimulationController::operator signal to the worker's
    // SimulationWorker::startThread function
    connect(this, &SimulationController::operate, worker,
            &SimulationWorker::startThread);

    // Connect the timestepComplete signal to the updateScreen function
    connect(worker, &SimulationWorker::timestepComplete, this,
            &SimulationController::updateScreen);

    // Start the worker thread, will wait
    workerThread.start();
}


void SimulationController::startSimulation() {
    // Emits a signal that will be picked up by the SimulationWorker
    emit this->operate("Begin");
}


void SimulationController::pauseSimulation() {
    // May cause a slight race condition
    this->worker->pauseSimulation();
}


void SimulationController::changeSpeed(SimulationWorker::Speed newSpeed) {
    // Pause the sim, change the speed, and restart the sim
    pauseSimulation();
    this->worker->changeSpeed(newSpeed);
    startSimulation();
}


SimulationController::~SimulationController() {
    workerThread.quit();
    workerThread.wait();
}


void SimulationController::updateScreen(const QString &) {
    sim->renderAgentUpdate();
    return;
}
