/**
 * @file:   ControlSystem.h
 * @class:  ControlSystem
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   13.01.2015
 * Created: 24.12.14 17:11 with Idatto, version 1.3
 *
 * @brief:  Check the systems health status
 *          Write via Tracer to logfile & to UI
 *
 * Copyright (c) 2015 All Rights Reserved
 */


#ifndef controlsystem_
#define controlsystem_

#include "UserInterface.h"
#include "Scheduler.h"
#include "Tracer.h"
#include "Pump.h"


#define BATTERY_MIN_LOAD 10
#define MAX_OPERATION_HOURS 500



class ControlSystem
{
    public:
        /**
         * @brief: The constructor instantiates all necessary objects
         *         and connects some signals to slot for the user interface
         */
        ControlSystem(UserInterface* ui);

        /**
         * Checks the operation hours of the mechanical parts (motor) and
         * returns the value in hours
         */
        virtual int checkOperationHours();

        /**
         * Checks the scheduler for correct operation and returns “True” when
         * everything is working fine
         */
        virtual bool checkScheduler();

        /**
         * Checks the hormone reservoir and returns “True” when everything is fine
         */
        virtual bool checkPump();

        /**
         * Checks the tracer and returns “True” when everything is fine
         */
        virtual bool checkTracer();

        /**
         * Checks the batteries charging state and returns the value in percent
         */
        virtual int checkBatteryStatus();

        /**
         * Returns the used scheduler
         */
        virtual Scheduler *getScheduler();

        /**
         * Getter & Setter for minumum Bettery load level in percent
         */
        virtual int getBatteryMinLoad() const;
        virtual void setBatteryMinLoad(int value);

        /**
         * Getter & Setter for maximum operation time in hours
         */
        virtual int getMaxOperationHours() const;
        virtual void setMaxOperationHours(int value);

        /**
         * Getter & Setter for Flag that thread should run periodically
         */
        virtual bool getSchouldRun() const;
        virtual void setSchouldRun(bool value);

private:
        /**
         * A local representation of the hormone pump
         */
        Pump *ThePump;

        /**
         * A local representation of the scheduler
         */
        Scheduler *TheScheduler;

        /**
         * A local representation of the tracer
         */
        Tracer *TheTracer;

        /**
         * Minumum Bettery load level in percent
         */
        int BatteryMinLoad;

        /**
         * Maximum operation time in hours
         */
        int MaxOperationHours;

        /**
         * Flag for the Thread method to check system health periodically
         */
        bool SchouldRun;

};

#endif
