/**
 * @file:   Config.h
 *
 * @author: Sven Sperner, sillyconn@gmail.com
 *
 * @date:   08.02.2015
 *
 * @brief:  Representation for the system configuration
 *
 * Copyright (c) 2015 All Rights Reserved
 */


#ifndef config_
#define config_


/**
 * @name        Configuration
 * @brief       A structure for the system configuration
 *
 *  Hsf         Hormone Sensitivity Factor
 *  UpperLevel  Upper Bloodsugar Target Level
 *  LowerLevel  Lower BLoodsugar Target Level
 *  UpperLimit  Upper Bloodsugar Target Limit
 *  LowerLimit  Lower Bloodsugar Target Limit
 *  UpperAlarm  Upper Bloodsugar Target Alarm
 *  LowerAlarm  Lower Bloodsugar Target Alarm
 *  AbsMaxBSL   Absolute Maximum Bloodsugar
 *  ResWarn     Reservoir Warning Fill Level
 *  ResCrit     Reservoir Critical Fill Level
 *  BattWarn    Battery Warning Level
 *  BattCrit    Battery Critical Level
 *  MaxOpTime   Maximum Operation Time (h)
 *  SchedInt    Scheduler Interval (sec)
 *  ContrInt    Controller Interval (sec)
 */
struct config{
    int hsf;
    int upperLevel;
    int lowerLevel;
    int upperLimit;
    int lowerLimit;
    int upperAlarm;
    int lowerAlarm;
    int absMaxBSL;
    int resWarn;
    int resCrit;
    int battWarn;
    int battCrit;
    int maxOpTime;
    int schedInt;
    int contrInt;
};

#endif




