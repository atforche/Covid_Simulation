#ifndef THREADEXECUTION_H
#define THREADEXECUTION_H

#include "windows.h"
#include "QObject"
#include "QThread"
#include "QTimer"

#include "Simulation.h"
//#include "EconomicSimulation.h"

/**
 * @brief The SimulationWorker class \n
 * Class that handles the execution of the Simulation on a Threader. A Worker
 * is dispatched by a controlled
 */
class SimulationWorker : public QObject {
    Q_OBJECT

public:

    /** Enum that defines the different simulation framerates*/
    enum Speed {PAUSED, SLOW, NORMAL, FAST, UNLIMITED};

    /**
     * @brief SimulationWorker \n
     * Constructor for the SimulationWorker class. Each simulation worker
     * executes a loop in which the Simulation takes timesteps forward. After
     * each timestep is complete, the Worker signals to the Simulation to
     * render the updates to the GUI because GUI updates must be in the main
     * thread
     * @param sim: pointer to the Simulation the worker is driving
     * @param frameRate: speed enum to define how long between sim executions
     */
    SimulationWorker(Simulation* sim, Speed frameRate);

    /** Destructor for the SimulationWorker class */
    ~SimulationWorker();

    /**
     * @brief pauseSimulation \n
     * Function that instructs SimulationWorker to pause execution of the Simulation
     * Must be called by the Thread Controller, which is signaled by the reset
     * or pause button in the GUI
     * @param reset: whether the simulation is being reset or not
     */
    void pauseSimulation(bool reset = false);

    /**
     * @brief changeSpeed \n
     * Function that changes the interval of the QTimer that specifies the
     * FPS of the simulation execution. Function should only be called by the
     * SimulationController in charge of this worker. Sim must be paused and
     * restarted for changes to take effect
     * @param speedIn: enum specifying the new speed of the simulation
     */
    void changeSpeed(Speed speedIn);

public slots:
    /**
     * @brief startThread \n
     * Function to be attached to a separate thread. Drives the simulation's
     * execution on a time loop
     * @param parameter: the status of the thread
     */
    void startThread(const QString &);

    /**
     * @brief executeSimulation \n
     * Slot that directly executes the simulation. Signals to the parent
     * Simulation of this worker to execute a timestep
     */
    void executeSimTimestep();

signals:
    /** Signal that indicate that the thread has finished execution */
    void timestepComplete(const QString &result);

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
 * @brief The SimulationController class \n
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
     * @brief SimulationController \n
     * Constructor for the Simulation Controller class. The simulation
     * controller class delegates SimulationWorkers to threads and assigns
     * tasks to the different workers to drive the simulation forward. Currently
     * only one worker is allocated
     */
    SimulationController(Simulation* sim, SimulationWorker::Speed frameRate);

    /** Destructor for the Simulation Controller class */
    ~SimulationController();

    /**
     * @brief startSimulation \n
     * Function to emit a signal to the SimulationWorker thread to begin
     * execution of the Simulation on a separate thread
     */
    void startSimulation();

    /**
     * @brief endSimulation \n
     * Function that calls the dropSimulation() function on the Simulation
     * Worker. This prevents the Worker from entering it's next step in
     * execution
     * @param reset: whether the sim is being reset
     */
    void pauseSimulation(bool reset = false);

    /**
     * @brief changeSpeed \n
     * This function changes the speed of the simulation being run by the
     * Simulation Worker. Accomplishes this by pausing the SimulationWorker,
     * calling SimulationWorker::changeSpeed, then restarting the Simulation
     * @param newSpeed: enum specifying the new speed of the simulation
     */
    void changeSpeed(SimulationWorker::Speed newSpeed);

public slots:

    /**
     * @brief updateScreen \n
     * Function to handle the results of the previous timestep and render
     * those results to the screen
     */
    void updateScreen(const QString &);


signals:

    /**
     * @brief beginSim \n
     * Signal that enables the SimulationWorker to begin performing work
     */
    void beginSim(const QString &);

};

#endif // THREADEXECUTION_H
