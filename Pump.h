// 
// File: Pump.h
// 
// Date: 24.12.14 17:11
// 
// Generated by: Idatto, version 1.3
// 
// Description:


#ifndef pump_
#define pump_

#include <vector>
#include <string>
#include "Tracer.h"
#include <QObject>

using namespace std;



class Pump : public QObject
{
    Q_OBJECT

    public:
        // refills insulin and returns “True” when done
        virtual bool refillInsulin();
        // refills glucagon and returns “True” when done
        virtual bool refillGlucagon();
        // Checks the battery status and returns the value in percent.
        // In case of a critical status (level smaller than 15%) the user will 
        // be notified acoustically and the incident will be logged by the 
        // tracer. 
        virtual int getBatteryStatus();
        // Checks the entire pump (reservoir, mechanical parts) and returns 
        // “True” when everything is working fine. 
        virtual bool getStatus();

    private:
        // When pump is active injecting insulin the value will be 1, when 
        // injecting glucagon the value will be 2 and when inactive the value 
        // will be 0. 
        int active;
        float InsulinLevel;
        float GlucagonLevel;
        // Injects the insulin into the body.
        // 
        // Parameter:
        // - amount: The amount of insulin which is injected into the body.
        virtual bool injectInsulin(float amount);
        // Injects the glucagon into the body.
        // 
        // Parameter:
        // - amount: The amount of glucagon which is injected into the body.
        virtual bool injectGlucagon(float amount);
        // Checks the blood sugar concentration and returns the value.
        virtual float getBloodsugar();
        // decreases insulin level in reservoir when injected to body and 
        // returns “True” when done 
        // 
        // Parameter:
        // - amount: The amount of insulin which is injected into the body 
        //     needs to be reduced in the reservoir. 
        virtual bool decreaseInsulinLevel(float amount);
        virtual bool decreaseGlucagonLevel(float amount);
        // Calculates the amount of insulin needed based on the blood sugar levels.
        virtual float calculateNeededInsulin();
        // Calculates the amount of glucagon needed based on the blood sugar 
        // levels. 
        virtual float calculateNeededGlucagon();
        // Returns the insulin level in the reservoir.
        virtual float getInsulinLevel();
        // Returns the glucagon level in the reservoir.
        virtual float getGlucagonLevel();

    signals:
        // Callback for updating Insulin Reservoir in the UI.
        //
        // Parameter:
        // - The current amount of insulin in the reservoir
        void updateInsulinReservoir(float amount);
        // Callback for updating Glucagon Reservoir in the UI.
        //
        // Parameter:
        // - The current amount of glucagon in the reservoir
        void updateGlucagonReservoir(float amount);

};

#endif
