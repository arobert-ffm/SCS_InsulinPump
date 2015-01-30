/**
 * @file:   Scheduler.h
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   20.01.2015
 * Created: 07.01.15 16:54 with Idatto, version 1.3
 *
 * @brief:  Main() routine for InsulinPump Simulation
 *          Static thread functions for ControlSystem & Scheduler
 *
 * Copyright (c) 2015 All Rights Reserved
 */


#include <QApplication>
#include <thread>
#include <unistd.h>
#include "UserInterface.h"
#include "ControlSystem.h"
#include "Scheduler.h"
#include "Pump.h"

#define CHECK_INTERVAL_SEC 5
#define RUN_INTERVAL_SEC 5

using namespace std;



/**
 * Static function for the Control Systems watcher Thread
 *
 * @brief Checks all System Components
 * @param Pointer to the representation of the ControlSystem object
 * @return EXIT_SUCCESS
 */
int watch(ControlSystem *ControlSystem)
{
    while(ControlSystem->getSchouldRun())
    {
        ControlSystem->checkBatteryStatus();
        ControlSystem->checkOperationHours();
        ControlSystem->checkPump();
        ControlSystem->checkScheduler();
        ControlSystem->checkTracer();

        sleep(CHECK_INTERVAL_SEC);
    }

    return EXIT_SUCCESS;
}

/**
 * Static function for the Scheduler scheduling Thread
 *
 * @brief Triggers the Pump for getting information from the Body
 *        Holds track of the Operation Time
 * @param Pointer to the representation of the Scheduler object
 * @return EXIT_SUCCESS
 */
int schedule(Scheduler *Scheduler)
{
    while(Scheduler->getSchouldRun())
    {
        if(Scheduler->getBatstatus() > 1)
        {
            Scheduler->triggerPump();
        }
        Scheduler->saveOperationTime();

        sleep(RUN_INTERVAL_SEC);
    }

    return EXIT_SUCCESS;
}


/**
 * Initiation of the Userinterface, Humanbody- and Insulinpumpsimulation.
 *
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    //Init battery

    // Create User Interface
    QApplication application(argc, argv);
    UserInterface window;

    // Create System Objects
    ControlSystem* TheControlSystem = new ControlSystem(&window);
    Scheduler* TheScheduler = TheControlSystem->getScheduler();

    // Start Scheduler Thread
    thread* Scheduler = new thread(schedule,TheScheduler);
    TheScheduler->setThread(Scheduler);

    // Start Controll System Thread
    thread* Controller = new thread(watch,TheControlSystem);
    Controller->detach();

    // Show UI
    window.show();

    return application.exec();
}
