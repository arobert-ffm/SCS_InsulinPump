//
// File: Body.cpp
//
// Date: 24.12.14 17:11
//
// Generated by: Idatto, version 1.3
//
// Description: Simulates a body suffering from diabetes.

#include "Body.h"
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

using namespace std;

#define BUFLEN          100
#define EXIT__FAILURE   -1

int     main            (void);
int     main_offline    (void); // is working!

int     fdes_body_to_pump; // fildescriptor for Body --> Pump
int     fdes_pump_to_body; // fildescriptor for Pump --> Body
char    buffer;
int     i;

unsigned int sleeping = 500;

// Increases the blood sugar level depending on the strength parameter. Returns
// “True” when done.
//
// Parameter:
// - strength: The strength of raising the blood sugar level 1: calm 2: middle
//     3: fast
bool Body::increaseBloodsugar(int strength) {
    return true;
    
}

// Decreases the blood sugar level depending on the strength parameter. Returns
// “True” when done.
//
// Parameter:
// - strength: The strength of sinking the blood sugar level 1: calm 2: middle
//     3: fast
bool Body::decreaseBloodsugar(int strength)
{
    return true;
}

// Reacts to insulin and returns “True” when done.
//
// Parameter:
// - amount_injected: the amount of insulin injected
bool Body::reactToInsulin(float amount_injected)
{
    return true;
}

// Reacts to glucagon and returns “True” when done.
//
// Parameter:
// - amount: the amount of glucagon injected
bool Body::reactToGlucagon(float amount)
{
    return true;
}

//
//  Body.cpp
//  Body
//
//  Created by Johannes Kinzig on 04.01.15.
//  Copyright (c) 2015 Johannes Kinzig. All rights reserved.
//

/****************************************************************
 *               used to store data for transmission             *
 *****************************************************************/

/**********************************
 * transmit_hormone_injection      *
 **********************************/

struct transmit_injection_hormones {
    float injected_insulin;
    float injected_glucagon;
} Injecting; // will be send over pipe: pump_to_body

/**********************************
 * transmit_bloodsugar      *
 **********************************/

struct transmit_bloodsugar {
    float bloodSugarLevel;
} BodyStatus; // will be send over pipe: body_to_pump

/****************************************************************
 *                          END                                 *
 ****************************************************************/

// simulating BSL - should be inside a seperate thread
int main(void) {
    
    // setting the starting value
    BodyStatus.bloodSugarLevel = 80.00; // mg/dL
    
    // defined 3 levels for increasing/decreasing:
    //      Level 1: calm   --> 1,03
    //      Level 2: middle --> 1,06
    //      Level 3: fast   --> 1,09
    
    // generating the curve values
    while (true) {
        BodyStatus.bloodSugarLevel = BodyStatus.bloodSugarLevel * 1.03;
        cout << BodyStatus.bloodSugarLevel;
        cout << "\n";
        
        if (BodyStatus.bloodSugarLevel > 180) {
            cout << "Value reached!\n";
            break;
        }
    }
    
    return 0;
}

// communication via pipes
int main_offline (void)
{
    
    BodyStatus.bloodSugarLevel = 29.00;
    
    // generate pipe for Body --> Pump
    mknod("/Users/johanneskinzig/Documents/XcodeDev/body_to_pump",S_IFIFO | 0666,0);
    
    if((fdes_body_to_pump=open("/Users/johanneskinzig/Documents/XcodeDev/body_to_pump",O_WRONLY))==(-1)) {
        puts("Fehler 'open pipe'");
        exit(EXIT__FAILURE);
    }
    
    // generate pipe for Pump --> Body
    mknod("/Users/johanneskinzig/Documents/XcodeDev/pump_to_body",S_IFIFO | 0666,0);
    
    if((fdes_pump_to_body=open("/Users/johanneskinzig/Documents/XcodeDev/pump_to_body",O_RDONLY))==(-1)) {
        puts("Fehler 'open pipe'");
        exit(EXIT__FAILURE);
    }
    
    // write Body --> Pump
    if((i=write(fdes_body_to_pump, &BodyStatus, BUFLEN)) != BUFLEN) {
        printf("Fehler 'write-call'");
        exit(EXIT__FAILURE);
    }
    close(fdes_body_to_pump);
    
    // read Pump --> Body
    read(fdes_pump_to_body, &Injecting, BUFLEN);
    cout << Injecting.injected_insulin;
    cout << "\n";
    cout << Injecting.injected_glucagon;
    cout << "\n";
    close(fdes_pump_to_body);
    
    exit(0);
    
} /* END_MAIN() */
