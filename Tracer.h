/**
 * @file:   Tracer.h
 * @class:  Tracer
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   25.01.2015
 * Created: 24.12.14 17:11 with Idatto, version 1.3
 *
 * @brief:  Writing to a logfile at different urgency
 *          Also signalling via Beep and/or Vibration
 *
 * Copyright (c) 2015 All Rights Reserved
 */


#ifndef tracer_
#define tracer_

#include <iostream>
#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QString>
#include <QTextStream>


#define LOGFILE_NAME "InsulinPump.log"



class Tracer : public QObject
{
    Q_OBJECT

    public:
        /**
         * @name:   Tracer
         * @brief:  Tracer Constructor
         *
         *  The constructor initializes the logfile
         */
        Tracer();

        /**
         * @name:   ~Tracer
         * @brief:  Tracer Destructor
         *
         *  The destructor flush & closes the logfile
         */
        ~Tracer();

        /**
         * @name:   Write Status Log
         * @brief:  Write a status message to the logfile
         *
         *  Writes a status message with prefixed timestamp an priority
         *  to the log file.
         *
         * @param:  The status message to be written
         * @return: When writing has finished, 'true' is returned
         */
        virtual bool writeStatusLog(QString message);

        /**
         * @name:   Write Warning Log
         * @brief:  Write a warning message to the logfile
         *
         *  Writes a warning message with prefixed timestamp an priority
         *  to the log file.
         *
         * @param:  The warning message to be written
         * @return: When writing has finished, 'true' is returned
         */
        virtual bool writeWarningLog(QString message);

        /**
         * @name:   Write Critical Log
         * @brief:  Write a critical message to the logfile
         *
         *  Writes a critical message with prefixed timestamp an priority
         *  to the log file.
         *
         * @param:  The critical message to be written
         * @return: When writing has finished, 'true' is returned
         */
        virtual bool writeCriticalLog(QString message);

        /**
         * @name:   Play Acoustic Warning
         * @brief:  Plays a beep sound
         *
         *  Plays a simple beep sound
         *
         * @return: 'true' is returned
         */
        virtual bool playAcousticWarning();

        /**
         * @name:   Vibration Warning
         * @brief:  Simulates a vibration warning
         *
         * Simply prints out "vibrating"
         *
         * @return: 'true' is returned
         */
        virtual bool vibrationWarning();

        /**
         * @name:   Check Tracer
         * @brief:  Checks the tracer for availability
         *
         *  Checks the logfile is available for writing and
         *  answers ControlSystem’s call for checkTracer()
         *
         * @return: When the logfile is fully available, 0 is returned
         *          When the logfile is not open, 1 is returned
         *          When the logfile is not writeable, 2 is returned
         */
        virtual int getStatus();

        /**
         * @name:   Get/Set Log File Name
         * @brief:  Get/Set the filename of the logfile
         *
         * @param:  The filename of the logfile
         * @return: The filename of the logfile
         */
        virtual QString getLogFileName() const;
        virtual void setLogFileName(QString value);

    private:
        /**
         * @name:   Log File Name
         * @brief:  File name of the logfile
         *
         *  File name of the logfile
         */
        QString LogFileName;

        /**
         * @name:   Save File
         * @brief:  QFile object for the logfile
         *
         *  File for logging the systems activities
         */
        QFile *LogFile;

    signals:
        /**
         * @name:   Write Status Log To User Interface
         * @brief:  Callback for writing a status message to the UI
         *
         *  From control system connected signal
         *  to slot 'insertStatusLog' in user interface
         *
         * @param: The status log message to insert
         */
        void writeStatusLogInUi(QString message);

        /**
         * @name:   Write Warning Log To User Interface
         * @brief:  Callback for writing a warning message to the UI
         *
         *  From control system connected signal
         *  to slot 'insertWarningLog' in user interface
         *
         * @param: The warning log message to insert
         */
        void writeWarningLogInUi(QString message);

        /**
         * @name:   Write Critical Log To User Interface
         * @brief:  Callback for writing a critcal message to the UI
         *
         *  From control system connected signal
         *  to slot 'insertCriticalLog' in user interface
         *
         * @param: The critical log message to insert
         */
        void writeCriticalLogInUi(QString message);

};

#endif




