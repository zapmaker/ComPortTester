/****************************************************************
 * rs232.cpp
 * Based on code from GrblHoming - zapmaker
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#include "rs232.h"

RS232::RS232()
    : port(NULL), detectedEOL(0), detectedLineFeedSize(0)
{
}

RS232::~RS232()
{
    CloseComport();
}

bool RS232::OpenComport(QString commPortStr, QString baudRate)
{
    if (port != NULL)
        CloseComport();

    bool ok;
    BaudRateType baud = (BaudRateType)baudRate.toInt(&ok);
    if (!ok)
    {
        baud = BAUD9600;
    }
    else
    {
        int possibleBaudRates[] = {BAUD110,BAUD300,BAUD600,BAUD1200,BAUD2400,BAUD4800,BAUD9600,BAUD19200,BAUD38400,BAUD57600,BAUD115200};
        int pbrCount = sizeof possibleBaudRates / sizeof possibleBaudRates[0];

        bool found = false;
        for (int i = 0; i < pbrCount; i++)
        {
            if (baud == possibleBaudRates[i])
            {
                found = true;
                break;
            }
        }
        if (!found)
            baud = BAUD9600;
    }

    PortSettings settings = {baud, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};

    port = new QextSerialPort(commPortStr, settings);// default is event driven, alternate:, QextSerialPort::Polling);

    connect(port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));

    port->open(QIODevice::ReadOnly);

    return port->isOpen();
}


void RS232::CloseComport()
{
    if (port != NULL)
    {
        port->close();
        delete port;
        port = NULL;
    }
}

void RS232::Reset() //still to test
{
    if (port != NULL)
        port->reset();
}

bool RS232::isPortOpen()
{
    if (port == NULL)
        return false;

    return port->isOpen();
}

QString RS232::getDetectedLineFeed()
{
    return detectedLineFeed;
}

bool RS232::haveData()
{
    //return responses.size() > 0;
    return false;
}

void RS232::onDataAvailable()
{
    QByteArray a(port->readAll());
    info("Got %d bytes", a.size());
    receivedData.append(a);

    if (detectedEOL == 0)
    {
        int pos = 0;
        char firstEOL = 0;
        char secondEOL = 0;
        for (int i = 0; i < receivedData.size(); i++)
        {
            char b = receivedData.at(i).toLatin1();
            if (b == '\n' || b == '\r')
            {
                if (firstEOL == 0)
                {
                    firstEOL = b;
                    pos = i;
                }
                else if ((pos + 1) == i)
                {
                    secondEOL = b;
                    break;
                }
                else
                    break;
            }
        }

        if (firstEOL != 0)
        {
            if (secondEOL != 0)
            {
                detectedEOL = secondEOL;
                detectedLineFeed = firstEOL;
                detectedLineFeed += secondEOL;
            }
            else
            {
                detectedEOL = firstEOL;
                detectedLineFeed = firstEOL;
            }

            detectedLineFeedSize = detectedLineFeed.size();
        }
    }

    if (detectedEOL)
    {
        // ok, we have EOL, we can no start splitting things up

        // first, check if end of bytes contains our line feed string
        if (receivedData.size() >= detectedLineFeedSize)
        {
            int last = receivedData.lastIndexOf(detectedLineFeed);
            if (last > 0)
            {
                QString s(receivedData.right(last + detectedLineFeedSize));
                QStringList result = s.split(detectedLineFeed, QString::SkipEmptyParts);

                QString currTime;
                bool gotTime = false;

                QString rotationCount;
                QString speedOfTrain;
                QString timeDeltaWheelTurn;
                bool gotSpeed = false;

                QString pwmOut;
                bool gotMotor = false;

                QString gyroX;
                QString gyroY;
                QString gyroZ;
                bool gotGyro = false;

                foreach (QString r, result)
                {
                    //info("Got:%s", r.toLocal8Bit().constData());

                    QStringList items = r.split(",");
                    int count = items.count();
                    if (count >= 4)
                    {
                        int value = atoi(items.at(0).toLocal8Bit().constData());
                        if (value > 0)
                        {
                            char type = items.at(1).toLocal8Bit().constData()[0];
                            switch (type)
                            {
                            case 'S':
                                if (count >= 6)
                                {
                                    //currTime = atoi(items.at(2).toLocal8Bit().constData());
                                    rotationCount = items.at(3);
                                    speedOfTrain = items.at(4);
                                    timeDeltaWheelTurn = items.at(5);
                                    gotSpeed = true;
                                }
                                else
                                {
                                    info("too few items for Speed: %d", count);
                                }
                                break;
                            case 'M':
                                if (count >= 4)
                                {
                                    currTime = items.at(2);
                                    gotTime = true;
                                    pwmOut = items.at(3);
                                    gotMotor = true;
                                }
                                else
                                {
                                    info("too few items for Motor: %d", count);
                                }
                                break;
                            case 'G':
                                if (count >= 6)
                                {
                                    currTime = items.at(2);
                                    gotTime = true;
                                    gyroX = items.at(3);
                                    gyroY = items.at(4);
                                    gyroZ = items.at(5);
                                    gotGyro = true;
                                }
                                else
                                {
                                    info("too few items for Gyro: %d", count);
                                }
                                break;
                            default:
                                info("Unknown code type: %c", type);
                                break;
                            }
                        }
                        else
                        {
                            info("Unknown code:%d", value);
                        }
                    }
                    else
                    {
                        info("Not usable data");
                    }

                }

                if (gotTime)
                {
                    //info("got time");
                    emit setTime(currTime);
                }

                if (gotSpeed)
                {
                    //info("got speed");
                    emit setSpeed(rotationCount, speedOfTrain, timeDeltaWheelTurn);
                }

                if (gotMotor)
                {
                    //info("got motor");
                    emit setMotor(pwmOut);
                }

                if (gotGyro)
                {
                    //info("got gyro");
                    emit setGyro(gyroX, gyroY, gyroZ);
                }

                //responses.append(result);
                receivedData.remove(0, last + detectedLineFeedSize);
            }
        }
    }
}

