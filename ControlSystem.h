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
         * @name:   Control System
         * @brief:  Control Systems Constructor
         *
         *  The constructor instantiates all necessary objects
         *  and connects some signals to slots for the user interface
         *
         * @param:  A pointer to the user interface for callbacks
         */
        ControlSystem(UserInterface* ui);

        /**
         * @name:   Check Operation Hours
         * @brief:  Check systems total operation time in hours
         *
         *  Gets the systems total operation time in hours from the
         *  scheduler and compares it with the absolute maximum,
         *  which depends on mechanical & electrical parts
         *
         * @return: The systems total operation time in hours
         */
        virtual int checkOperationHours();

        /**
         * @name:   Check Scheduler
         * @brief:  Check scheduler for availability
         *
         *  Checks the scheduler for correct operation, which depends
         *  on the implementation of the scheduler itself
         *
         * @return: When everything is working fine, 'true' is returned
         */
        virtual bool checkScheduler();

        /**
         * @name:   Check Pump
         * @brief:  Checks the insulin pump for availability
         *
         *  Checks the insulin pump for correct operation, which depends
         *  on the implementation of the insulin pump itself
         *
         * @return: When everything is working fine, 'true' is returned
         */
        virtual bool checkPump();

        /**
         * @name:   Check Tracer
         * @brief:  Checks the tracer for availability
         *
         *  Checks the tracer for correct operation, which depends
         *  on the implementation of the tracer itself
         *
         * @return: When everything is working fine, 'true' is returned
         */
        virtual bool checkTracer();

        /**
         * @name:   Check Battery Status
         * @brief:  Checks the batteries charging state in percent
         *
         *  Gets the insulin pumps battery charging state from the
         *  insulin pump and compares it with the absolute maximum,
         *  which depends on the used bettery technology
         *
         * @return: The systems total operation time in hours
         */
        virtual int checkBatteryStatus();

        /**
         * @name:   Get Scheduler
         * @brief:  Get the used scheduler
         *
         * @return: A pointer to the used scheduler
         */
        virtual Scheduler *getScheduler();

        /**
         * @name:   Get/Set Bettery Minimum Load
         * @brief:  Get/Set the minimum battery load level in percent
         *
         * @param:  The batteries minimum load level in percent
         * @return: The batteries minimum load level in percent
         */
        virtual int getBatteryMinLoad() const;
        virtual void setBatteryMinLoad(int value);

        /**
         * @name:   Get/Set Maximum Operation Hours
         * @brief:  Get/Set the maximum operation time in hours
         *
         * @param:  The maximum operation time in hours
         * @return: The maximum operation time in hours
         */
        virtual int getMaxOperationHours() const;
        virtual void setMaxOperationHours(int value);

        /**
         * @name:   Get/Set Should Run
         * @brief:  Get/Set the should run flag for the thread
         *
         * @param:  'true' if the thread should run
         * @return: When the thread should run, 'true' is returned
         */
        virtual bool getSchouldRun() const;
        virtual void setSchouldRun(bool value);

private:
        /**
         * @name:   The Pump
         * @brief:  A local representation of the Insulin Pump
         */
        Pump *ThePump;

        /**
         * @name:   The Scheduler
         * @brief:  A local representation of the Scheduler
         */
        Scheduler *TheScheduler;

        /**
         * @name:   The Tracer
         * @brief:  A local representation of the Tracer
         */
        Tracer *TheTracer;

        /**
         * @name:   Minumum Battery Load
         * @brief:  Minumum battery load level in percent
         */
        int BatteryMinLoad;

        /**
         * @name:   Maximum Operation Hours
         * @brief:  Maximum operation time in hours
         */
        int MaxOperationHours;

        /**
         * @name:   Schould Run
         * @brief:  Flag for the thread method
         *
         *  A flag for the thread method to check system health periodically
         *  If 'SchoulRun' is true, the control system thread loop will run
         *  If 'ShouldRun' is flase, the thread stops the periodic checking
         */
        bool SchouldRun;

};

#endif
