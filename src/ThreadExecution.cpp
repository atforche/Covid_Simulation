#include "Headers/ThreadExecution.h"


SimulationWorker::SimulationWorker(Simulation* sim, Speed frameRate) {
    this->sim = sim;
    this->frameRate = frameRate;
    executeLoop = nullptr;
}


void SimulationWorker::startThread(const QString &) {
    delete executeLoop; // ensure old timer is deleted to prevent two timers running
    executeLoop = new QTimer();
    continueSimulation = true; // Ensure the simulation is able to continue
    connect(executeLoop, &QTimer::timeout, this,
            &SimulationWorker::executeSimTimestep);
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
    if (continueSimulation) {
        sim->execute();
        emit timestepComplete("Done");
    } else {
        executeLoop->stop();
        delete executeLoop;
        executeLoop = nullptr;
    }
}


void SimulationWorker::pauseSimulation() {
    // Stops the timer from executing which prevents the further calling
    // of the Simulation::execute() function
    continueSimulation = false;
}


void SimulationWorker::changeSpeed(Speed speedIn) {
    this->frameRate = speedIn;
}


SimulationController::SimulationController(Simulation* sim,
                                           SimulationWorker::Speed frameRate) {
    worker = new SimulationWorker(sim, frameRate);

    this->sim = sim;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, sim, &Simulation::renderAgentUpdate);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &SimulationController::operate, worker,
            &SimulationWorker::startThread);
    connect(worker, &SimulationWorker::timestepComplete, this,
            &SimulationController::updateScreen);
    workerThread.start();
}


void SimulationController::startSimulation() {
    emit this->operate("Begin");
}


void SimulationController::pauseSimulation() {
    // This is allowable because it operates more quickly than the signal
    // mechanism. Although the SimulationWorker lives on another thread, its
    // own memory is not being modified by that thread
    this->worker->pauseSimulation();
}


void SimulationController::changeSpeed(SimulationWorker::Speed newSpeed) {
    pauseSimulation(); //Pause the simulation
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
