// 
// File: ControlSystem.cpp
// 
// Date: 24.12.14 17:11
// 
// Generated by: Idatto, version 1.3
// 
// Description:  Check the status of the other classes
//
// Author:       Sven Sperner, sillyconn@gmail.com


#include "ControlSystem.h"

using namespace std;



ControlSystem::ControlSystem(UserInterface* ui)
{
    // Init UI Callbacks
    QObject::connect(&pump, SIGNAL(updateInsulinReservoir(float)), ui, SLOT(insulinAmountInReservoirChanged(float)));
    QObject::connect(&pump, SIGNAL(updateGlucagonReservoir(float)), ui, SLOT(glucagonAmountInReservoirChanged(float)));
    QObject::connect(&tracer, SIGNAL(writeStatusLogInUi(string&)), ui, SLOT(insertStatusLog(string&)));
    QObject::connect(&tracer, SIGNAL(writeWarningLogInUi(string&)), ui, SLOT(insertWarningLog(string&)));
    QObject::connect(&tracer, SIGNAL(writeCriticalLogInUi(string&)), ui, SLOT(insertCriticalLog(string&)));
    QObject::connect(ui, SIGNAL(refillInsulinInPump()), &pump, SLOT(refillInsulin()));
    QObject::connect(ui, SIGNAL(refillGlucagonInPump()), &pump, SLOT(refillGlucagon()));
}

// Checks the operation hours of the mechanical parts (motor) and returns the 
// value in hours. 
int ControlSystem::checkOperationHours()
{
    scheduler.getOperationTime();
}

// Checks the scheduler for correct operation and returns “True” when 
// everything is working fine! 
bool ControlSystem::checkScheduler()
{
    scheduler.getStatus();

    return true;
}

// Checks the hormone reservoir and returns “True” when everything is fine
bool ControlSystem::checkPump()
{
    pump.getStatus();

    return true;
}

// Checks the tracer and returns “True” when everything is fine!
bool ControlSystem::checkTracer()
{
    //tracer.getStatus();

    return true;
}

// checks the batteries charging state and returns the value in percent
int ControlSystem::checkBatteryStatus()
{
    pump.getBatteryStatus();
}




