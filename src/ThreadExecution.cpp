#include "Headers/ThreadExecution.h"


SimulationWorker::SimulationWorker(Simulation* sim, Speed frameRate) {
    this->sim = sim;
    this->frameRate = frameRate;
}


void SimulationWorker::doWork(const QString &) {
    executeLoop = new QTimer();
    connect(executeLoop, &QTimer::timeout, this,
            &SimulationWorker::executeSimulation);
    executeLoop->start(16);
}


void SimulationWorker::executeSimulation() {
    sim->execute();
    emit resultReady("Done");
}


void SimulationWorker::dropSimulation() {
    // Stops the timer from executing which prevents the further calling
    // of the Simulation::execute() function
    executeLoop->stop();
}


SimulationController::SimulationController(Simulation* sim,
                                           SimulationWorker::Speed frameRate) {
    worker = new SimulationWorker(sim, frameRate);

    this->sim = sim;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, sim, &Simulation::renderAgentUpdate);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &SimulationController::operate, worker,
            &SimulationWorker::doWork);
    connect(worker, &SimulationWorker::resultReady, this,
            &SimulationController::handleResults);
    workerThread.start();
}


void SimulationController::startSimulation() {
    emit this->operate("Begin");
}


void SimulationController::endSimulation() {
    // This is allowable because it operates more quickly than the signal
    // mechanism. Although the SimulationWorker lives on another thread, its
    // own memory is not being modified by that thread
    this->worker->dropSimulation();
}


SimulationController::~SimulationController() {
    workerThread.quit();
    workerThread.wait();
}


void SimulationController::handleResults(const QString &) {
    sim->renderAgentUpdate();
    return;
}
