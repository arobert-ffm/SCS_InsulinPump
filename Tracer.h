/**
 * @file:   Tracer.h
 * @class:  Tracer
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   13.01.2015
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
         * The constructor initializes the logfile
         */
        Tracer();

        /**
         * The destructor closes the logfile
         */
        ~Tracer();

        /**
         * Writes the message to the log file.
         * Every Message is signed by date and time.
         * When writing to file has finished, “True” is returned
         */
        virtual bool writeStatusLog(QString message);

        /**
         * Writes the message to the log file.
         * Every Message is signed by date and time.
         * When writing to file has finished, “True” is returned
         */
        virtual bool writeWarningLog(QString message);

        /**
         * Writes the message to the log file.
         * Every Message is signed by date and time.
         * When writing to file has finished, “True” is returned
         */
        virtual bool writeCriticalLog(QString message);

        /**
         * Plays an acoustic sound and returns “True” when done
         */
        virtual bool playAcousticWarning();

        /**
         * Vibrates on a specific event and returns “True” when done
         */
        virtual bool vibrationWarning();

        /**
         * Answers ControlSystem’s call for checkTracer()
         */
        virtual bool getStatus();

        /**
         * Getter & Setter for the file name of the logfile
         */
        virtual QString getLogFileName() const;
        virtual void setLogFileName(QString value);

    private:
        /**
         * The complete filename and path will stored here.
         */
        QString LogFileName;

        /**
         * The representation of the logfile
         */
        QFile *LogFile;

    signals:
        /**
         * Callback for writing status log in the UI.
         *
         * @param: The status log message to insert
         */
        void writeStatusLogInUi(QString message);

        /**
         * Callback for writing warning log in the UI.
         *
         * @param: The warning log message to insert
         */
        void writeWarningLogInUi(QString message);

        /**
         * Callback for writing critical log in the UI.
         *
         * @param: The critical log message to insert
         */
        void writeCriticalLogInUi(QString message);

};

#endif
