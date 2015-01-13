/**
 * @file:   Scheduler.cpp
 * @class:  Scheduler
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   13.01.2015
 * Created: 24.12.14 17:11 with Idatto, version 1.3
 *
 * @brief:  Triggering the hormone pump
 *          Keep track of operation time
 *
 * Copyright (c) 2015 All Rights Reserved
 */


#include "Scheduler.h"

using namespace std;



/* The constructor initializes the time measurement
 */
Scheduler::Scheduler(Pump *ThePump)
{
    startOperationTimeCounter();
    ConfigFileName = CONFIGFILE_NAME;
    this->ThePump = ThePump;
}

/* The destructor stops the time measurement
 */
Scheduler::~Scheduler()
{
    stopOperationTimeCounter();
}

/* Resets the timer and sets the countdown time according to parameter
 */
bool Scheduler::resetTimer(int time_min)
{
    TotalOperationTime = time_min*60*1000;
    Timer.restart();

    return true;
}

/* Answers ControlSystem’s call for checkScheduler()
 */
bool Scheduler::getStatus()
{
    if(!Timer.isValid())
    {
        return false;
    }

    return true;
}

/* Answers ControlSystem’s call for checkOperationTime()
 */
qint64 Scheduler::getOperationTime()
{
    return TotalOperationTime;
}

/* Triggers the pump which then checks the blood sugar level
 */
bool Scheduler::triggerPump()
{
    if(!ThePump->runPump())
    {
        return false;
    }

    return true;
}

/* Triggers the scheduler to save the systems operation time
 */
bool Scheduler::saveOperationTime()
{
    stopOperationTimeCounter();
    startOperationTimeCounter();

    return true;
}

/* Starts the counter for operation time and returns “True” when successfully
 * started. The value will be read from “TotalOperationTime”.
 */
bool Scheduler::startOperationTimeCounter()
{
    // Start the timer for measuring the operation time
    Timer.start();

    // Read the total operation time and copy the value
    SaveFile = new QSettings(ConfigFileName, QSettings::NativeFormat);
    SaveFile->beginGroup( "InsulinPump" );
    TotalOperationTime = SaveFile->value("TotalOperationTime").toLongLong();
    SaveFile->endGroup();

    return true;
}

/* Stops the counter for operation time and returns “True” when successfully
 * stopped. The value will be written to “TotalOperationHours”.
 */
bool Scheduler::stopOperationTimeCounter()
{
    // Get actual system uptime and add to total operation time
    TotalOperationTime += Timer.elapsed();

    // Save the total operation time into a file
    SaveFile->beginGroup( "InsulinPump" );
    SaveFile->setValue("TotalOperationTime", TotalOperationTime);
    SaveFile->endGroup();

    return true;
}


/* Getter & Setter for the file name of the configuration file
 */
QString Scheduler::getConfigFileName() const
{
    return ConfigFileName;
}

void Scheduler::setConfigFileName(QString value)
{
    ConfigFileName = value;
}

/* Getter & Setter for Flag that thread should run periodically
 */
bool Scheduler::getSchouldRun() const
{
    return SchouldRun;
}

void Scheduler::setSchouldRun(bool value)
{
    SchouldRun = value;
}




