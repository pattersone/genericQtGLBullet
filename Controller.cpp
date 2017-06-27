#include "Controller.h"

Controller::Controller() : win(0), physSim(new PhysicsSimulator(this)), simRunning(false)
{
    physSim->simSetup();
}

//----------------------------------------------------------------
Controller::~Controller()
{
    physSim->simClose();
    delete physSim;
}

//----------------------------------------------------------------
//Called within each physSim->simStep() call to the simulator to pass info back here, which is passed onto the GUI.
void Controller::updateModelMat(float scale, btVector3 transl)
{
    if (win != 0)
    { win->getGLWidget()->updateModelMat(scale, transl); }
    else
    { qDebug() << "MainWindow not yet created."; }
}

//----------------------------------------------------------------
void Controller::runSimStep()
{
    physSim->simStep();
}

//----------------------------------------------------------------
void Controller::restartSim()
{
    physSim->simClose();
    physSim->simSetup();
    simRunning = true;
}

//----------------------------------------------------------------
/*
void Controller::stopSim()
{
    if (simRunning)
    {
         physSim->simClose();
    }
}
*/
