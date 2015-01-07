// 
// File: ControlSystem.h
// 
// Date: 24.12.14 17:11
// 
// Generated by: Idatto, version 1.3
// 
// Description:  Check the status of the other classes
//
// Author:       Sven Sperner, sillyconn@gmail.com


#ifndef controlsystem_
#define controlsystem_

#include "UserInterface.h"
#include "Scheduler.h"
#include "Tracer.h"
#include "Pump.h"



class ControlSystem
{
    public:
        ControlSystem(UserInterface ui);

        // Checks the operation hours of the mechanical parts (motor) and 
        // returns the value in hours. 
        virtual int checkOperationHours();

        // Checks the scheduler for correct operation and returns “True” when 
        // everything is working fine! 
        virtual bool checkScheduler();

        // Checks the hormone reservoir and returns “True” when everything is fine
        virtual bool checkPump();

        // Checks the tracer and returns “True” when everything is fine!
        virtual bool checkTracer();

        // checks the batteries charging state and returns the value in percent
        virtual int checkBatteryStatus();

    private:
        Pump pump;
        Scheduler scheduler;
        Tracer tracer;

};

#endif
