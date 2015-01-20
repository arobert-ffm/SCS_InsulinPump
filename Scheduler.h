/**
 * @file:   Scheduler.h
 * @class:  Scheduler
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   20.01.2015
 * Created: 24.12.14 17:11 with Idatto, version 1.3
 *
 * @brief:  Triggering the hormone pump
 *          Keep track of operation time
 *
 * Copyright (c) 2015 All Rights Reserved
 */


#ifndef scheduler_
#define scheduler_

#include <QElapsedTimer>
#include <QSettings>
#include <thread>
#include <unistd.h>
#include "Pump.h"


#define CONFIGFILE_NAME "InsulinPump.conf"



class Scheduler : public QObject
{
    Q_OBJECT

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
         * @return: When everything is working fine, 0 is returned
         *          When the timer is not valid, 1 is returned
         *          When the thread is not valid, 2 is returned
         *          When the thread is not joinable, 3 is returned
         */
        virtual int getStatus();

        /**
         * @name:   Get/Set Operation Time
         * @brief:  Get/Set total operation time of the system in ms
         *
         *  Gets the systems total operation time in milliseconds and
         *  answers ControlSystem’s call for checkOperationTime(),
         *
         * @return: The systems total operation time in milliseconds
         */
        virtual quint64 getOperationTime();
        virtual void setOperationTime(quint64 milliseconds);

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
         * @brief:  Save the systems total operation time in milliseconds
         *
         *  The scheduler is keeping track of the total operation time
         *  by adding the actual operation time to the last total
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

        /**
         * @name:   Get/Set Thread
         * @brief:  Get/Set the thread object of the schedulling thread
         *
         * @param:  The thread object reference of the schedulling thread
         * @return: The thread object reference of the schedulling thread
         */
        virtual std::thread* getThread() const;
        virtual void setThread(std::thread* value);

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
        quint64 TotalOperationTime;

        /**
         * @name:   The Pump
         * @brief:  A local representation of the Insulin Pump
         */
        Pump *ThePump;

        /**
         * @name:   Schould Run
         * @brief:  Flag for the thread method
         *
         *  A flag for the thread method to check if it should run
         *  If 'SchoulRun' is true, the scheduler thread loop will run
         *  If 'ShouldRun' is flase, the thread stops the periodic checking
         */
        bool SchouldRun;

        /**
         * @name:   Thread
         * @brief:  The thread object reference of the schedulling thread
         *
         */
        std::thread *Thread;

        /**
         * @name:   Start Operation Time Counter
         * @brief:  Starts the operation time counter
         *
         *  Starts the elapsed timer as counter for measuring the
         *  actual system total operation time in milliseconds
         *
         * @return: 'true' is returned
         */
        virtual bool startOperationTimeCounter();

        /**
         * @name:   Stop Operation Time Counter
         * @brief:  Stops the operation time counter
         *
         * Stops the timer for the operation time measurement,
         * the timer gets invalidated
         *
         * @return: 'true' is returned
         */
        virtual bool stopOperationTimeCounter();

        /**
         * @name:   Start Operation Time Counter
         * @brief:  Starts the operation time counter
         *
         *  Reads the old value from the configuration file from the section
         *  “TotalOperationTime” and sets 'TotalOperationTime' in ms
         */
        virtual void readOperationTime();

        /**
         * @name:   Stop Operation Time Counter
         * @brief:  Stops the operation time counter
         *
         * Writes the actual total operation time to the configuration file
         */
        virtual void writeOperationTime();

public slots:
        /**
         * @name:   Set Operation Time in Hours
         * @brief:  Sets the actual operation time
         *
         *  Public slot to set the actual operation time
         *  and emit a signal to the user interface
         *
         * @param:  The new operation time in hours
         */
        void setOperationTimeInHours(int hours);

    signals:
        /**
         * @name:   Update Operation Time
         * @brief:  Sets the actual operation time in the UI
         *
         *  Signal that gets emitted when the actual operation
         *  time gets changed
         *
         * @param:  The new operation time in hours
         */
        void updateOperationTime(int hours);
};

#endif
