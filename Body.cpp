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

using namespace std;


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


int main()
{

    return 0;
}




