/****************************************************************
 * rs232.h
 * Based on GrblHoming - zapmaker
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#ifndef RS232_H
#define RS232_H

#include <QtGlobal>
#include <QObject>

#include <stdio.h>
#include <string.h>
#if defined(Q_OS_LINUX) || defined(Q_OS_MACX) || defined(Q_OS_ANDROID)
#include <termios.h>
#include "../termiosext.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#else
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <Windows.h>
#else
// TODO - R - if I leave out Windows.h then Sleep is not found???
#include <Windows.h>
#include <QtGui/QWindow>
#endif
#endif

void status(const char *str, ...);
void diag(const char *str, ...);
void err(const char *str, ...);
void warn(const char *str, ...);
void info(const char *str, ...);

#include <QMessageBox>
#include <qextserialport.h>
#include <qextserialenumerator.h>

class RS232 : public QObject
{
    Q_OBJECT

public:
    RS232();
    virtual ~RS232();

    //methods
    bool OpenComport(QString commPortStr, QString baudRate);
    void CloseComport();
    void Reset();
    bool isPortOpen();
    QString getDetectedLineFeed();
    bool haveData();

signals:
    void setTime(QString currTime);
    void setGyro(QString x, QString y, QString z);
    void setMotor(QString pwmOut);
    void setSpeed(QString rotationCount, QString speedOfTrain, QString timeDeltaWheelTurn);

public slots:

private slots:
    void onDataAvailable();

private:
    QextSerialPort *port;
    char detectedEOL;
    QString detectedLineFeed;
    int detectedLineFeedSize;
    QString receivedData;
    //QStringList responses;
};


#endif // RS232_H
