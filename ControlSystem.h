/**
 * @file:   ControlSystem.h
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


#ifndef controlsystem_
#define controlsystem_

#include <QFileInfo>
#include "UserInterface.h"
#include "Scheduler.h"
#include "Tracer.h"
#include "Pump.h"


#define CONFIGFILE_NAME "InsulinPump.conf"



class ControlSystem : public QObject
{
    Q_OBJECT

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
         * @name:   Get Bettery Minimum Load
         * @brief:  Get the minimum battery load level in percent
         *
         * @param:  The batteries minimum load level in percent
         * @return: The batteries minimum load level in percent
         */
        virtual int getBatteryMinLoad() const;

        /**
         * @name:   Get Maximum Operation Hours
         * @brief:  Get the maximum operation time in hours
         *
         * @param:  The maximum operation time in hours
         * @return: The maximum operation time in hours
         */
        virtual int getMaxOperationHours() const;

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
         * @name:   Save File
         * @brief:  QSettings object for the configuration file
         *
         *  File for loading some specific configuration values
         */
        QSettings *SaveFile;

        /**
         * @name:   Operation Time
         * @brief:  Systems actual opration time in ms
         */
        quint64 OperationTime;

        /**
         * @name:   Minumum Battery Load
         * @brief:  Minumum battery load level in percent
         */
        int BatteryMinLoad;

        /**
         * @name:   Maximum Operation Hours
         * @brief:  Maximum operation time in hours
         */
        unsigned int MaxOperationHours;

        /**
         * @name:   Schould Run
         * @brief:  Flag for the thread method
         *
         *  A flag for the thread method to check system health periodically
         *  If 'SchoulRun' is true, the control system thread loop will run
         *  If 'ShouldRun' is flase, the thread stops the periodic checking
         */
        bool SchouldRun;

        /**
         * @name:   Configuration values
         * @brief:  Some values from config file for pump
         *
         *  Sensitivity
         *  UpperLevel
         *  LowerLevel
         *  UpperLimit
         *  LowerLimit
         *  AbsMax
         *  ResWarn
         *  ResCrit
         *  BattWarn
         *  BattCrit
         */
        config Configuration;

        /**
         * @name:   Read Configuration
         * @brief:  Reads the configuration file
         *
         *  Reads some specific values from the configuration file
         */
        bool readConfiguration(QString filename);

    public slots:
        /**
         * @name:   Set Bettery Minimum Load
         * @brief:  Sets the minimum battery load level in percent
         *
         *  Public slot to set the minimum load level of the battery
         *  and emit a signal to the user interface
         *
         * @param:  The batteries minimum load level in percent
         */
        virtual void setBatteryMinLoad(int load);

        /**
         * @name:   Set Maximum Operation Hours
         * @brief:  Sets the maximum operation time in hours
         *
         *  Public slot to set the maximum operation time in hours
         *  and emit a signal to the user interface
         *
         * @param:  The maximum operation time in hours
         */
        virtual void setMaxOperationHours(int hours);

    signals:
        /**
         * @name:   Update Minimum Battery Level
         * @brief:  Sets the minimum battery level in the UI
         *
         *  Signal that gets emitted when the minimum battery
         *  load level gets changed
         *
         * @param:  The new operation time in hours
         */
        void updateMinBatteryLevel(int level);

        /**
         * @name:   Update Maximum Operation Time
         * @brief:  Sets the maximum operation time in the UI
         *
         *  Signal that gets emitted when the maximum operation
         *  time gets changed
         *
         * @param:  The new maximum operation time in hours
         */
        void updateMaxOperationTime(int hours);
};

#endif




