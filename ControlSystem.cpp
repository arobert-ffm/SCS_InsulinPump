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



// The constructor instantiates all necessary objects
// and connects some signals to slot for the user interface
ControlSystem::ControlSystem(UserInterface* ui)
{
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

// Checks the operation hours of the system and returns the value in hours
int ControlSystem::checkOperationHours()
{
    qint64 OperationTime = TheScheduler->getOperationTime();

    if(OperationTime > (MAX_OPERATION_HOURS*60*60*1000))
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

// Checks the scheduler for correct operation and returns “True” when 
// everything is working fine
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

// Checks the hormone reservoir and returns “True” when everything is fine
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

// Checks the tracer and returns “True” when everything is fine
bool ControlSystem::checkTracer()
{
    if(!TheTracer->getStatus())
    {
        cout << "The tracer is in a critical state.";
        return false;
    }

    return true;
}

// Checks the batteries charging state and returns the value in percent
int ControlSystem::checkBatteryStatus()
{
    int BatteryStatus = ThePump->checkPumpBatteryStatus();

    if(BatteryStatus < BATTERY_MIN_LOAD)
    {
        QString msg = "The batteries charging state is to low (" + QString::number(BatteryStatus)+ "%).";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
        return EXIT_FAILURE;
    }

    return BatteryStatus;
}

// Returns the used scheduler
Scheduler* ControlSystem::getScheduler()
{
    return TheScheduler;
}




