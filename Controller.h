#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "PhysicsSimulator.h"
#include <QDebug>

#include "mainwindow.h"

class Controller
{
public:
    Controller();
    ~Controller();
    void setWindow(MainWindow *w) { win = w; }
    void updateModelMat(float scale, btVector3 transl);         // Physics -> FC -> MainWindow
    void runSimStep();
    //void stopSim();
    void restartSim();

private:
    MainWindow *win;
    PhysicsSimulator *physSim;
    bool simRunning;
};

#endif //CONTROLLER_H
