#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MyQOpenGlWidget.h"

class Controller;

namespace Ui
{ class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Controller* getController() const {return ctrl;}
    void setController(Controller *c) {ctrl = c;}
    MyQOpenGLWidget* getGLWidget() const {return glWidget;}
    void initGL() { glWidget->initShaders(); glWidget->initVertexBuffers(); }

//signals:

public slots:
    void timerSlot();
    void startSimSlot();
    void resetSlot();

protected slots:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    MyQOpenGLWidget *glWidget;
    Controller *ctrl;
    QTimer *timer;
};


#endif // MAINWINDOW_H
