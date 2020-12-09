#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Headers/Simulation.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the MainWindow of the application
     * Creates the UI design for the main window. Initializes the drawing scene
     * and all individual UI elements
     * @param parent: the parent widget to which this window belongs
     */
    MainWindow(QWidget *parent = nullptr);

    /** Destructor for the MainWindow object, clears all memory*/
    ~MainWindow();


private slots:
    /**
     * @brief On Click listener for the runSimulation button
     * Starts the simulation
     */
    void on_runSimulation_clicked();

    /**
     * @brief On Click Listener for the resetSimulation button
     * Resets the simulation to its initial conditions
     */
    void on_resetSimulation_clicked();

    /**
     * @brief on_numLocations_valueChanged
     * On Value Changed listener for numLocations slider
     * @param value: the new value of the slider
     */
    void on_numLocations_valueChanged(int value);

    void on_numAgents_valueChanged(int value);

private:
    /* Pointer to store and access the ui components of the window*/
    Ui::MainWindow *ui;

    /* Pointer to store and access the Simulation that is running*/
    Simulation* sim;
};
#endif // MAINWINDOW_H
