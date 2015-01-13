/**
 * @file:   Tracer.cpp
 * @class:  Tracer
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   13.01.2015
 * Created: 24.12.14 17:11 with Idatto, version 1.3
 *
 * @brief:  Writing to a logfile at different urgency
 *          Also signalling via Beep and/or Vibration
 *
 * Copyright (c) 2015 All Rights Reserved
 */


#include "Tracer.h"

using namespace std;



/* The constructor initializes the logfile
 */
Tracer::Tracer()
{
    // Copy the logfile name and open the file
    LogFileName = LOGFILE_NAME;
    LogFile = new QFile(LogFileName);
    LogFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}

/* The destructor closes the logfile
 */
Tracer::~Tracer()
{
    // Flush the logfile buffer and close the file
    LogFile->flush();
    LogFile->close();
}

/* Writes the message to the log file. Every Message is signed by date and time.
 * When writing to file has finished, “True” is returned
 */
bool Tracer::writeStatusLog(QString message)
{
    // Write status message to logfile
    QTextStream TextStream(LogFile);

    // Add Timestamp and Tag
    QString prefix = QDateTime::currentDateTime().toString("yyyy.MM.dd-HH:mm:ss") + " INFO: ";

    // Write to logfile
    TextStream << prefix << message  << endl;

    // Update UI with actually sent status message
    emit writeStatusLogInUi(message);

    return true;
}

/* Writes the message to the log file. Every Message is signed by date and time.
 * When writing to file has finished, “True” is returned
 */
bool Tracer::writeWarningLog(QString message)
{
    // Write warning message to logfile
    QTextStream TextStream(LogFile);

    // Add Timestamp and Tag
    QString prefix = QDateTime::currentDateTime().toString("yyyy.MM.dd-HH:mm:ss") + " WARNING: ";

    // Write to logfile
    TextStream << prefix << message  << endl;

    // Update UI with actually sent warning message
    emit writeWarningLogInUi(message);

    return true;
}

/* Writes the message to the log file. Every Message is signed by date and time.
 * When writing to file has finished, “True” is returned
 */
bool Tracer::writeCriticalLog(QString message)
{
    // Write critical message to logfile
    QTextStream TextStream(LogFile);

    // Add Timestamp and Tag
    QString prefix = QDateTime::currentDateTime().toString("yyyy.MM.dd-HH:mm:ss") + " CRITICAL: ";

    // Write to logfile
    TextStream << prefix << message  << endl;

    // Update UI with actually sent critical message
    emit writeCriticalLogInUi(message);

    return true;
}

/* Plays an acoustic sound and returns “True” when done
 * When playing a sound has finished, “True” is returned
 */
bool Tracer::playAcousticWarning()
{
    QApplication::beep();
    return true;
}

/* Vibrates on a specific event and returns “True” when done
 * When vibration has finished, “True” is returned
 */
bool Tracer::vibrationWarning()
{
    cout << "vibrating..." << endl;
    return true;
}

/* Answers ControlSystem’s call for checkTracer()
 * Everything is fine, if the logfile is opened & writeable
 */
bool Tracer::getStatus()
{
    if(!LogFile->isOpen() || !LogFile->isWritable())
    {
        return false;
    }

    return true;
}


/* Getter & Setter for the file name of the logfile
 */
QString Tracer::getLogFileName() const
{
    return LogFileName;
}

void Tracer::setLogFileName(QString value)
{
    LogFileName = value;
}




