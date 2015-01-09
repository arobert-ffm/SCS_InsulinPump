// 
// File: ControlSystem.cpp
// 
// Date: 24.12.14 17:11
// 
// Generated by: Idatto, version 1.3
// 
// Description:  Check the status of the other objects
//
// Author:       Sven Sperner, sillyconn@gmail.com


#include "ControlSystem.h"

using namespace std;



ControlSystem::ControlSystem(UserInterface* ui)
{
    ThePump = new Pump();
    TheScheduler = new Scheduler(ThePump);
    TheTracer = new Tracer();

    // Init UI Callbacks
    QObject::connect(ThePump, SIGNAL(updateInsulinReservoir(float)), ui, SLOT(insulinAmountInReservoirChanged(float)));
    QObject::connect(ThePump, SIGNAL(updateGlucagonReservoir(float)), ui, SLOT(glucagonAmountInReservoirChanged(float)));
    QObject::connect(TheTracer, SIGNAL(writeStatusLogInUi(string&)), ui, SLOT(insertStatusLog(string&)));
    QObject::connect(TheTracer, SIGNAL(writeWarningLogInUi(string&)), ui, SLOT(insertWarningLog(string&)));
    QObject::connect(TheTracer, SIGNAL(writeCriticalLogInUi(string&)), ui, SLOT(insertCriticalLog(string&)));
    QObject::connect(ui, SIGNAL(refillInsulinInPump()), ThePump, SLOT(refillInsulin()));
    QObject::connect(ui, SIGNAL(refillGlucagonInPump()), ThePump, SLOT(refillGlucagon()));
}

// Checks the operation hours of the system and returns the value in hours
int ControlSystem::checkOperationHours()
{
    qint64 OperationTime = TheScheduler->getOperationTime();

    if(OperationTime > (MAX_OPERATION_HOURS*60*1000))
    {
        string msg = "The maximum operation time is reached.";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
        return EXIT_FAILURE;
    }

    return (OperationTime/60/1000);
}

// Checks the scheduler for correct operation and returns “True” when 
// everything is working fine
bool ControlSystem::checkScheduler()
{
    if(!TheScheduler->getStatus())
    {
        string msg = "The scheduler is in a critical state.";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
        return false;
    }

    return true;
}

// Checks the hormone reservoir and returns “True” when everything is fine
bool ControlSystem::checkPump()
{
    if(!ThePump->getStatus())
    {
        string msg = "The pump is in a critical state.";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
        return false;
    }

    return true;
}

// Checks the tracer and returns “True” when everything is fine
bool ControlSystem::checkTracer()
{
/*
    if(!TheTracer->getStatus())
    {
        cout << "The tracer is in a critical state.";
        return false
    }
*/

    return true;
}

// Checks the batteries charging state and returns the value in percent
int ControlSystem::checkBatteryStatus()
{
    int BatteryStatus = ThePump->getBatteryStatus();

    if(BatteryStatus < BATTERY_MIN_LOAD)
    {
        string msg = "The batteries charging state is to low.";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
        return EXIT_FAILURE;
    }

    return BatteryStatus;
}




