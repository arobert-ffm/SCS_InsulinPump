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
#include "BodyPipeCommunicator.h"

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

#define BUFLEN          100 //<--- sizeof(struct) inststead of hard coded valuew
#define EXIT__FAILURE   -1

int     main                        (void);
int     BSL_Sim_thread              (void); // is working
int     Sim_Controll_Thread         (void); // implemented right now

int     fdes_body_to_pump; // fildescriptor for Body --> Pump
int     fdes_pump_to_body; // fildescriptor for Pump --> Body
int     i; // needed for communication over pipes


/*****************************************************************
 *               used to store data for transmission             *
 *****************************************************************/

/**********************************
 * transmit_hormone_injection     *
 **********************************/
struct transmit_injection_hormones {
    int injected_insulin;
    int injected_glucagon;
} Injecting; // will be send over pipe: pump_to_body

/**********************************
 *       transmit_bloodsugar      *
 **********************************/
struct transmit_bloodsugar {
    int bloodSugarLevel;
} BodyStatus; // will be send over pipe: body_to_pump
/****************************************************************
 *                      END transmission                        *
 ****************************************************************/

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
 *            Class for pipe communication            *
 ******************************************************/
BodyPipeCommunicator::BodyPipeCommunicator(void) {
    // generate pipe for Body --> Pump
    mknod("body_to_pump",S_IFIFO | 0666,0);
    
    if((fdes_body_to_pump=open("body_to_pump",O_WRONLY))==(-1)) {
        puts("Fehler 'open pipe'");
        exit(EXIT__FAILURE);
    }
    
    // generate pipe for Pump --> Body
    mknod("pump_to_body",S_IFIFO | 0666,0);
    
    if((fdes_pump_to_body=open("pump_to_body",O_RDONLY))==(-1)) {
        puts("Fehler 'open pipe'");
        exit(EXIT__FAILURE);
    }

};
BodyPipeCommunicator::~BodyPipeCommunicator(void) {
};
// getter - setter methods
void BodyPipeCommunicator::setSendBSL(int BSL) {
    this->SendBSLVar = BSL;
}
int BodyPipeCommunicator::getSendBSL(void) {
    return this->SendBSLVar;
}

void BodyPipeCommunicator::setRecvGlucagon(int gluc) {
    this->RecvGlucagonVar = gluc;
}
int BodyPipeCommunicator::getRecvGlucagon(void) {
    return this->RecvGlucagonVar;
}

void BodyPipeCommunicator::setRecvInsulin(int insl) {
    this->RecvInsulinVar = insl;
}
int BodyPipeCommunicator::getRecvInsulin(void) {
    return this->RecvInsulinVar;
}

void BodyPipeCommunicator::sendBSL() {
    // write Body --> Pump
    if((i=write(fdes_body_to_pump, &BodyStatus, BUFLEN)) != BUFLEN) {
        printf("Fehler 'write-call'");
        exit(EXIT__FAILURE);
    }
    close(fdes_body_to_pump);
    
}

void BodyPipeCommunicator::recvAgents() {
    // read Pump --> Body
    read(fdes_pump_to_body, &Injecting, BUFLEN);
    cout << Injecting.injected_insulin;
    cout << "\n";
    cout << Injecting.injected_glucagon;
    cout << "\n";
    close(fdes_pump_to_body);
}
/******************************************************
 *          END Class for pipe communication          *
 ******************************************************/



/******************************************************
 *                       Main                         *
 ******************************************************/

Body body(110.00, 5, 5); // generate Body object - natural BSL, insulin constant, glucagon constant
BodyThreadController communication; // generate object for communication


int main(void) {
    
    // Initialize values
    communication.setThreadBodyFactor(1.03);
    communication.setThreadRising(false);
    communication.setThreadInsulinUnits(0);
    communication.setThreadGlucagonUnits(0);
    communication.setThreadEndThread(false);
    
    
    cout << "Start\n";
    
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
    cout << "\nThread started\n";
    
    ofstream logfile;
    
    cout << "Init value for BloodSugarLevel: ";

    
    while (true) {
        
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
        
        logfile.open("log.txt", ios::out | ios::app);
        logfile << body.getBloodSugarLevel();
        logfile << "\n";
        logfile.close();
        usleep(1000000);
        
        if (communication.getThreadEndThread() == true) {
            break;
        }

    }
    cout << "\nThread ended\n";
    return 0;
}
/******************************************************
 *                 END BSL-Simulator                  *
 ******************************************************/



