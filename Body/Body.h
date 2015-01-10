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

#ifndef body_
#define body_

#include <vector>
#include <string>

class Body {
    public:
    
    Body(float BSL, int insulin_constant, int gluc_constant);
    ~Body();
    
    // changes the blood sugar level;
    // increasing: if True: rising; if False: falling
    // strength: the factor the BSL is rising or falling
    virtual bool changeBloodSugarLevel(float strength, bool increasing, bool use_insulin_constant, bool use_glucagon_constant);
    virtual float getBloodSugarLevel();
    virtual void setBloodSugarLevel(float);
    
    private:
    float BloodsugarLevel;
    int insulin_constant;
    int glucagon_constant;

};
#endif
