/**
 * @file:   Pump.cpp
 * @class:  Pump
 *
 * @author: Markus Ernst, markuser@stud.fra-uas.de
 *          Jenny Kreger, je.kreger@gmail.com
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


Pump::Pump(Tracer *trcr, config cfg)
{

    /**
    absMaxBSL = cfg.absMaxBSL;
    battCrit = cfg.battCrit;
    battWarn = cfg.battWarn;
    resCrit = cfg.resCrit;
    resWarn = cfg.resWarn;
    */

    batteryPowerLevel=100;
    hormoneSensitivityFactor = cfg.hsf;
    active = true;
    delay = false;
    targetBloodSugarLevel=110;
    insulin = false;
    currentBSLevel = 0;
    latestBSLevel = 0;
    upperTargetBSL = cfg.upperLevel;
    lowerTargetBSL = cfg.lowerLevel;
    upperAlarmLimit = cfg.upperLimit;
    lowerAlarmLimit = cfg.lowerLimit;

    refillInsulinReservoir();
    refillGlucagonReservoir();
    this->tracer = trcr;
}

//DTOR
Pump::~Pump()
{
}

//
bool Pump::initPump()
{
    emit updateInsulinReservoir(insulinReservoirLevel);
    emit updateGlucagonReservoir(glucagonReservoirLevel);

    return true;
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
        //cout << "char line: " << line << endl;
        //cout << "int line: " << atoi(line) << endl;

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
void Pump::injectHormone(bool insulin, int amount)
{
    // inject to body
    injectHormoneToBody(amount, insulin);

    if (amount > 0)
    {
        delay = true;
        // call gui
        if (insulin)
        {
            insulinReservoirLevel -= amount;
            emit updateInsulinReservoir(insulinReservoirLevel);
            emit updateHormoneInjectionLog(UserInterface::INSULIN, amount);
        }
        else
        {
            glucagonReservoirLevel -= amount;
            emit updateGlucagonReservoir(glucagonReservoirLevel);
            emit updateHormoneInjectionLog(UserInterface::GLUCAGON, amount);
        }
        //drain power of battery
        drainBatteryPower(1);
    }
}

// substrace value from reservoirs
bool Pump::decreaseHormoneReservoir(int amount, bool insulin)
{
    QString str_hormone, str_insulin="Insulin", str_glucagon="Glucagon";
    QString err = "Reservoir "+ str_hormone +" too low!";

    //set str_hormone to "Insulin" or "Glucagon". ternary operator!
    str_hormone = insulin? str_insulin : str_glucagon;

    if (insulin)
    {
        if (amount <= this->getInsulinReservoirLevel() && amount!=0)
        {
            insulinReservoirLevel -= amount;
            emit updateInsulinReservoir(insulinReservoirLevel);
            return true;
        }
    }
    else
    {
        if (amount <= this->getGlucagonReservoirLevel() && amount !=0)
        {
            glucagonReservoirLevel -= amount;
            emit updateGlucagonReservoir(glucagonReservoirLevel);
            return true;
        }
    }
    tracer->writeCriticalLog(err);
    return false;
}

// checks battery status of pump
int Pump::checkPumpBatteryStatus(void)
{
    int powerlevel = getBatteryPowerLevel();

    emit updateBatteryPowerLevel(powerlevel);

    if(powerlevel<=15 && powerlevel >0)
    {
        return powerlevel;
    }
    else if (powerlevel>15 && powerlevel <= 100)
    {
        return powerlevel;
    }
    else if (powerlevel==0)
    {
        return powerlevel;
    }

    return EXIT_SUCCESS;
}

// calculates units for needed hormone
int Pump::calculateNeededHormone(int targetBloodSugarLevel)
{
    int difference; int fictHormUnit;
    difference = abs(currentBSLevel - targetBloodSugarLevel);
    fictHormUnit = ceil(difference / hormoneSensitivityFactor);

    cout << "int targetBloodSugarLevel: " << targetBloodSugarLevel << endl;
    cout << "difference: " << difference << endl;
    cout << "fictHormUnit: " << fictHormUnit << endl;

    return fictHormUnit;
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
            QString err = "No body found!";
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

    int hormonesToInject = 0; //<<---init with bogus value.
    emit updateBloodSugarLevel(currentBSLevel);

    // inject insulin
    if (currentBSLevel > upperAlarmLimit)
    {
        if (currentBSLevel > latestBSLevel)
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
    else if (currentBSLevel < lowerAlarmLimit)
    {
        if (currentBSLevel < latestBSLevel)
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
    injectHormone(insulin, hormonesToInject);
    return true;
}

// battery recharge
void Pump::rechargeBatteryPower(int charge)
{
    QString err = "Insufficient Power! Battery not charged!";
    if(charge >=batteryPowerLevel && charge <= MAX_BATTERY_CHARGE)
    {
        //TODO! <- check for correctness.
        batteryPowerLevel = charge;
    }
    tracer->writeCriticalLog(err);
}

// power drain
void Pump::drainBatteryPower(int powerdrain)
{
    QString err = "Power drainage too high!";

    if(powerdrain>0 && powerdrain<=batteryPowerLevel)
    {
        //TODO! <- check for correctness.
        batteryPowerLevel-=powerdrain;
    }
    tracer->writeCriticalLog(err);
}

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
//END FUNCTIONS


// GETTER
int Pump::getBatteryPowerLevel() const
{
    return this->batteryPowerLevel;
}

bool Pump::getPumpStatus() const
{
    return true;
}

int Pump::getInsulinReservoirLevel() const
{
    return this->insulinReservoirLevel;
}

int Pump::getGlucagonReservoirLevel() const
{
    return this->glucagonReservoirLevel;
}
//>>>> auto generated getter
// END GETTER


// SETTER
void Pump::setBatteryPowerLevel(int powerlevel=100)
{
    batteryPowerLevel=powerlevel;
}

void Pump::setTargetBloodSugarLevel(int tbsl)
{
    QString err = "Target Blood Sugar Level not within limits!";

    if(tbsl>lowerTargetBSL && tbsl<upperTargetBSL)
    {
        targetBloodSugarLevel=tbsl;
    }
    tracer->writeCriticalLog(err);
}

//>>>> auto generated setter
void Pump::setHormoneSensitivityFactor(int value)
{
    QString err = "HSF invalid! Please enter valid HSF!";
    if(value!=0)
    {
        hormoneSensitivityFactor = value;
    }
    tracer->writeCriticalLog(err);
}

void Pump::setDelay(bool value)
{
    delay = value;
}

void Pump::setInsulin(bool value)
{
    insulin = value;
}

void Pump::setActive(int value)
{
    active = value;
}
// END SETTER

