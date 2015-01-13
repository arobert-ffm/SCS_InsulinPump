/**
 * @file:   Scheduler.h
 * @class:  Scheduler
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   13.01.2015
 * Created: 24.12.14 17:11 with Idatto, version 1.3
 *
 * @brief:  Triggering the hormone pump
 *          Keep track of operation time
 *
 * Copyright (c) 2015 All Rights Reserved
 */


#ifndef scheduler_
#define scheduler_

#include <QSettings>
#include <QElapsedTimer>
#include "Pump.h"


#define CONFIGFILE_NAME "InsulinPump.conf"



class Scheduler
{
    public:
        /**
         * @name:   Scheduler
         * @brief:  Scheduler Constructor
         *
         *  The constructor initializes the time measurement,
         *  sets the configuration file name and
         *  connects to the insulin pump
         *
         * @param:  A pointer to the insulin pump
         */
        Scheduler(Pump *ThePump);

        /**
         * @name:   ~Scheduler
         * @brief:  Scheduler Destructor
         *
         *  The destructor stops the time measurement
         */
        ~Scheduler();

        /**
         * @name:   Get Status
         * @brief:  Get opration status of the scheduler
         *
         *  Checks if the timer is started & working and
         *  answers ControlSystem’s call for checkScheduler(),
         *
         * @return: When everything is working fine, 'true' is returned
         */
        virtual bool getStatus();

        /**
         * @name:   Get Operation Time
         * @brief:  Get total operation time of the system in hours
         *
         *  Gets the systems total operation time in hours and
         *  answers ControlSystem’s call for checkOperationTime(),
         *
         * @return: The systems total operation time in hours
         */
        virtual qint64 getOperationTime();

        /**
         * @name:   Reset Timer
         * @brief:  Reset the timer for measuring the operation time
         *
         *  Sets the new timer start value and restarts the timer
         *
         * @param:  The new timer start value in minutes
         * @return: 'true' is returned
         */
        virtual bool resetTimer(int time_min);

        /**
         * @name:   Trigger Pump
         * @brief:  Lets the insulin pump run a cycle
         *
         *  Triggers the insulin pump to run a cycle of measurement and,
         *  if needed, hormone injection
         *
         * @return: When the cycle finished without errors, 'true' is returned
         */
        virtual bool triggerPump();

        /**
         * @name:   Save Operation Time
         * @brief:  Save the systems total operation time in hours
         *
         *  The scheduler is keeping track of the total operation time
         *  by adding the actual operation time to the last total and
         *  saving the total time in a file
         *
         * @return: When saving the time is finished, 'true' is returned
         */
        virtual bool saveOperationTime();

        /**
         * @name:   Get/Set Configuration File Name
         * @brief:  Get/Set the filename of the configuration file
         *
         * @param:  The filename of the configuration file
         * @return: The filename of the configuration file
         */
        virtual QString getConfigFileName() const;
        virtual void setConfigFileName(QString value);

        /**
         * @name:   Get/Set Should Run
         * @brief:  Get/Set the flag that the thread should run periodically
         *
         * @param:  The flag that the thread should run periodically
         * @return: The flag that the thread should run periodically
         */
        virtual bool getSchouldRun() const;
        virtual void setSchouldRun(bool value);

private:
        /**
         * @name:   Timer
         * @brief:  QElapsedTimer object for measuring the system time
         *
         *  Timer of the Scheduler for measuring operation the systems
         *  total operation time in hours
         */
        QElapsedTimer Timer;

        /**
         * @name:   Configuration File Name
         * @brief:  File name of the configuration file
         *
         *  File name of the configuration file
         */
        QString ConfigFileName;

        /**
         * @name:   Save File
         * @brief:  QSettings object for the configuration file
         *
         *  File for saving the systems total operation time in hours
         */
        QSettings *SaveFile;

        /**
         * @name:   Total Operation Time
         * @brief:  The systems total operation time in milliseconds
         *
         * Counter of operation time for noticing the user when an inspection
         * is needed or when system lifetime (because of mechanical outwear) is
         * reached.
         */
        qint64 TotalOperationTime;

        /**
         * @name:   The Pump
         * @brief:  A local representation of the Insulin Pump
         */
        Pump *ThePump;

        /**
         * @name:   Start Operation Time Counter
         * @brief:  Starts the operation time counter
         *
         *  Reads the old value from the configuration file from the section
         *  “TotalOperationHours” and starts the counter for measuring the
         *  actual system total operation time in milliseconds
         *
         * @return: 'true' is returned
         */
        virtual bool startOperationTimeCounter();

        /**
         * @name:   Stop Operation Time Counter
         * @brief:  Stops the operation time counter
         *
         * Stops the timer for operation time measurement,
         * adds the actual operation time to the total operation time and
         * writes the actual total operation time to the configuration file
         *
         * @return: 'true' is returned
         */
        virtual bool stopOperationTimeCounter();

        /**
         * @name:   Schould Run
         * @brief:  Flag for the thread method
         *
         *  A flag for the thread method to check the bodies health periodically
         *  If 'SchoulRun' is true, the scheduler thread loop will run
         *  If 'ShouldRun' is flase, the thread stops the periodic checking
         */
        bool SchouldRun;
};

#endif
