/**
* @file 
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2019 Asgardia
* @date June 2019
* @brief ...
*/


#ifndef CANSAT_NAV_H
#define CANSAT_NAV_H

#include "Arduino.h"
#include<Servo.h>
#include<SimpleTimer.h>


class CansatNavigation
{
  public:
    CansatNavigation();
    void begin(int pin,void (*winchCallback)(),int offset);
    void updateNavigationParamers(float dist,float bearing, float course,float d_alt);
    void updateControlAngle();
    void printNavigationInfo();
    void setNavigationMode(bool mode);
    bool getNavigationMode();
    void winchControl(float angle);
    void winchControlTurnAround(float angle,float turnaround_time);
    void winchNeutral();
    float getControlAngle();
    void timerRun();
    void turnWinch(uint16_t microsec);
    uint16_t angleToMicrosec(float angle);

  private:
    float _distance_from_destination;
    float _bearing_angle_wrap180;
    float _course_angle_wrap180;
    float _control_angle;
    float _diff_altitude;
    uint32_t _winch_angle;
    int _winch_angle_offset;
    bool _mode;  // false: automode, true: manual mode

    int _winch_servo_pin;
    Servo _winch_servo;

    SimpleTimer _winchTimer;
    int _timerId;
    void (*_pCallback)(void);
};




#endif
