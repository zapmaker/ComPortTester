/****************************************************************
 * mainwindow.cpp
 * by zapmaker
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lcdPwm->setDigitCount(4);
    ui->lcdSpeed->setDigitCount(4);
    ui->lcdTimeCount->setDigitCount(8);
    ui->lcdTimeDiffWheelTurn->setDigitCount(4);
    ui->lcdWheelRotationCount->setDigitCount(4);

    ui->lcdGyroX->setDigitCount(4);
    ui->lcdGyroY->setDigitCount(4);
    ui->lcdGyroZ->setDigitCount(4);


    connect(&port, SIGNAL(setTime(QString)), this, SLOT(setTime(QString)));
    connect(&port, SIGNAL(setGyro(QString, QString, QString)), this, SLOT(setGyro(QString, QString, QString)));
    connect(&port, SIGNAL(setMotor(QString)), this, SLOT(setMotor(QString)));
    connect(&port, SIGNAL(setSpeed(QString, QString, QString)), this, SLOT(setSpeed(QString, QString, QString)));
}

MainWindow::~MainWindow()
{
    port.CloseComport();
    delete ui;
}

void MainWindow::on_btnOne_clicked()
{
    if (port.isPortOpen())
        return;

    port.OpenComport("COM8", "115200");

    info("Port state:%d\n", port.isPortOpen());
}

void MainWindow::setTime(QString currTime)
{
    ui->lcdTimeCount->display(currTime);
}

void MainWindow::setGyro(QString x, QString y, QString z)
{
    ui->lcdGyroX->display(x);
    ui->lcdGyroY->display(y);
    ui->lcdGyroZ->display(z);
}

void MainWindow::setMotor(QString pwmOut)
{
    ui->lcdPwm->display(pwmOut);
}

void MainWindow::setSpeed(QString rotationCount, QString speedOfTrain, QString timeDeltaWheelTurn)
{
    ui->lcdWheelRotationCount->display(rotationCount);
    ui->lcdSpeed->display(speedOfTrain);
    ui->lcdTimeDiffWheelTurn->display(timeDeltaWheelTurn);
}

