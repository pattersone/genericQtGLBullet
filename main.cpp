#include "mainwindow.h"
#include "Controller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller ctrl;
    MainWindow win;

    ctrl.setWindow(&win);
    win.setController(&ctrl);
    win.show();

    return a.exec();
}
