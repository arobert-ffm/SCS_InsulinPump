//
//
//  Body.cpp
//  Body
//
//  Created by Johannes Kinzig on 04.01.15.
//  Copyright (c) 2015 Johannes Kinzig. All rights reserved.
//
//  Description: Simulates a body suffering from diabetes and reacting to insulin and/or glucagon.
//

#include "Body.h"
#include "BodyThreadController.h"
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <thread>
#include <fstream>

using namespace std;

#define BUFLEN          100 //<--- sizeof(struct) inststead of hard coded value
#define EXIT__FAILURE   -1

int     main                        (void);
int     BSL_Sim_thread              (void); // is working
int     Sim_Controll_Thread         (void); // seems to be working --> testing needed

// generate chars for pipe buffers
char symbols_insulin[3];
char symbols_glucagon[3];

/*****************************************************************
 *                   Class: ThreadController                     *
 *****************************************************************/
// constructor, destructor
BodyThreadController::BodyThreadController() {
}
BodyThreadController::~BodyThreadController() {

}

//ThreadBodyFactor -- tells the thread the body factor
void BodyThreadController::setThreadBodyFactor(float factor){
    this->ThreadBodyFactor = factor;
}
float BodyThreadController::getThreadBodyFactor(void) {
    return this->ThreadBodyFactor;
}

// ThreadRising -- tells the thread to rise or fall the BSL level
void BodyThreadController::setThreadRising(bool value) {
    this->ThreadRising = value;
}
bool BodyThreadController::getThreadRising(void) {
    return this->ThreadRising;
}

// ThreadGlucagonUnits -- tells the thread the amount of glucagon units to use
void BodyThreadController::setThreadGlucagonUnits(int units) {
    this->ThreadGlucagonUnits = units;
}

int BodyThreadController::getThreadGlucagonUnits() {
    return this->ThreadGlucagonUnits;
}

void BodyThreadController::minusThreadGlucagonUnits(int value) {
    this->ThreadGlucagonUnits = this->ThreadGlucagonUnits - value;
}

// ThreadInsulinUnits -- tells the thread the amount of inuslin units to use
void BodyThreadController::setThreadInsulinUnits(int units) {
    this->ThreadInsulinUnits = units;
}

int BodyThreadController::getThreadInsulinUnits() {
    return this->ThreadInsulinUnits;
}

void BodyThreadController::minusThreadInsulinUnits(int value) {
    this->ThreadInsulinUnits = this->ThreadInsulinUnits - value;
}

// ThreadEndThread -- tells the thread to terminate
void BodyThreadController::setThreadEndThread(bool value) {
    this->ThreadEndThread = value;
}

bool BodyThreadController::getThreadEndThread() {
    return this->ThreadEndThread;
}
/****************************************************************
 *                  END BodyThreadController                    *
 ****************************************************************/


/****************************************************************
 *                      Class: Body                             *
 ****************************************************************/

// constructor
Body::Body(float BSL, int in_constant, int gluc_constant){
    BloodsugarLevel = BSL; // unit: mg/dL
    insulin_constant = in_constant; // unit: mg/dL sinking per iteration => one iteration estimated as 0.5 hours
    glucagon_constant = gluc_constant; // glucagon which influences the BSL per 0.5 hours
    
}

// destructor
Body::~Body(){
    
}

/******************************************************
 *   defined 3 levels for increasing/decreasing:      *
 *      Level 1: calm   --> 1.03                      *
 *      Level 2: middle --> 1.06                      *
 *      Level 3: fast   --> 1.09                      *
 ******************************************************/

bool Body::changeBloodSugarLevel(float strength, bool increasing, bool use_insulin_constant, bool use_glucagon_constant) {
    
    if (increasing == false && use_insulin_constant == false && use_glucagon_constant == false) {
        // BSL falling with body factor
        this->BloodsugarLevel = this->BloodsugarLevel / strength;
    }
    
    else if (increasing == false && use_insulin_constant == false && use_glucagon_constant == true) {
        //BSL falling with body factor + adding the glucagon constant
        this->BloodsugarLevel = this->BloodsugarLevel / strength;
        this->BloodsugarLevel = this->BloodsugarLevel + this->glucagon_constant;
    }
    
    else if (increasing == false && use_insulin_constant == true && use_glucagon_constant == false) {
        // BSL falling with body factor + falling with the additional insulin constant
        this->BloodsugarLevel = this->BloodsugarLevel / strength;
        this->BloodsugarLevel = this->BloodsugarLevel - this->insulin_constant;
    }
    
    else if (increasing == false && use_insulin_constant == true && use_glucagon_constant == true) {
        // injecting insulin and glucagon at the same time??? does this make sense???
    }
    
    else if (increasing == true && use_insulin_constant == false && use_glucagon_constant == false) {
        // rising gently
        this->BloodsugarLevel = this->BloodsugarLevel * strength;
    }
    
    else if (increasing == true && use_insulin_constant == false && use_glucagon_constant == true) {
        // rising with body factor + adding glucagon constant
        this->BloodsugarLevel = this->BloodsugarLevel * strength;
        this->BloodsugarLevel = this->BloodsugarLevel + this->glucagon_constant;
    }
    
    else if (increasing == true && use_insulin_constant == true && use_glucagon_constant == false) {
        // rising with body factor + subtracting the insulin constant
        this->BloodsugarLevel = this->BloodsugarLevel * strength;
        this->BloodsugarLevel = this->BloodsugarLevel - this->insulin_constant;
    }
    
    else if (increasing == true && use_insulin_constant == true && use_glucagon_constant == true) {
        // rising + injecting insulin + injecting glucagon at the same time, does this make sense???
    }
    
    return true;
    
}

/******************************************************
 *      declaring getter and setter methods           *
 *      for private var BloodsugarLevel               *
 ******************************************************/
void Body::setBloodSugarLevel(float BSL) {
    this->BloodsugarLevel = BSL;
}

float Body::getBloodSugarLevel() {
    return this->BloodsugarLevel;
}

/****************************************************************
 *                          END Body                            *
 ****************************************************************/


/******************************************************
 *                       Main                         *
 ******************************************************/

Body body(110.00, 5, 5); // generate Body object - natural BSL, insulin constant, glucagon constant
BodyThreadController communication; // generate object for communication


int main(void) {

    cout << "Start\n";
    
    // Init values
    communication.setThreadBodyFactor(1.03);
    communication.setThreadRising(false);
    communication.setThreadInsulinUnits(0);
    communication.setThreadGlucagonUnits(0);
    communication.setThreadEndThread(false);
    
    thread first_thread(Sim_Controll_Thread);
    thread second_thread(BSL_Sim_thread);
    
    first_thread.join();
    second_thread.join();
    
    cout << "End\n";
    return 0;
}
/******************************************************
 *                      END main                      *
 ******************************************************/


/******************************************************
 *              Simulation-Controller                 *
 ******************************************************/
int Sim_Controll_Thread(void) {
    
    int user_bsl_ris_fal;
    
    cout << "Body simulator for SCS-Project InsulinPump\nV1.0\n\n";
    
    while (true) {
        cout << "Please set:\n 1: Eating a lot of sweets (BSL rising fast)\n 2: BSL Eating a snack (BSL rising moderate) \n 3: Drinking water (BSL falling slowly)\n 4: Doing sports (BSL falling)\n 5: End Simulation\n";
        cout << "Option: ";
        cin >> user_bsl_ris_fal;
        cout << "Your choice: " << user_bsl_ris_fal << "\n";
    
        if (user_bsl_ris_fal == 1) {
            communication.setThreadRising(true);
            communication.setThreadBodyFactor(1.04);
        }
        else if (user_bsl_ris_fal == 2) {
            communication.setThreadRising(true);
            communication.setThreadBodyFactor(1.02);
        }
        else if (user_bsl_ris_fal == 3) {
            communication.setThreadRising(false);
            communication.setThreadBodyFactor(1.01);
        }
        
        else if (user_bsl_ris_fal == 4) {
            communication.setThreadRising(false);
            communication.setThreadBodyFactor(1.05);
        }
        
        else if (user_bsl_ris_fal == 5) {
            communication.setThreadEndThread(true);
            break;
        }
        else {
            cout << "Not a valid option. Please try again!\n";
        }
        
        
    }
    
    return 0;
}
/******************************************************
 *            END Simulation-Controller               *
 ******************************************************/


/******************************************************
 *                  BSL-Simulator                     *
 ******************************************************/
int BSL_Sim_thread(void) {
    int insulin_amount, glucagon_amount;

    cout << "\nThread started\n";
    
    ofstream logfile;
    
    cout << "Init value for BloodSugarLevel: " << body.getBloodSugarLevel() << endl;

    
    while (true) {
        
        if (communication.getThreadEndThread() == true) {
            break;
        }
        
        /******************************************************
         *      Communication between body and pump           *
         ******************************************************/
        
        // write Body --> Pump
        ofstream out_pipe("pipe_to_pump", ios_base::out);
        out_pipe << (int)body.getBloodSugarLevel();
        out_pipe.close();
        
        if (communication.getThreadEndThread() == true) {
            break;
        }

        /******************************************************
         *       End Communication between body and pump      *
         ******************************************************/
        
        
        // generate BSL graph by reacting or not reacting to insulin
        if ((communication.getThreadInsulinUnits()) > 0 && communication.getThreadGlucagonUnits() == 0) {
            body.changeBloodSugarLevel(communication.getThreadBodyFactor(), communication.getThreadRising(), true, false);
            communication.minusThreadInsulinUnits(1);
        }
        
        else if (communication.getThreadInsulinUnits() == 0 && communication.getThreadGlucagonUnits() == 0) {
            body.changeBloodSugarLevel(communication.getThreadBodyFactor(), communication.getThreadRising(), false, false);
        }
        
        // generate BSL graph by reacting or not reacting to glucagon
        else if (communication.getThreadGlucagonUnits() > 0 && communication.getThreadInsulinUnits() == 0) {
            body.changeBloodSugarLevel(communication.getThreadBodyFactor(), communication.getThreadRising(), false, true);
            communication.minusThreadGlucagonUnits(1);
        }
        
        
        /******************************************************
         *      Communication between body and pump           *
         ******************************************************/

        // read Pump --> Body
        ifstream in_pipe; // pipe_to_body
        in_pipe.open("pipe_to_body", ios_base::in);
        
        while (!(in_pipe.good())) {
            /*
            cout << "Pipe not available!\nWaiting..." << endl;
            */
            in_pipe.close();
            usleep(100000);
            in_pipe.open("pipe_to_body", ios_base::in);
        }
        
        if (in_pipe.good()) {
            in_pipe.seekg(0, ios::beg);
            in_pipe.read(symbols_insulin, 2);
            in_pipe.seekg(2, ios::beg);
            in_pipe.read(symbols_glucagon, 2);
            
            in_pipe.close();
            
            remove("pipe_to_body");

            insulin_amount = atoi(symbols_insulin);
            glucagon_amount = atoi(symbols_glucagon);
            if( insulin_amount == 0 && glucagon_amount == 0 )
            {
                cout << "\rInjected Insulin: " << atoi(symbols_insulin)
                     << ", Injected Glucagon: " << atoi(symbols_glucagon)
                     << flush;
            }
            else
            {
                cout << "\rInjected Insulin: " << atoi(symbols_insulin)
                     << ", Injected Glucagon: " << atoi(symbols_glucagon)
                     << endl << flush;
            }
            fflush(stdout);

            communication.setThreadInsulinUnits(atoi(symbols_insulin));
            communication.setThreadGlucagonUnits(atoi(symbols_glucagon));
            
        }
        else {
            cout << "Communication is not possible, broken pipe!\n";
        }
         
        /******************************************************
         *       End Communication between body and pump      *
         ******************************************************/


        logfile.open("log.txt", ios::out | ios::app);
        logfile << body.getBloodSugarLevel();
        logfile << "\n";
        logfile.close();
        //usleep(100000);

    }
    cout << "\nThread ended\n";
    return 0;
}
/******************************************************
 *                 END BSL-Simulator                  *
 ******************************************************/



