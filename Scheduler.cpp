/**
 * @file:   Scheduler.cpp
 * @class:  Scheduler
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   26.01.2015
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
    SchouldRun = true;
    TotalOperationTime = 0;
    ConfigFileName = CONFIGFILE_NAME;

    this->ThePump = ThePump;

    SaveFile = new QSettings(ConfigFileName, QSettings::NativeFormat);
    readOperationTime();

    startOperationTimeCounter();
}

/* The destructor stops the time measurement
 */
Scheduler::~Scheduler()
{
    stopOperationTimeCounter();
}


/* Answers ControlSystem’s call for checkScheduler()
 */
int Scheduler::getStatus()
{
    if(!Timer.isValid())
    {
        return 1;
    }
    else if(Thread)
    {
        if(!Thread->joinable())
        {
            return 3;
        }
    }
    else
    {
        return 2;
    }

    return 0;
}

/* Answers ControlSystem’s call for checkOperationTime()
 */
quint64 Scheduler::getOperationTime()
{
    TotalOperationTime += Timer.restart();

    emit updateOperationTime(TotalOperationTime/3600000);

    return TotalOperationTime;
}

/* Set the total operation time in milliseconds
 */
void Scheduler::setOperationTime(quint64 milliseconds)
{
    TotalOperationTime = milliseconds;
}

/* Triggers the pump which then checks the blood sugar level
 */
bool Scheduler::triggerPump()
{
    if(ThePump->getPumpStatus())
    {
        if(!ThePump->runPump())
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

/* Returns the actual battery load level
 */
int Scheduler::getBatstatus()
{
    return ThePump->getBatteryPowerLevel();
}

/* Triggers the scheduler to save the systems operation time
 */
bool Scheduler::saveOperationTime()
{
    //getOperationTime();
    writeOperationTime();

    return true;
}

/* Starts the counter for operation time
 */
bool Scheduler::startOperationTimeCounter()
{
    Timer.start();

    return true;
}

/* Stops the counter for operation time
 */
bool Scheduler::stopOperationTimeCounter()
{
    Timer.invalidate();

    return true;
}

/* Reads the total operation time from the config file
 */
void Scheduler::readOperationTime()
{
    SaveFile->beginGroup( "InsulinPump-Dynamic" );
    TotalOperationTime = SaveFile->value("TotalOperationTime").toLongLong();
    SaveFile->endGroup();
    SaveFile->sync();
}

/* Writes the total operation time to the config file
 */
void Scheduler::writeOperationTime()
{
    SaveFile->beginGroup( "InsulinPump-Dynamic" );
    SaveFile->setValue("TotalOperationTime", TotalOperationTime);
    SaveFile->endGroup();
    SaveFile->sync();
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

/* Getter & Setter for the thread object of the schedulling thread
 */
std::thread* Scheduler::getThread() const
{
    return Thread;
}

void Scheduler::setThread(std::thread* value)
{
    Thread = value;
}


/* Slots
 */
void Scheduler::setOperationTimeInHours(int hours)
{
    TotalOperationTime = hours*3600000;

    emit updateOperationTime(hours);
}




