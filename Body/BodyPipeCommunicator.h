//
//
//  Body.cpp
//  Body
//
//  Created by Johannes Kinzig on 09.01.15.
//  Copyright (c) 2015 Johannes Kinzig. All rights reserved.
//
//  Description: Simulates a body suffering from diabetes and reacting to insulin and/or glucagon.
//

#ifndef bodyPipeCommunicator_
#define bodyPipecommunicator_

#include <vector>
#include <string>

class bodyPipeCommunicator {

    public:
    // constructor - destructor
    bodyPipeCommunicator();
    ~bodyPipeCommunicator();
    
    // getter - setter methods
    void setSendBSL(int BSL);
    int getSendBSL(void);
    
    void setRecvGlucagon(int gluc);
    int getRecvGlucagon(void);
    
    void setRecvInsulin(int insl);
    int getRecvInsulin(void);
    
    
    private:
    int SendBSL;
    int RecvGlucagon;
    int RecvInsulin;

}