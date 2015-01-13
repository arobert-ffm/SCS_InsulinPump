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
         * The constructor initializes the time measurement
         */
        Scheduler(Pump *ThePump);

        /**
         * The constructor initializes the logfile
         */
        ~Scheduler();

        /**
         * Answers ControlSystem’s call for checkScheduler()
         */
        virtual bool getStatus();

        /**
         * Answers ControlSystem's call for checkOperationTime()
         */
        virtual qint64 getOperationTime();

        /**
         * Resets the timer and sets the countdown time according to parameter
         */
        virtual bool resetTimer(int time_min);

        /**
         * Triggers the pump which then checks the blood sugar level
         */
        virtual bool triggerPump();

        /**
         * Triggers the scheduler to save the systems operation time
         */
        virtual bool saveOperationTime();

        /**
         * Getter & Setter for the file name of the configuration file
         */
        virtual QString getConfigFileName() const;
        virtual void setConfigFileName(QString value);

        /**
         * Getter & Setter for Flag that thread should run periodically
         */
        virtual bool getSchouldRun() const;
        virtual void setSchouldRun(bool value);

private:
        /**
         * Timer of the Scheduler for measuring operation time
         */
        QElapsedTimer Timer;

        /**
         * File name of the configuration file
         */
        QString ConfigFileName;

        /**
         * File for saving total operation time
         */
        QSettings *SaveFile;

        /**
         * Counter of operation time for noticing the user when an inspection
         * is needed or when system lifetime (because of mechanical outwear) is
         * reached.
         */
        qint64 TotalOperationTime;

        /**
         * A local representation of the hormone pump
         */
        Pump *ThePump;

        /**
         * Starts the counter for operation time and returns “True” when success-
         * fully started. The value will be read from “TotalOperationHours”.
         */
        virtual bool startOperationTimeCounter();

        /**
         * Stops the counter for operation time and returns “True” when success-
         * fully stopped. The value will be written to “TotalOperationHours”.
         */
        virtual bool stopOperationTimeCounter();

        /**
         * Flag for the Thread method to trigger the pump periodically
         */
        bool SchouldRun;
};

#endif
