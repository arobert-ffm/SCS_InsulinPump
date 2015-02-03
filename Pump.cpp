/**
 * @file:   Pump.cpp
 * @class:  Pump
 *
 * @author: Markus Ernst, markuser@stud.fra-uas.de
 *          Jenny Kreger, je.kreger@gmail.com
 *          Sven Sperner, sillyconn@gmail.com
 *          - fixed twistd bsl error messages
 *          - fixed injection conditions
 *          - fixed rechargeBatteryPower() missing else
 *          - fixed drainBatteryPower() missing else
 *          - added hormone test slider functionality
 *          - small modifications of tracer messages
 *          - rewrite of getPumpStatus()
 *
 * @date:   14.01.2015
 * Created: 24.12.14 17:11 with Idatto, version 1.3
 *
 * @brief:  Measure Blood Sugar Level, Calculate Units of Hormone (Insulin or Glucagon)
 *          and Inject Calculated Units.
 *
 * Copyright (c) 2015 All Rights Reserved
 */

#include "Pump.h"
#include "UserInterface.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <QObject>
#include <fstream>

using namespace std;


/****************************************************************************************************
 *                                                                                                  *
 *                              EXTERNAL CALLABLE FUNCTIONS                                         *
 *                                                                                                  *
 ***************************************************************************************************/
Pump::Pump(Tracer *trcr, config cfg)
{   
    this->tracer = trcr;

    hormoneSensitivityFactor = cfg.hsf;
    upperTargetBSL = cfg.upperLevel;
    lowerTargetBSL = cfg.lowerLevel;
    upperLimit = cfg.upperLimit;
    lowerLimit = cfg.lowerLimit;
    upperAlarm = cfg.upperAlarm;
    lowerAlarm = cfg.lowerAlarm;
    reservoirWarning = cfg.resWarn;
    reservoirCritical = cfg.resCrit;
}


//DTOR
Pump::~Pump()
{
}


// initializes working attributes
void Pump::initPump()
{
    batteryPowerLevel = 100;
    active = true;
    delay = false;
    refillInsulinReservoir();
    refillGlucagonReservoir();
    insulin = false;
    currentBSLevel = 0;
    latestBSLevel = 0;
}


// main for pump
bool Pump::runPump()
{
    //drain power of battery
    drainBatteryPower(1);

    // First iteration: no latest blood sugar value, set latest to current
    if (currentBSLevel <= 0)
    {
        currentBSLevel = readBloodSugarSensor();
        if (currentBSLevel == -1)
        {
            QString err = "Pump: No body found!";
            tracer->writeCriticalLog(err);
            return false;
        }
        else
        {
            latestBSLevel = currentBSLevel;
        }
    }
    // Following iterations: former blood sugar value to latest, read new one from sensor
    else
    {
        latestBSLevel = currentBSLevel;
        currentBSLevel = readBloodSugarSensor();
    }

    // low/high blood sugar level checks
    if (currentBSLevel <= lowerAlarm)
    {
        QString err = "Pump: Low blood sugar level!";
        tracer->writeCriticalLog(err);
    }
    if (currentBSLevel >= upperAlarm)
    {
        QString err = "Pump: High blood sugar level!";
        tracer->writeCriticalLog(err);
    }

    int hormonesToInject = 0; //<<---init with bogus value.
    emit updateBloodSugarLevel(currentBSLevel);

    // inject insulin
    if (currentBSLevel > upperLimit)
    {
        if (currentBSLevel >= latestBSLevel)
        {
            insulin = true;
            if (delay)
            {
                hormonesToInject = calculateNeededHormone(latestBSLevel);
            }
            else
            {
                hormonesToInject = calculateNeededHormone(upperTargetBSL);
            }
        }
    }
    // inject glucagon
    else if (currentBSLevel < lowerLimit)
    {
        if (currentBSLevel <= latestBSLevel)
        {
            insulin = false;
            if (delay)
            {
                hormonesToInject = calculateNeededHormone(latestBSLevel);
            }
            else
            {
                hormonesToInject = calculateNeededHormone(lowerTargetBSL);
            }
        }
    }
    // inject nothing
    else
    {
        hormonesToInject = 0;
    }

    delay = false;
    prepareInjection(insulin, hormonesToInject);
    return true;
}


// battery recharge
void Pump::rechargeBatteryPower(int charge)
{
    QString err = "Pump: Insufficient Power! Battery not charged!";
    if(charge >=batteryPowerLevel && charge <= MAX_BATTERY_CHARGE)
    {
        //TODO! <- check for correctness.
        batteryPowerLevel = charge;
    }
    else
    {
        tracer->writeCriticalLog(err);
    }
}


int Pump::getBatteryPowerLevel()
{
    int powerlevel = this->batteryPowerLevel;
    emit updateBatteryPowerLevel(powerlevel);
    return this->batteryPowerLevel;
}


int Pump::getPumpStatus() const
{
    int RetVal = 0;

    if(insulinReservoirLevel < reservoirCritical)
    {
        RetVal += 1;
    }
    else if(insulinReservoirLevel < reservoirWarning)
    {
        RetVal += 2;
    }
    if(glucagonReservoirLevel < reservoirCritical)
    {
        RetVal += 4;
    }
    else if(glucagonReservoirLevel < reservoirWarning)
    {
        RetVal += 8;
    }

    return RetVal;
}


/****************************************************************************************************
 *                                                                                                  *
 *                                  INTERNAL WORKING FUNCTIONS                                      *
 *                                                                                                  *
 ***************************************************************************************************/
// power drain
void Pump::drainBatteryPower(int powerdrain)
{
    QString err = "Pump: Power drainage too high!";

    if(powerdrain>0 && powerdrain<=batteryPowerLevel)
    {
        //TODO! <- check for correctness.
        batteryPowerLevel-=powerdrain;
    }
    else
    {
        tracer->writeCriticalLog(err);
    }
}


// read BSL value from sensor
int Pump::readBloodSugarSensor()
{
    ifstream file;
    char line[4];

    file.open("pipe_to_pump", ios_base::in);

    if (file.good())
    {
        file.seekg(0L, ios::beg);
        file.getline(line, 4);
        file.close();

        remove("pipe_to_pump");

        return atoi(line);
    }
    else
    {
        return -1;
    }
}


// inject hormone to body
void Pump::injectHormoneToBody(int amount, bool insulin)
{
    ofstream file("pipe_to_body", ios_base::out);

    if (insulin)
    {
        if (amount < 10)
        {
            file << "0";
        }
        file << amount;
        file << "00";
    }
    else
    {
        file << "00";
        if (amount < 10)
        {
            file << "0";
        }
        file << amount;
    }
    file.close();
}


// inject hormone
void Pump::prepareInjection(bool insulin, int amount)
{
    QString msg = "";
    if (amount > 0)
    {
        //drain power of battery
        drainBatteryPower(1);
        delay = true;

        if (insulin)
        {
            if (insulinReservoirLevel >= amount)
            {
                insulinReservoirLevel -= amount;
            }
            else
            {
                amount = insulinReservoirLevel;
                insulinReservoirLevel = 0;
                msg = "Pump: Insulin reservoir too low to inject proper amount!";
                tracer->writeCriticalLog(msg);
            }
            emit updateInsulinReservoir(insulinReservoirLevel);
            emit updateHormoneInjectionLog(UserInterface::INSULIN, amount);
/*
            if (insulinReservoirLevel <= reservoirCritical)
            {
                msg = "Pump: Insulin reservoir empty! Please refill!";
                tracer->writeCriticalLog(msg);
            }
            else if (insulinReservoirLevel <= reservoirWarning)
            {
                msg = "Pump: Insulin reservoir nearly empty! Please refill!";
                tracer->writeWarningLog(msg);
            }
*/
        }
        else
        {
            if (glucagonReservoirLevel >= amount)
            {
                glucagonReservoirLevel -= amount;
            }
            else
            {
                amount = glucagonReservoirLevel;
                glucagonReservoirLevel = 0;
                QString err = "Pump: Glucagon reservoir too low to inject proper amount!";
                tracer->writeCriticalLog(err);
            }
            emit updateGlucagonReservoir(glucagonReservoirLevel);
            emit updateHormoneInjectionLog(UserInterface::GLUCAGON, amount);

            if (glucagonReservoirLevel <= reservoirCritical)
            {
                msg = "Pump: Glucagon reservoir empty! Please refill!";
                tracer->writeCriticalLog(msg);
            }
            else if (glucagonReservoirLevel <= reservoirWarning)
            {
                msg = "Pump: Glucagon reservoir nearly empty! Please refill!";
                tracer->writeWarningLog(msg);
            }
        }
    }

    // inject to body
    injectHormoneToBody(amount, insulin);
}


// calculates amount of needed hormone
int Pump::calculateNeededHormone(int targetBloodSugarLevel)
{
    int difference; int fictHormUnit;
    difference = abs(currentBSLevel - targetBloodSugarLevel);
    fictHormUnit = ceil(difference / hormoneSensitivityFactor);

    return fictHormUnit;
}


/****************************************************************************************************
 *                                                                                                  *
 *                                             SLOTS                                                *
 *                                                                                                  *
 ***************************************************************************************************/
// changes the batteries power level
void Pump::changeBatteryPowerLevel(int level)
{
    batteryPowerLevel = level;
    // Update UI
    emit updateBatteryPowerLevel(level);
}

// refills insulin reservoir
void Pump::refillInsulinReservoir()
{
    insulinReservoirLevel = 100;
    // Update UI
    emit updateInsulinReservoir(100);
}

// refills glucagon reservoir
void Pump::refillGlucagonReservoir()
{
    glucagonReservoirLevel = 100;
    // Update UI
    emit updateGlucagonReservoir(100);
}

// changes insulin amount
void Pump::setInsulinAmount(int level)
{
    insulinReservoirLevel = level;
    // Update UI
    emit updateInsulinReservoir(level);
}

// changes glucagon amount
void Pump::setGlucagonAmount(int level)
{
    glucagonReservoirLevel = level;
    // Update UI
    emit updateGlucagonReservoir(level);
}
