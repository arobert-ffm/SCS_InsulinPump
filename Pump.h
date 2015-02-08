/**
 * @file:   Pump.h
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

/* abbreviations:
 *
 * HSF/hsf  =   Hormone Sensitivity Factor;
 * BSL      =   Blood Sugar Level;
 * cBSL     =   current Blood Sugar Level;
 * tBSL     =   target Blood Sugar Level;
 *
 *
 */

#ifndef pump_
#define pump_

#define MAX_BATTERY_CHARGE  100

#include "Config.h"
#include "Tracer.h"
#include <QObject>

using namespace std;


class Pump : public QObject
{
    Q_OBJECT

private:
    /****************************************************************************************************
     *                                                                                                  *
     *                                  CONFIGURATION ATTRIBUTES                                        *
     *                                                                                                  *
     ***************************************************************************************************/
    //for logging purposes
    Tracer *tracer;

    // hormone sensitivity factor
    int hormoneSensitivityFactor;

    // target to decrease blood sugar level with insulin injection
    int upperTargetBSL;

    // target to increase blood sugar level with glucagon injection
    int lowerTargetBSL;

    // maximal healthy blood sugar level
    int upperLimit;

    // minimum healthy blood sugar level
    int lowerLimit;

    // blood sugar level when user is alarmed
    int upperAlarm;

    // blood sugar level when user is alarmed
    int lowerAlarm;

    // reservoir level warning
    int reservoirWarning;

    // reservoir level critical
    int reservoirCritical;



    /****************************************************************************************************
     *                                                                                                  *
     *                                  INTERNAL WORKING ATTRIBUTES                                     *
     *                                                                                                  *
     ***************************************************************************************************/
    /* When pump is active injecting insulin the value will be 1, when
     * injecting glucagon the value will be 2 and when inactive the value
     * will be 0.
     */
    int active;

    // fill level of insulin reservoir
    int insulinReservoirLevel;

    // fill level of glucagon reservoir
    int glucagonReservoirLevel;

    // current blood sugar level
    int currentBSLevel;

    // the blood sugar level in the latest cycle
    int latestBSLevel;

    // inject insulin or glucagon, true when insulin
    bool insulin;

    // true if there was an injection in the last cycle
    bool delay;

    // current battery power level
    int batteryPowerLevel;



    /****************************************************************************************************
     *                                                                                                  *
     *                                  INTERNAL WORKING FUNCTIONS                                      *
     *                                                                                                  *
     ***************************************************************************************************/
    /**
     * @brief drainBatteryPower
     *        Drains power from battery.
     * @param powerdrain
     *        amount of powerdrainage.
     */
    void drainBatteryPower(int powerdrain);

    /**
     * @brief injectHormone
     *        Injects either insulin or glucagon into the body.
     *
     * @param targetBloodSugarLevel
     *        predefined target blood sugar level.
     *
     * @param insulin
     *        true if the hormone to inject is insulin, false if it is glucagon.
     *
     * @param amount
     *        the amount of FU that should be injected.
     *
     * @return false on failed injection.
     */
    void prepareInjection(bool insulin, int amount);

    /**
     * @brief   Pump::readBloodSugarSensor
     *          Reads the current blood sugar level via pipe from body.
     *
     * @return  the current blood sugar level.
     */
    int readBloodSugarSensor();

    /**
     * @brief   Pump::injectHormoneToBody
     *          Injects the calculated amount of hormones to the body via pipe.
     *
     * @param   amount
     *          the amount of hormones to inject.
     *
     * @param   insulin
     *          true if insulin, false if glucagon.
     *
     * @return  true if anything is ok.
     */
    void injectHormoneToBody(int amount, bool insulin);

    /**
     * @brief calculateNeededHormone
     *        Calculates the amount of hormone needed based on the blood sugar levels.
     *
     * @param targetBloodSugarLevel
     *        predefined value to raise or reduce blood sugar level to,
     *        e.g. 90mg/dl -> targetBloodSugarLevel = 90;
     *
     * @return Returns fictional Units of specified hormone.
     */
    int calculateNeededHormone(int targetBloodSugarLevel);




public:
    /****************************************************************************************************
     *                                                                                                  *
     *                              EXTERNAL CALLABLE FUNCTIONS                                         *
     *                                                                                                  *
     ***************************************************************************************************/
    /* CTOR
     *
     */
    /**
      * @brief Pump
      *        init pump object with values from .conf
      * @param tracer
      *        error handling obj
      * @param cfg
      *        struct for .conf
      */
     Pump(Tracer *tracer, config cfg);

    /* DTOR
     *
     */
    ~Pump();

     /**
     * @brief   initPump
     *          initializes working attributes
     */
    void initPump();

    /**
     * @brief runPump
     *        "main"-function for pump triggered by Scheduler.
     * @return true if pump runs.
     */
    bool runPump();

    /**
     * @brief rechargeBatteryPower
     *        recharges battery up to 100% of charge.
     *
     * @param charge
     *        level of recharge energy.
     */
     void rechargeBatteryPower(int charge);

     /** @return pump status.
       *         0 - everything is fine
       *         1 - insulin level critical
       *         2 - insulin level warning
       *         4 - glucagon level critical
       *         8 - glucagon level warning
       */
     int getPumpStatus() const;

     /** @return battery power level.*/
     int getBatteryPowerLevel();


public slots:
     /****************************************************************************************************
      *                                                                                                  *
      *                                             SLOTS                                                *
      *                                                                                                  *
      ***************************************************************************************************/
    /** Changes the Power Level of the Battery.*/
    void changeBatteryPowerLevel(int level);

    /** Refills the Insulin in the Reservoir of the Pump.*/
    void refillInsulinReservoir();

    /** Refills the Glucagon in the Reservoir of the Pump.*/
    void refillGlucagonReservoir();

    /** Changes the Amount of Insulin in the Reservoir.*/
    void setInsulinAmount(int level);

    /** Changes the Amount of Glucagon in the Reservoir.*/
    void setGlucagonAmount(int level);


signals:
    /****************************************************************************************************
     *                                                                                                  *
     *                                             SIGNALS                                              *
     *                                                                                                  *
     ***************************************************************************************************/
     /**
      * @brief updateBatteryPowerLevel
      *        Callback for updating Battery Power Level in the UI.
      *
      * @param level
      *        The current level of bettery power.
      */
     void updateBatteryPowerLevel(int level);

     /**
     * @brief updateInsulinReservoir
     *        Callback for updating Insulin Reservoir in the UI.
     *
     * @param amount
     *        The current amount of insulin in the reservoir.
     */
    void updateInsulinReservoir(int amount);

    /**
     * @brief updateGlucagonReservoir
     *        Callback for updating Glucagon Reservoir in the UI.
     *
     * @param amount
     *        The current amount of glucagon in the reservoir.
     */
    void updateGlucagonReservoir(int amount);

    /**
     * @brief updateBloodSugarLevel
     *        The current amount of insulin in the reservoir.
     *
     * @param tbsl
     *        target blood sugar level.
     *
     */
    void updateBloodSugarLevel(int tbsl);

    /**
     * @brief Inserts the new injection in to the QList in the UI
     *
     * @param hormone
     *        Hormone used.
     *
     * @param injectHormUnits
     *        units of injected hormone.
     */
    void updateHormoneInjectionLog(int hormone, int injectHormUnits);
}; //END HEADER

#endif
