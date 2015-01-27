/**
 * @file:   ControlSystem.cpp
 * @class:  ControlSystem
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   27.01.2015
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

    TheTracer = new Tracer();

    if(readConfiguration(CONFIGFILE_NAME))
    {
        ThePump = new Pump(TheTracer, Sensitivity, UpperLevel, LowerLevel, UpperLimit, LowerLimit);
    }
    else
    {
        ThePump = new Pump();
    }

    TheScheduler = new Scheduler(ThePump);

    // Init UI Callbacks
    QObject::connect(ThePump, SIGNAL(updateBatteryPowerLevel(int)), ui, SLOT(batteryPowerLevelChanged(int)));
    QObject::connect(ui, SIGNAL(setBatteryPowerLevel(int)), ThePump, SLOT(changeBatteryPowerLevel(int)));
    QObject::connect(ThePump, SIGNAL(updateInsulinReservoir(int)), ui, SLOT(insulinAmountInReservoirChanged(int)));
    QObject::connect(ui, SIGNAL(refillInsulinInPump()), ThePump, SLOT(refillInsulinReservoir()));
    QObject::connect(ThePump, SIGNAL(updateGlucagonReservoir(int)), ui, SLOT(glucagonAmountInReservoirChanged(int)));
    QObject::connect(ui, SIGNAL(refillGlucagonInPump()), ThePump, SLOT(refillGlucagonReservoir()));
    QObject::connect(ThePump, SIGNAL(updateBloodSugarLevel(int)), ui, SLOT(updateBloodsugarLevel(int)));
    QObject::connect(ThePump, SIGNAL(updateHormoneInjectionLog(int,int)), ui, SLOT(updateHormoneInjectionLog(int,int)));

    QObject::connect(TheScheduler, SIGNAL(updateOperationTime(int)), ui, SLOT(operationTimeChanged(int)));
    QObject::connect(ui, SIGNAL(setOperationTime(int)), TheScheduler, SLOT(setOperationTimeInHours(int)));

    QObject::connect(TheTracer, SIGNAL(writeStatusLogInUi(QString)), ui, SLOT(insertStatusLog(QString)));
    QObject::connect(TheTracer, SIGNAL(writeWarningLogInUi(QString)), ui, SLOT(insertWarningLog(QString)));
    QObject::connect(TheTracer, SIGNAL(writeCriticalLogInUi(QString)), ui, SLOT(insertCriticalLog(QString)));

    QObject::connect(this, SIGNAL(updateMinBatteryLevel(int)), ui, SLOT(minBatteryLevelChanged(int)));
    QObject::connect(ui, SIGNAL(setMinBatteryLevel(int)), this, SLOT(setBatteryMinLoad(int)));
    QObject::connect(this, SIGNAL(updateMaxOperationTime(int)), ui, SLOT(maxOperationTimeChanged(int)));
    QObject::connect(ui, SIGNAL(setMaxOperationTime(int)), this, SLOT(setMaxOperationHours(int)));

    // Refill hormone reservoirs
    ThePump->refillGlucagonReservoir();
    ThePump->refillInsulinReservoir();
}


/* Checks the operation hours of the system
 */
int ControlSystem::checkOperationHours()
{
    if( OperationTime == TheScheduler->getOperationTime())
    {
        QString msg = "The operation time has not changed since the last cycle";
        TheTracer->writeWarningLog(msg);
    }
    else
    {
        OperationTime = TheScheduler->getOperationTime();
    }

    uint OperationHours = OperationTime/3600000;
    if(OperationHours > MaxOperationHours)
    {
        QString msg = "The maximum operation time (" + QString::number(MaxOperationHours)
                    + "h) is reached (" + QString::number(OperationHours) + "h).";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
    }
    else if(OperationHours > (MaxOperationHours*0.9))
    {
        QString msg = "The actual operation time (" + QString::number(MaxOperationHours) +
                      "h) is near maximum (" + QString::number(OperationHours) + "h).";
        TheTracer->writeWarningLog(msg);
    }

    return (int)OperationHours;
}

/* Checks the scheduler for correct operation
 */
bool ControlSystem::checkScheduler()
{
    QString msg = "";

    switch(TheScheduler->getStatus())
    {
        case 0: return true;
        case 1: msg = "The scheduler is in a critical state: " \
                      "the timer is not valid!";
                break;
        case 2: msg = "The scheduler is in a critical state: " \
                      "the thread is not valid!";
                break;
        case 3: msg = "The scheduler is in a critical state: " \
                      "the thread is not joinable!";
                break;
        default:msg = "The scheduler is in a critical state: " \
                      "unexpected behavior!";
    }

    TheTracer->writeCriticalLog(msg);
    TheTracer->playAcousticWarning();
    TheTracer->vibrationWarning();

    return false;
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
    switch(TheTracer->getStatus())
    {
        case 0: return true;
        case 1: cout << "The Tracer is in a critical state: "
                     << "the logfile is not opened!" << endl;
                break;
        case 2: cout << "The Tracer is in a critical state: "
                     << "the logfile is not writeable!" << endl;
                break;
        default:cout << "The Tracer is in a critical state: "
                     << "unexpected behavior!" << endl;
    }

    return false;
}

/* Checks the batteries charging state
 */
int ControlSystem::checkBatteryStatus()
{
    int BatteryStatus = ThePump->checkPumpBatteryStatus();

    if(BatteryStatus < BatteryMinLoad)
    {
        QString msg = "The batteries charging state (" + QString::number(BatteryStatus)
                    + "%) is beyond minimum (" + QString::number(BatteryMinLoad) + "%).";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
    }
    else if(BatteryStatus < (BatteryMinLoad*1.1))
    {
        QString msg = "The batteries charging state (" + QString::number(BatteryStatus)
                    + "%) is getting low (min:" + QString::number(BatteryMinLoad)+ "%).";
        TheTracer->writeWarningLog(msg);
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

/* (SLOT) */
void ControlSystem::setBatteryMinLoad(int load)
{
    BatteryMinLoad = load;

    emit updateMinBatteryLevel(load);
}

/* Getter & Setter for maximum operation time in hours
 */
int ControlSystem::getMaxOperationHours() const
{
    return MaxOperationHours;
}

/* (SLOT) */
void ControlSystem::setMaxOperationHours(int hours)
{
    MaxOperationHours = hours;

    emit updateMaxOperationTime(hours);
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


/* Reads the configuration file
 */
bool ControlSystem::readConfiguration(QString filename)
{
    SaveFile = new QSettings(filename, QSettings::NativeFormat);

    if(!SaveFile->status() == QSettings::NoError)
    {
        return false;
    }

    SaveFile->beginGroup( "InsulinPump-Static" );
    Sensitivity = SaveFile->value("Sensitivity").toInt();
    UpperLevel = SaveFile->value("UpperLevel").toInt();
    LowerLevel = SaveFile->value("LowerLevel").toInt();
    UpperLimit = SaveFile->value("UpperLimit").toInt();
    LowerLimit = SaveFile->value("LowerLimit").toInt();
    SaveFile->endGroup();
    SaveFile->sync();

    delete SaveFile;

    return true;
}




