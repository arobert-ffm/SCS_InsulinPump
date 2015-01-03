// 
// File: Pump.cpp
// 
// Date: 24.12.14 17:11
// 
// Generated by: Idatto, version 1.3
// 
// Description:

#include "Pump.h"



// Injects the insulin into the body.
// 
// Parameter:
// - amount: The amount of insulin which is injected into the body.
bool Pump::injectInsulin(float amount)
{
}

// Injects the glucagon into the body.
// 
// Parameter:
// - amount: The amount of glucagon which is injected into the body.
bool Pump::injectGlucagon(float amount)
{
}

// Checks the blood sugar concentration and returns the value.
float Pump::getBloodsugar()
{
}

// refills insulin and returns “True” when done
bool Pump::refillInsulin()
{
}

// refills glucagon and returns “True” when done
bool Pump::refillGlucagon()
{
}

// decreases insulin level in reservoir when injected to body and returns 
// “True” when done 
// 
// Parameter:
// - amount: The amount of insulin which is injected into the body needs to be 
//     reduced in the reservoir. 
bool Pump::decreaseInsulinLevel(float amount)
{
}

bool Pump::decreaseGlucagonLevel(float amount)
{
}

// Checks the battery status and returns the value in percent.
// In case of a critical status (level smaller than 15%) the user will be 
// notified acoustically and the incident will be logged by the tracer. 
int Pump::getBatteryStatus()
{
}

// Checks the entire pump (reservoir, mechanical parts) and returns “True” when 
// everything is working fine. 
bool Pump::getStatus()
{
}

// Calculates the amount of insulin needed based on the blood sugar levels.
float Pump::calculateNeededInsulin()
{
}

// Calculates the amount of glucagon needed based on the blood sugar levels.
float Pump::calculateNeededGlucagon()
{
}

// Returns the insulin level in the reservoir.
float Pump::getInsulinLevel()
{
}

// Returns the glucagon level in the reservoir.
float Pump::getGlucagonLevel()
{
}


// needed to be realized here just for testing purpose, will be moved later on to correct location
// and can be used with a method invocation later on

int main(){

	// Main process for pump and its components/classes
	// Generating two pipes, 

	return 0;

}



