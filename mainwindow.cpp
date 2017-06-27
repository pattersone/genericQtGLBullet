#include <QtWidgets>

#include "mainwindow.h"
#include "bin/ui_mainwindow.h"
#include "Controller.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        glWidget(new MyQOpenGLWidget(this)),
        ctrl(0),
        timer(new QTimer(this))

{
    ui->setupUi(this);
    ui->btnAndGlVLayout->addWidget(glWidget);
    QString text = QString("Slider") + QString::number(ui->sliderSlider->value());
    ui->sliderLabel->setText(text);

    connect(ui->startSimBtn, SIGNAL(clicked()), this, SLOT(startSimSlot()));
    connect(ui->resetBtn, SIGNAL(clicked()), this, SLOT(resetSlot()));

    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
}

//----------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
    delete glWidget;
}

//----------------------------------------------------------------
void MainWindow::timerSlot()
{
    ctrl->runSimStep();
    glWidget->repaint();
}

//----------------------------------------------------------------
void MainWindow::startSimSlot()
{
    timer->start(10);       // This sets the timeout at 10 msec for a refresh of 100Hz; currently, simStep would be tied to this same update.
    //timer->start(1);      // This would run it about as fast as it would; repaints are suggested but may only be scheduled as can be handled.
    ctrl->restartSim();
}

//----------------------------------------------------------------
void MainWindow::resetSlot()
{
    timer->stop();

    glWidget->initShaders();
    glWidget->initVertexBuffers();
    glWidget->resetModelMat();
    glWidget->repaint();
}

//----------------------------------------------------------------
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
}
