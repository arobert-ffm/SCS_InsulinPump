// 
// File: Tracer.cpp
// 
// Date: 24.12.14 17:11
// 
// Generated by: Idatto, version 1.3
// 
// Description:  Writing to a logfile
//               Acoustic and haptic signalling
//
// Author:       Sven Sperner, sillyconn@gmail.com

#include "Tracer.h"
#include <QString>
#include <iostream>

using namespace std;



Tracer::Tracer()
{
    FileName = LOGFILE_NAME;

    LogFile = new QFile(FileName);
    LogFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}

Tracer::~Tracer()
{
    LogFile->flush();
    LogFile->close();
}

// Writes the message to the log file. Every Message is signed by date and time.
// When writing to file has finished, “True” is returned
bool Tracer::writeStatusLog(QString message)
{
    QTextStream TextStream(LogFile);
    TextStream << QDateTime::currentDateTime().toString("yyyy.MM.dd-HH:mm:ss") << " INFO: " << message << endl;

    // Update UI
    emit writeStatusLogInUi(message);
    return true;
}

// Writes the message to the log file. Every Message is signed by date and time.
// When writing to file has finished, “True” is returned
bool Tracer::writeWarningLog(QString message)
{
    QTextStream TextStream(LogFile);
    TextStream << QDateTime::currentDateTime().toString("yyyy.MM.dd-HH:mm:ss") << " WARNING: " << message << endl;

    // Update UI
    emit writeWarningLogInUi(message);
    return true;
}

// Writes the message to the log file. Every Message is signed by date and time.
// When writing to file has finished, “True” is returned
bool Tracer::writeCriticalLog(QString message)
{
    QTextStream TextStream(LogFile);
    TextStream << QDateTime::currentDateTime().toString("yyyy.MM.dd-HH:mm:ss") << " CRITICAL: " << message << endl;

    // Update UI
    emit writeCriticalLogInUi(message);
    return true;
}

// Plays an acoustic sound and returns “True” when done
bool Tracer::playAcousticWarning()
{
    QApplication::beep();
    return true;
}

// vibrates on a specific event and returns “True” when done
bool Tracer::vibrationWarning()
{
    cout << "vibrating...";
    return true;
}




