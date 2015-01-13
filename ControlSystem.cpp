/**
 * @file:   ControlSystem.cpp
 * @class:  ControlSystem
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   13.01.2015
 * Created: 24.12.14 17:11 with Idatto, version 1.3
 *
 * @brief:  Check the systems health status
 *          Write via Tracer to logfile & to UI
 *
 * Copyright (c) 2015 All Rights Reserved
 */


#include "ControlSystem.h"

using namespace std;



/* The constructor instantiates all necessary objects
 * and connects some signals to slot for the user interface
 */
ControlSystem::ControlSystem(UserInterface* ui)
{
    SchouldRun = true;
    BatteryMinLoad = BATTERY_MIN_LOAD;
    MaxOperationHours = MAX_OPERATION_HOURS;

    ThePump = new Pump();
    TheScheduler = new Scheduler(ThePump);
    TheTracer = new Tracer();

    // Init UI Callbacks
    QObject::connect(ThePump, SIGNAL(updateInsulinReservoir(float)), ui, SLOT(insulinAmountInReservoirChanged(float)));
    QObject::connect(ThePump, SIGNAL(updateGlucagonReservoir(float)), ui, SLOT(glucagonAmountInReservoirChanged(float)));
    QObject::connect(TheTracer, SIGNAL(writeStatusLogInUi(QString)), ui, SLOT(insertStatusLog(QString)));
    QObject::connect(TheTracer, SIGNAL(writeWarningLogInUi(QString)), ui, SLOT(insertWarningLog(QString)));
    QObject::connect(TheTracer, SIGNAL(writeCriticalLogInUi(QString)), ui, SLOT(insertCriticalLog(QString)));
    QObject::connect(ui, SIGNAL(refillInsulinInPump()), ThePump, SLOT(refillInsulinReservoir()));
    QObject::connect(ui, SIGNAL(refillGlucagonInPump()), ThePump, SLOT(refillGlucagonReservoir()));
    QObject::connect(ThePump, SIGNAL(updateBloodSugarLevel(int,int,int)), ui, SLOT(updateBloodsugarLevel(int,int,int)));
    //QObject::connect(this, SIGNAL(saveOperationTimeInUi(int)), ui, SLOT(insertOprationTime(int)));
}


/* Checks the operation hours of the system
 */
int ControlSystem::checkOperationHours()
{
    qint64 OperationTime = TheScheduler->getOperationTime();

    if(OperationTime > (MaxOperationHours*60*60*1000))
    {
        QString msg = "The maximum operation time (" + QString::number(MAX_OPERATION_HOURS)
                    + "h) is reached (" + QString::number(OperationTime/1000) + "sec).";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
        return EXIT_FAILURE;
    }

    // Update UI with actual operation time
    //emit saveOperationTimeInUi(getOperationTime()/60/60/1000);

    return (OperationTime/60/60/1000);
}

/* Checks the scheduler for correct operation
 */
bool ControlSystem::checkScheduler()
{
    if(!TheScheduler->getStatus())
    {
        QString msg = "The scheduler is in a critical state.";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
        return false;
    }

    return true;
}

/* Checks the hormone reservoir
 */
bool ControlSystem::checkPump()
{
    if(!ThePump->getPumpStatus())
    {
        QString msg = "The pump is in a critical state.";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
        return false;
    }

    return true;
}

/* Checks the tracer
 */
bool ControlSystem::checkTracer()
{
    if(!TheTracer->getStatus())
    {
        cout << "The tracer is in a critical state.";
        return false;
    }

    return true;
}

/* Checks the batteries charging state
 */
int ControlSystem::checkBatteryStatus()
{
    int BatteryStatus = ThePump->checkPumpBatteryStatus();

    if(BatteryStatus < BatteryMinLoad)
    {
        QString msg = "The batteries charging state is to low (" + QString::number(BatteryStatus)+ "%).";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
        return EXIT_FAILURE;
    }

    return BatteryStatus;
}

/* Returns the used scheduler
 */
Scheduler* ControlSystem::getScheduler()
{
    return TheScheduler;
}


/* Getter & Setter for minumum Bettery load level in percent
 */
int ControlSystem::getBatteryMinLoad() const
{
    return BatteryMinLoad;
}

void ControlSystem::setBatteryMinLoad(int value)
{
    BatteryMinLoad = value;
}

/* Getter & Setter for maximum operation time in hours
 */
int ControlSystem::getMaxOperationHours() const
{
    return MaxOperationHours;
}

void ControlSystem::setMaxOperationHours(int value)
{
    MaxOperationHours = value;
}

/* Getter & Setter for Flag that thread should run periodically
 */
bool ControlSystem::getSchouldRun() const
{
    return SchouldRun;
}

void ControlSystem::setSchouldRun(bool value)
{
    SchouldRun = value;
}


