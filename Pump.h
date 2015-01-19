/**
 * @file:   Pump.h
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

#include "Tracer.h"
#include <QObject>

using namespace std;



class Pump : public QObject
{
    Q_OBJECT

// ATTRIBUTES
private:
        //for logging purposes
        Tracer tracer;

        /* When pump is active injecting insulin the value will be 1, when
        *  injecting glucagon the value will be 2 and when inactive the value
        *  will be 0.
        */
        int active;

        // fill level of insulin reservoir
        int insulinReservoirLevel;

        // fill level of glucagon reservoir
        int glucagonReservoirLevel;

        // current blood sugar level
        int currentBloodSugarLevel;

        // the blood sugar level in the latest cycle
        int latestBloodSugarLevel;

        // maximal healthy blood sugar level
        int maxBloodSugarLevel;

        // minimum healthy blood sugar level
        int minBloodSugarLevel;

        // target blood sugar level
        int targetBloodSugarLevel;

        // target to decrease blood sugar level with insulin injection
        int upperTargetBloodSugarLevel;

        // target to increase blood sugar level with glucagon injection
        int lowerTargetBloodSugarLevel;

        // inject insulin or glucagon, true when insulin
        bool insulin;

        // true if there was an injection in the last cycle
        bool delay;

        // current battery power level
        int batteryPowerLevel;

        // hormone sensitivity factor
        int hormoneSensitivityFactor;
// END ATTRBUTES


// FUNCTIONS
public:
        /* CTOR
         *
         */
        /**
         * @brief Pump
         *        init object with default values.
         */
        Pump();

        /* DTOR
         *
         */
        ~Pump();

        /**
         * @brief runPump
         *        "main"-function for pump triggered by Scheduler.
         * @return true if pump runs.
         */
        bool runPump();

        /**
         * @brief checkPumpBatteryStatus
         *        Checks the battery status and returns the value in percent.
         *        Throws ok, warning and error, according to power level.
         *
         *        ok:       level between 15 and 100 percent.
         *        warning:  level below 15 percent.
         *        critical: level at zero percent.
         *
         * @return 0 on success.
         *
         */
        int checkPumpBatteryStatus(void);

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
        bool injectHormone(int targetBloodSugarLevel, bool insulin, int amount);

        /**
         * @brief decreaseHormoneLevel
         *        Decreases either the insulin or the glucagon level in the reservoir when a hormone is
         *        injected to the body.
         *
         * @param amount
         *        the amount of FU by that the reservoir should be decreased.
         *
         * @param insulin
         *        true if the hormone to inject is insulin, false if it is glucagon.
         *
         * @return true on successful decrease.
         */
        bool decreaseHormoneReservoir(int amount, bool insulin);

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
        bool injectHormoneToBody(int amount, bool insulin);

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

        /**
          * @brief rechargeBatteryPower
          *        recharges battery up to 100% of charge.
          *
          * @param charge
          *        level of recharge energy.
          */
         void rechargeBatteryPower(int charge);
// END FUNCTIONS


// GETTER
public:
        /** @return 'true' when everything is working fine.*/
        bool getPumpStatus() const;

        /** @return target blood sugar value.*/
        int getTargetBloodSugarLevel(void) const;

        /** @return the insulin level in the reservoir.*/
        int getInsulinReservoirLevel() const;

        /** @return the glucagon level in the reservoir.*/
        int getGlucagonReservoirLevel() const;

        /** @return the current blood sugar level.*/
        int getCurrentBloodSugarLevel() const;

        /** @return battery power level.*/
        int getBatteryPowerLevel() const;

        /** @return pump status active. See also attribute comment. */
        int getActive() const;

        /** @return latest blood sugar level.*/
        int getLatestBloodSugarLevel() const;

        /** @return HSF.*/
        int getHormoneSensitivityFactor() const;

        /** @return true if delay flag is set.*/
        bool getDelay() const;

        /** @return true if hormone is insulin, false if glucagon.*/
        bool getInsulin() const;

        /** @return lower threshold of blood sugar level.*/
        int getLowerTargetBloodSugarLevel() const;

        /** @return upper threshold of BSL.*/
        int getUpperTargetBloodSugarLevel() const;

        /** @return minimum blood sugar level.*/
        int getMinBloodSugarLevel() const;

        /** @return maximum blood sugar level.*/
        int getMaxBloodSugarLevel() const;

// END GETTER


// SETTER

     /**
      * @brief setBatteryPowerLevel
      *        sets power level of battery.
      * @param powerlevel
      *        value of charge.
      */
     void setBatteryPowerLevel(int powerlevel);

     /**
      * @brief setTargetBloodSugarLevel
      *        set target blood sugar level.
      *
      * @param tbsl
      *        predefined value for target BSL.
      */
     void setTargetBloodSugarLevel(int tbsl);

     /**
      * @brief setActive
      *        set pump on active.
      *
      * @param value
      *        1: active, injecting insulin.
      *        2: active, injecting glucagon.
      *        3: inactive.
      */
     void setActive(int value);

     /**
      * @brief setLatestBloodSugarLevel
      *        sets latest blood sugar level.
      * @param value
      *        last blood sugar level measured by sensor.
      */
     void setLatestBloodSugarLevel(int value);

     /**
      * @brief setHormoneSensitivityFactor
      *        sets HSF.
      *
      * @param value
      *        e.g. HSF = 5
      */
     void setHormoneSensitivityFactor(int value);

     /**
      * @brief setDelay
      *        sets delay flag.
      *
      * @param value
      *        true if flag set.
      */
     void setDelay(bool value);

     /**
      * @brief setInsulin
      *        set insulin value.
      * @param value
      *        true if insulin.
      */
     void setInsulin(bool value);

     /**
      * @brief setLowerTargetBloodSugarLevel
      *        set lower BSL.
      * @param value
      *        value of lower BSL.
      */
     void setLowerTargetBloodSugarLevel(int value);

     /**
      * @brief setUpperTargetBloodSugarLevel
      *        set upper BSL.
      *
      * @param value
      *        value of upper BSL.
      */
     void setUpperTargetBloodSugarLevel(int value);

     /**
      * @brief setMinBloodSugarLevel
      *        set minimum BSL.
      * @param value
      *        value of minimum BSL.
      */
     void setMinBloodSugarLevel(int value);

     /**
      * @brief setMaxBloodSugarLevel
      *        set minimum BSL.
      *
      * @param value
      *        value of maximum BSL.
      */
     void setMaxBloodSugarLevel(int value);

// END SETTER

// SLOTS
public slots:
    /** Changes the Power Level of the Battery.*/
    void changeBatteryPowerLevel(int level);

    /** Refills the Insulin in the Reservoir of the Pump.*/
    void refillInsulinReservoir();

    /** Refills the Glucagon in the Reservoir of the Pump.*/
    void refillGlucagonReservoir();

// END SLOTS


// SIGNALS
signals:

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
    void updateInsulinReservoir(float amount);

    /**
     * @brief updateGlucagonReservoir
     *        Callback for updating Glucagon Reservoir in the UI.
     *
     * @param amount
     *        The current amount of glucagon in the reservoir.
     */
    void updateGlucagonReservoir(float amount);

    /**
     * @brief updateBloodSugarLevel
     *        The current amount of insulin in the reservoir.
     *
     * @param tbsl
     *        target blood sugar level.
     *
     * @param amountHormone
     *        amount of hormone used.
     *
     * @param injectHormUnits
     *        units of injected hormone.
     */
    void updateBloodSugarLevel(int tbsl, int amountHormone, int injectHormUnits);

// END SIGNALS
}; //END HEADER

#endif
