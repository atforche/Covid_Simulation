#ifndef THREADEXECUTION_H
#define THREADEXECUTION_H

#include "QObject"
#include "QThread"
#include "QTimer"
#include "windows.h"
#include "Simulation.h"

/** File that handles all classes required for multithreaded execution of the
 * the simulation to prevent blocking of the UI
*/

/**
 * @brief The SimulationWorker class
 * Class that handles the execution of the Simulation on a Threader. A Worker
 * is dispatched by a controlled
 */
class SimulationWorker : public QObject {
    Q_OBJECT

public:

    /** Enum that defines the three different framerates*/
    enum Speed {PAUSED, SLOW, NORMAL, FAST, UNLIMITED};

    /**
     * @brief SimulationWorker
     * Constructor for the SimulationWorker class. Each simulation worker
     * executes a loop in which the Simulation takes timesteps forward. After
     * each timestep is complete, the Worker signals to the Simulation to
     * render the updates to the GUI because GUI updates must be in the main
     * thread
     * @param sim: pointer to the Simulation the worker is driving
     * @param frameRate: speed enum to define how long between sim executions
     */
    SimulationWorker(Simulation* sim, Speed frameRate);

    /**
     * @brief dropSimulation
     * Function that instructs Thread Work to drop execution of the Simulation
     * Must be called by the Thread Controller, which is signaled by the reset
     * button in the GUI
     */
    void dropSimulation();

    /**
     * @brief changeSpeed
     * Function that changes the interval of the QTimer that specifies the
     * FPS of the simulation execution. Function should only be called by the
     * SimulationController in charge of this worker. Sim must be paused and
     * restarted for changes to take effect
     * @param speedIn: enum specifying the new speed of the simulation
     */
    void changeSpeed(Speed speedIn);

public slots:
    /**
     * @brief doWork
     * Function to be attached to a separate thread. Drives the simulation's
     * execution on a time loop
     * @param parameter: the status of the thread
     */
    void doWork(const QString &);

    /**
     * @brief executeSimulation
     * Slot that directly executes the simulation. Signals to the parent
     * Simulation of this worker to execute a timestep
     */
    void executeSimulation();

signals:
    /** Signal that indicate that the thread has finished execution */
    void resultReady(const QString &result);

private:

    /** Pointer to the Simulation object the worker is driving */
    Simulation* sim;

    /** Speed enum to define time between Sim executions */
    SimulationWorker::Speed frameRate;

    /** QTimer object that calls the execute loop after a certain
        amount of time has passed */
    QTimer* executeLoop;

    /** Boolean that indicates whether or not to continue the Simulation */
    bool continueSimulation;

};

/**
 * @brief The SimulationController class
 * Class that controls the simulation and delegates it to a thread. Also
 * controls pausing and resuming thread execution
 */
class SimulationController : public QObject {
    Q_OBJECT

private:

    /** Second thread on which the simulation will be executed*/
    QThread workerThread;

    /** Simulation the controller is controlling */
    Simulation* sim;

    /** Pointer to the worker controlling execution on the second thread */
    SimulationWorker* worker;

public:

    /**
     * @brief SimulationController
     * Constructor for the Simulation Controller class. The simulation
     * controller class delegates SimulationWorkers to threads and assigns
     * tasks to the different workers to drive the simulation forward. Currently
     * only one worker is allocated
     */
    SimulationController(Simulation* sim, SimulationWorker::Speed frameRate);

    /**
     * @brief startSimulation
     * Function to emit a signal to the SimulationWorker thread to begin
     * execution of the Simulation on a separate thread
     */
    void startSimulation();

    /**
     * @brief endSimulation
     * Function that calls the dropSimulation() function on the Simulation
     * Worker. This prevents the Worker from entering it's next step in
     * execution
     */
    void endSimulation();

    /**
     * @brief changeSpeed
     * This function changes the speed of the simulation being run by the
     * Simulation Worker. Accomplishes this by pausing the SimulationWorker,
     * calling SimulationWorker::changeSpeed, then restarting the Simulation
     * @param newSpeed: enum specifying the new speed of the simulation
     */
    void changeSpeed(SimulationWorker::Speed newSpeed);

    /** Destructor for the Simulation Controller class */
    ~SimulationController();

public slots:

    /**
     * @brief handleResults
     * Function to handle the results of the threads execution and clean up
     */
    void handleResults(const QString &);


signals:

    /**
     * @brief operate
     * Signal that enables the SimulationWorker to begin performing work
     */
    void operate(const QString &);

};

#endif // THREADEXECUTION_H
