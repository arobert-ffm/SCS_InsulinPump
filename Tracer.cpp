/**
 * @file:   Tracer.cpp
 * @class:  Tracer
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   20.01.2015
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


/* Writes a signed status message to the log file
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
    emit writeStatusLogInUi(prefix + message);

    return true;
}

/* Writes a signed warning message to the log file
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
    emit writeWarningLogInUi(prefix + message);

    return true;
}

/* Writes a signed critical message to the log file
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
    emit writeCriticalLogInUi(prefix + message);

    return true;
}

/* Plays an acoustic sound
 */
bool Tracer::playAcousticWarning()
{
    QApplication::beep();
    return true;
}

/* Simulates vibration
 */
bool Tracer::vibrationWarning()
{
    cout << "vibrating..." << endl;
    return true;
}

/* Answers ControlSystem’s call for checkTracer()
 */
int Tracer::getStatus()
{
    if(!LogFile->isOpen())
    {
        return 1;
    }

    if(!LogFile->isWritable())
    {
        return 2;
    }

    return 0;
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




