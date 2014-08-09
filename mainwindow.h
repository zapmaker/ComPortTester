/****************************************************************
 * mainwindow.h
 * by zapmaker
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>

#include "log4qtdef.h"
#include "rs232.h"

#define LOG_MSG_TYPE_DIAG       "DIAG"
#define LOG_MSG_TYPE_STATUS     "STATUS"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_btnOne_clicked();

    void setTime(QString currTime);
    void setGyro(QString x, QString y, QString z);
    void setMotor(QString pwmOut);
    void setSpeed(QString rotationCount, QString speedOfTrain, QString timeDeltaWheelTurn);


private:
    void send(char *str);

private:
    Ui::MainWindow *ui;
    RS232 port;
};

void status(const char *str, ...);
void diag(const char *str, ...);
void err(const char *str, ...);
void warn(const char *str, ...);
void info(const char *str, ...);

#endif // MAINWINDOW_H
