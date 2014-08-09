/****************************************************************
 * main.cpp
 * by zapmaker
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/
#include "mainwindow.h"
#include <QApplication>

enum GC_LOG_TYPES
{
    LOG_DEBUG_TYPE = 1,
    LOG_ERROR_TYPE,
    LOG_WARN_TYPE,
    LOG_INFO_TYPE,
    LOG_STATUS_TYPE,
};

void logit(GC_LOG_TYPES type, const char *str, va_list args);

Log4Qt::PatternLayout *p_layout;


int main(int argc, char *argv[])
{
    // setup logging
    Log4Qt::LogManager::rootLogger();
    //Log4Qt::TTCCLayout *p_layout = new Log4Qt::TTCCLayout(Log4Qt::TTCCLayout::ISO8601);
    //Log4Qt::PatternLayout *p_layout = new Log4Qt::PatternLayout(Log4Qt::PatternLayout::TTCC_CONVERSION_PATTERN);
    p_layout = new Log4Qt::PatternLayout("%d %p (%c) - %m%n");
    p_layout->setName(QLatin1String("GC Basic Layout"));
    p_layout->activateOptions();

    // Create a console appender
    Log4Qt::ConsoleAppender *p_appender = new Log4Qt::ConsoleAppender(p_layout, Log4Qt::ConsoleAppender::STDOUT_TARGET);
    p_appender->setName(QLatin1String("GC Basic Console Appender"));
//#ifdef QT_DEBUG
    p_appender->setThreshold(Log4Qt::Level::TRACE_INT);
//#else
//    p_appender->setThreshold(Log4Qt::Level::WARN_INT);
//#endif
    p_appender->activateOptions();
    Log4Qt::Logger::rootLogger()->addAppender(p_appender);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}



//------------------------------
void status(const char *str, ...)
{
    va_list args;

    va_start(args, str );
    logit(LOG_STATUS_TYPE, str, args);
    va_end(args);
}

void err(const char *str, ...)
{
    va_list args;

    va_start(args, str );
    logit(LOG_ERROR_TYPE, str, args);
    va_end(args);
}

void warn(const char *str, ...)
{
    va_list args;

    va_start(args, str );
    logit(LOG_WARN_TYPE, str, args);
    va_end(args);
}

void info(const char *str, ...)
{
    va_list args;

    va_start(args, str );
    logit(LOG_INFO_TYPE, str, args);
    va_end(args);
}

void diag(const char *str, ...)
{
    va_list args;

    va_start(args, str );
    logit(LOG_DEBUG_TYPE, str, args);
    va_end(args);
}

void logit(GC_LOG_TYPES type, const char *str, va_list args)
{
#define PRNTBUFSIZE 500
    char buf[PRNTBUFSIZE];
    buf[PRNTBUFSIZE-1] = '\0';

    vsnprintf(buf, sizeof(buf) - 1, str, args);

    int len = strlen(buf);
    if (len > 0)
    {
        if (len == 1 && (buf[0] == '\r' || buf[0] == '\n'))
        {
            buf[0] = '\0';
        }
        else if (len > 1)
        {
            if (buf[len - 2] == '\r' || buf[len - 2] == '\n')
                buf[len - 2] = '\0';
            else if (buf[len - 1] == '\r' || buf[len - 1] == '\n')
                buf[len - 1] = '\0';
        }
    }
    else if (len == 0)
        return;

    switch (type)
    {
        case LOG_STATUS_TYPE:
            Log4Qt::Logger::logger(LOG_MSG_TYPE_STATUS)->info(buf);
            break;
        case LOG_DEBUG_TYPE:
            Log4Qt::Logger::logger(LOG_MSG_TYPE_DIAG)->debug(buf);
            break;
        case LOG_ERROR_TYPE:
            Log4Qt::Logger::logger(LOG_MSG_TYPE_DIAG)->error(buf);
            break;
        case LOG_WARN_TYPE:
            Log4Qt::Logger::logger(LOG_MSG_TYPE_DIAG)->warn(buf);
            break;
        case LOG_INFO_TYPE:
            Log4Qt::Logger::logger(LOG_MSG_TYPE_DIAG)->info(buf);
            break;
    }
}
