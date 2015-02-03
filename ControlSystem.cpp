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
    // Initialise variables & objects
    SchouldRun = true;

    TheTracer = new Tracer();

    if(readConfiguration(CONFIGFILE_NAME))
    {
        ui->init(Configuration);
        ThePump = new Pump(TheTracer, Configuration);
        TheScheduler = new Scheduler(ThePump);
    }
    else
    {
        TheTracer->writeCriticalLog("Problem parsing the configuration file! Exiting...");
        QMessageBox msgBox;
        msgBox.setText("Problem parsing the configuration file!\nIs it in path?\nExiting...");
        msgBox.exec();
        exit(EXIT_FAILURE);
    }

    // Initialise callbacks for user interface
    QObject::connect(ThePump, SIGNAL(updateBatteryPowerLevel(int)), ui, SLOT(batteryPowerLevelChanged(int)));
    QObject::connect(ui, SIGNAL(setBatteryPowerLevel(int)), ThePump, SLOT(changeBatteryPowerLevel(int)));
    QObject::connect(ThePump, SIGNAL(updateInsulinReservoir(int)), ui, SLOT(insulinAmountInReservoirChanged(int)));
    QObject::connect(ui, SIGNAL(refillInsulinInPump()), ThePump, SLOT(refillInsulinReservoir()));
    QObject::connect(ui, SIGNAL(setInsulinReservoirLevel(int)), ThePump, SLOT(setInsulinAmount(int)));
    QObject::connect(ThePump, SIGNAL(updateGlucagonReservoir(int)), ui, SLOT(glucagonAmountInReservoirChanged(int)));
    QObject::connect(ui, SIGNAL(refillGlucagonInPump()), ThePump, SLOT(refillGlucagonReservoir()));
    QObject::connect(ui, SIGNAL(setGlucagonReservoirLevel(int)), ThePump, SLOT(setGlucagonAmount(int)));
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

    // Let objects do their initialisation
    ThePump->initPump();
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

    int OperationHours = OperationTime/3600000;
    if(OperationHours > Configuration.maxOpTime)
    {
        QString msg = "The maximum operation time (" + QString::number(Configuration.maxOpTime)
                    + "h) is reached (" + QString::number(OperationHours) + "h).";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
    }
    else if(OperationHours > (Configuration.maxOpTime*0.9))
    {
        QString msg = "The actual operation time (" + QString::number(Configuration.maxOpTime) +
                      "h) is near maximum (" + QString::number(OperationHours) + "h).";
        TheTracer->writeWarningLog(msg);
    }

    return OperationHours;
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
    int Status = ThePump->getPumpStatus();
    QString msg = "";

    if(Status == 0)
    {
        return true;
    }

    if(Status & 1)
    {
        msg = "Pump status: " \
              "the insulin fill level is very low!";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
    }
    if(Status & 2)
    {
        msg = "Pump status: " \
              "the insulin amount is getting low!";
        TheTracer->writeWarningLog(msg);
    }
    if(Status & 4)
    {
        msg = "Pump status: " \
              "the glucagon fill level is very low!";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
    }
    if(Status & 8)
    {
        msg = "Pump status: " \
              "the glucagon amount is getting low!";
        TheTracer->writeWarningLog(msg);
    }

    return false;
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
    int BatteryStatus = ThePump->getBatteryPowerLevel();

    if(BatteryStatus < Configuration.battCrit)
    {
        QString msg = "The batteries charging state (" + QString::number(BatteryStatus)
                    + "%) is beyond minimum (" + QString::number(Configuration.battCrit) + "%).";
        TheTracer->writeCriticalLog(msg);
        TheTracer->playAcousticWarning();
        TheTracer->vibrationWarning();
    }
    else if(BatteryStatus < Configuration.battWarn)
    {
        QString msg = "The batteries charging state (" + QString::number(BatteryStatus)
                    + "%) is getting low (min:" + QString::number(Configuration.battCrit)+ "%).";
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
    return Configuration.battCrit;
}

/* (SLOT) */
void ControlSystem::setBatteryMinLoad(int load)
{
    Configuration.battCrit = load;

    emit updateMinBatteryLevel(load);
}

/* Getter & Setter for maximum operation time in hours
 */
int ControlSystem::getMaxOperationHours() const
{
    return Configuration.maxOpTime;
}

/* (SLOT) */
void ControlSystem::setMaxOperationHours(int hours)
{
    Configuration.maxOpTime = hours;

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
    QFileInfo checkFile(filename);
    if(!checkFile.exists())
    {
        return false;
    }

    SaveFile = new QSettings(filename, QSettings::NativeFormat);

    if(!SaveFile->status() == QSettings::NoError)
    {
        return false;
    }

    SaveFile->beginGroup( "InsulinPump-Static" );
    if((Configuration.hsf = SaveFile->value("Sensitivity").toInt()) == 0)
    {
        return false;
    }
    if((Configuration.upperLevel = SaveFile->value("UpperLevel").toInt()) == 0)
    {
        return false;
    }
    if((Configuration.lowerLevel = SaveFile->value("LowerLevel").toInt()) == 0)
    {
        return false;
    }
    if((Configuration.upperLimit = SaveFile->value("UpperLimit").toInt()) == 0)
    {
        return false;
    }
    if((Configuration.lowerLimit = SaveFile->value("LowerLimit").toInt()) == 0)
    {
        return false;
    }
    if((Configuration.upperAlarm = SaveFile->value("UpperAlarm").toInt()) == 0)
    {
        return false;
    }
    if((Configuration.lowerAlarm = SaveFile->value("LowerAlarm").toInt()) == 0)
    {
        return false;
    }
    if((Configuration.absMaxBSL = SaveFile->value("AbsoluteMax").toInt()) == 0)
    {
        return false;
    }
    if((Configuration.resWarn = SaveFile->value("ReservoirWarn").toInt()) == 0)
    {
        return false;
    }
    if((Configuration.resCrit = SaveFile->value("ReservoirCrit").toInt()) == 0)
    {
        return false;
    }
    if((Configuration.battWarn = SaveFile->value("BatterieWarn").toInt()) == 0)
    {
        return false;
    }
    if((Configuration.battCrit = SaveFile->value("BatterieCrit").toInt()) == 0)
    {
        return false;
    }
    if((Configuration.maxOpTime = SaveFile->value("MaxOpTime").toInt()) == 0)
    {
        return false;
    }
    SaveFile->endGroup();
    SaveFile->sync();

    delete SaveFile;

    return true;
}




