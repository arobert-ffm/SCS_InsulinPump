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

//TODO
/*
 * 1. clean up code.
 * 2. check return values of functions
 * 3. check initial values!
 * 4.
 * 5.
 * 6. refactor code. especially code that is redundant.
 * 7. test pump.
 * 8. check for TODOs.
 * 9. check methods for correctness.
 * A.
 * B. see TODO tags!
 * C. setter and getter for all attributes! <--- auto generated!
 * D. fill getter and setter with life!
 * E.
 * F.
 * 0.
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

using namespace std;

#define BUFLEN  100 //<---- get buffer length for struct by sizeof(struct)

int     i;
int     main (void);
int     fdes_body_to_pump; // fildescriptor for Body --> Pump
int     fdes_pump_to_body; // fildescriptor for Pump --> Body


/**********************************
 * transmit_hormone_injection     *
 **********************************/
struct transmit_injection_hormones {
    int injected_insulin;
    int injected_glucagon;
} Injecting;

/**********************************
 * transmit_bloodsugar            *
 **********************************/
struct transmit_bloodsugar {
    int bloodSugarLevel;
} BodyStatus;


// read BSL value from sensor
int Pump::readBloodSugarSensor()
{
    int result;

    // open pipe Body --> Pump
    if((fdes_body_to_pump=open("body_to_pump",O_RDONLY))==(-1)) {
        printf("Failure 'open pipe'");
        exit(-1);
    }

    // read Body --> Pump
    read(fdes_body_to_pump, &BodyStatus, BUFLEN);
    result = BodyStatus.bloodSugarLevel;
    close(fdes_body_to_pump);

    return result;
}

// inject hormone to body
bool Pump::injectHormoneToBody(int amount, bool insulin)
{
    // open pipe Pump --> Body
    if((fdes_pump_to_body=open("pump_to_body",O_WRONLY))==(-1)) {
        printf("Failure 'open pipe'");
        return false;
    }

    if (insulin)
    {
        Injecting.injected_insulin = amount;
        Injecting.injected_glucagon = 0;
    }
    else
    {
        Injecting.injected_insulin = 0;
        Injecting.injected_glucagon = amount;
    }

    // write Pump --> Body
    if((i=write(fdes_pump_to_body, &Injecting, BUFLEN)) != BUFLEN) {
        printf("Fehler 'write-call'");
        return false;
    }
    close(fdes_pump_to_body);
    return true;
}


// CTOR
Pump::Pump()
{
    this->setBatteryPowerLevel(100);
    this->setHormoneSensitivityFactor(5);
    this->setActive(true);
    this->setDelay(false);
    this->setTargetBloodSugarLevel(110);
    this->setInsulin(false);
}

//DTOR
Pump::~Pump()
{
}

// inject hormone
bool Pump::injectHormone(int targetBloodSugarLevel, bool insulin, int amount)
{   
    // inject to body
    if (injectHormoneToBody(amount, insulin))
    {
        // call gui
        if (insulin)
        {
            emit updateBloodSugarLevel(targetBloodSugarLevel, UserInterface::INSULIN, amount);
            return true;
        } else
        {
            emit updateBloodSugarLevel(targetBloodSugarLevel, UserInterface::GLUCAGON, amount);
            return true;
        }

        QString err = "Injection aborted! No GUI detected";
        tracer.writeCriticalLog(err);
        return false;
    }
    else
    {
        QString err = "Injection aborted! No body detected!";
        tracer.writeCriticalLog(err);
        return false;
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
    tracer.writeCriticalLog(err);
    return false;
}

// checks battery status of pump
int Pump::checkPumpBatteryStatus(void)
{
    int powerlevel = getBatteryPowerLevel();

    QString warn = "WARNING! Battery low! Charge at: " + powerlevel;
    QString okm = "INFO! Battery ok! Charge at: " + powerlevel;
    QString err = "CRITICAL! Battery critical! Charge at: " + powerlevel;

    if(powerlevel<=15 && powerlevel >0)
    {
        tracer.writeWarningLog(warn);
        return getBatteryPowerLevel();
    }
    else if (powerlevel>15 && powerlevel <= 100)
    {
        tracer.writeStatusLog(okm);
        return getBatteryPowerLevel();
    }
    else if (powerlevel==0)
    {
        tracer.writeCriticalLog(err);
        return getBatteryPowerLevel();
    }

    return EXIT_SUCCESS;
}

// calculates units for needed hormone
int Pump::calculateNeededHormone(int targetBloodSugarLevel)
{
    int difference; int fictHormUnit;
    difference = abs(currentBloodSugarLevel - targetBloodSugarLevel);
    fictHormUnit = ceil(difference / hormoneSensitivityFactor);
    return fictHormUnit;
}

// main for pump
bool Pump::runPump()
{
    // TODO first iteration? predefined value?
    latestBloodSugarLevel = currentBloodSugarLevel;
    currentBloodSugarLevel = readBloodSugarSensor();
    int hormonesToInject=111; //<<---init with bogus value.

    // inject insulin
    if (currentBloodSugarLevel > maxBloodSugarLevel)
    {
        if (currentBloodSugarLevel > latestBloodSugarLevel)
        {
            insulin = true;
            if (delay)
            {
                hormonesToInject = calculateNeededHormone(latestBloodSugarLevel);
            }
            else
            {
                hormonesToInject = calculateNeededHormone(upperTargetBloodSugarLevel);
            }
        }
    }
    // inject glucagon
    else if (currentBloodSugarLevel < minBloodSugarLevel)
    {
        if (currentBloodSugarLevel < latestBloodSugarLevel)
        {
            insulin = false;
            if (delay)
            {
                hormonesToInject = calculateNeededHormone(latestBloodSugarLevel);
            }
            else
            {
                hormonesToInject = calculateNeededHormone(lowerTargetBloodSugarLevel);
            }
        }
    }
    // inject nothing
    else
    {
        hormonesToInject = 0;
    }

    return injectHormone(targetBloodSugarLevel, insulin, hormonesToInject);
}

// battery recharge
void Pump::rechargeBatteryPower(int charge)
{
    QString err = "Insufficient Power! Battery not charged!";
    if(charge >=batteryPowerLevel && charge <= MAX_BATTERY_CHARGE)
    {
        //TODO! <- check for correctness.
        this->batteryPowerLevel = charge;
    }
    tracer.writeCriticalLog(err);
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
    tracer.writeCriticalLog(err);
}

// refills insulin reservoir
void Pump::refillInsulinReservoir()
{
    // Update UI
    emit updateInsulinReservoir(100);
}

// refills glucagon reservoir
void Pump::refillGlucagonReservoir()
{
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

int Pump::getTargetBloodSugarLevel() const
{
    return this->targetBloodSugarLevel;
}

int Pump::getCurrentBloodSugarLevel() const
{
   return this->currentBloodSugarLevel;
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
int Pump::getLowerTargetBloodSugarLevel() const
{
    return lowerTargetBloodSugarLevel;
}

int Pump::getUpperTargetBloodSugarLevel() const
{
    return upperTargetBloodSugarLevel;
}

int Pump::getMinBloodSugarLevel() const
{
    return minBloodSugarLevel;
}

int Pump::getMaxBloodSugarLevel() const
{
    return maxBloodSugarLevel;
}

int Pump::getLatestBloodSugarLevel() const
{
    return latestBloodSugarLevel;
}

int Pump::getActive() const
{
    return active;
}

int Pump::getHormoneSensitivityFactor() const
{
    return hormoneSensitivityFactor;
}

bool Pump::getDelay() const
{
    return delay;
}

bool Pump::getInsulin() const
{
    return insulin;
}
// END GETTER


// SETTER
void Pump::setBatteryPowerLevel(int powerlevel=100)
{
    batteryPowerLevel=powerlevel;
}

void Pump::setTargetBloodSugarLevel(int tbsl)
{
    QString err = "Target Blood Sugar Level not within limits!";

    if(tbsl>70 && tbsl<120)
    {
        this->targetBloodSugarLevel=tbsl;
    }
    tracer.writeCriticalLog(err);
}

//>>>> auto generated setter
void Pump::setHormoneSensitivityFactor(int value)
{
    hormoneSensitivityFactor = value;
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

void Pump::setLowerTargetBloodSugarLevel(int value)
{
    lowerTargetBloodSugarLevel = value;
}

void Pump::setUpperTargetBloodSugarLevel(int value)
{
    upperTargetBloodSugarLevel = value;
}

void Pump::setLatestBloodSugarLevel(int value)
{
    latestBloodSugarLevel = value;
}

void Pump::setMaxBloodSugarLevel(int value)
{
    maxBloodSugarLevel = value;
}

void Pump::setMinBloodSugarLevel(int value)
{
    minBloodSugarLevel = value;
}
// END SETTER
