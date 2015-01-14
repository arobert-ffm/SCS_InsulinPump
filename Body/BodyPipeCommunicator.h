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
#define bodyPipeCommunicator_

#include <vector>
#include <string>

class BodyPipeCommunicator {

    public:
    // constructor - destructor
    BodyPipeCommunicator();
    ~BodyPipeCommunicator();
    
    // getter - setter methods
    void setSendBSL(int BSL);
    int getSendBSL(void);
    
    void setRecvGlucagon(int gluc);
    int getRecvGlucagon(void);
    
    void setRecvInsulin(int insl);
    int getRecvInsulin(void);
    
    // methods for sending and receiving from pipe
    void sendBSL();
    void recvAgents();
    
    
    private:
    int SendBSLVar;
    int RecvGlucagonVar;
    int RecvInsulinVar;

};
#endif