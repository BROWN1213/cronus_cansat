/**
* @file 
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2019 Asgardia
* @date June 2019
* @brief ...
*/


#include "cansat_navigation.h"
#include "cansat_Debug.h"

#define MILLPER1T 1560.*1.5 //1.56sec per 1 turn. 1.5 is tuning value 1.5 PSCS
#define MAXTURN 3.
#define CONTROLL_MAX 0.75 // 180deg to 0.75 turn
#define WINCHMAX_CALLBACKTIME MILLPER1T*CONTROLL_MAX
#define MIN_CALLBACK_ANGLE 10

#define WINCH_MIN (uint16_t)(1500.- (600./MAXTURN*CONTROLL_MAX) ) //0.75 turn to 1650
#define WINCH_MAX (uint16_t)(1500.+ (600./MAXTURN*CONTROLL_MAX) )  //-0.75 turn to 1350
#define WINCH_CALLBACK_TIME(angle) (uint16_t)(WINCHMAX_CALLBACKTIME*fabs(angle)/180.)

CansatNavigation::CansatNavigation()
{

}
void CansatNavigation::begin(int pin,void (*winchCallback)(),int offset)
{
    _mode=true; //set to manual mode
    _winch_angle_offset=offset;
    _winch_servo_pin=pin;
    pinMode(pin, OUTPUT);
    _winch_servo.attach(pin);
    _control_angle=0; //
    winchNeutral();
    _pCallback=winchCallback;
    _timerId=0;


}
void CansatNavigation::setNavigationMode(bool mode)
{
    _mode=mode;
}
bool CansatNavigation::getNavigationMode()
{
    return _mode;
}

void CansatNavigation::updateNavigationParamers(float dist,float bearing, float course,float d_alt)
{
    _distance_from_destination=dist;
    _diff_altitude=d_alt;

    //convert 0~359 to -179~+180
    _bearing_angle_wrap180=bearing;
    if(_bearing_angle_wrap180>180)_bearing_angle_wrap180 -= 360;
    _course_angle_wrap180=course;
    if(_course_angle_wrap180>180)_course_angle_wrap180 -= 360;
}
void CansatNavigation::updateControlAngle()
{

    _control_angle=_bearing_angle_wrap180-_course_angle_wrap180;
    //To keep _control_angle into -179~+180
    if(_control_angle< -180){
        _control_angle+=360.f;
    }
    if(_control_angle> 180){
    _control_angle-=360.f;
    }
}

void CansatNavigation::timerRun(){
    _winchTimer.run();
}

void CansatNavigation::winchControl(float angle)
{
  uint16_t winch_neutal_time;
  if(!_mode){  // auto mode
    turnWinch( angleToMicrosec(_control_angle-_winch_angle_offset) );
    if(fabs(angle)>MIN_CALLBACK_ANGLE){
      winch_neutal_time=WINCH_CALLBACK_TIME(angle); 
      if(_winchTimer.getNumTimers()>0)_winchTimer.deleteTimer(_timerId);
      _timerId=_winchTimer.setTimeout(winch_neutal_time, _pCallback);
    }
    CANSAT_LOG("Turn winch(auto):");
    CANSAT_LOG1(_control_angle);
  }else{ //manual mode
    turnWinch( angleToMicrosec(angle-_winch_angle_offset) );
    if(fabs(angle)>MIN_CALLBACK_ANGLE){
      winch_neutal_time=WINCH_CALLBACK_TIME(angle); 
      //Serial.print("callback time=");
      //Serial.println(winch_neutal_time);
      if(_winchTimer.getNumTimers()>0)_winchTimer.deleteTimer(_timerId);
      _timerId=_winchTimer.setTimeout(winch_neutal_time, _pCallback);
    }
    CANSAT_LOG("Turn winch(manu):");
    CANSAT_LOG1(angle);
  }
}
void CansatNavigation::winchControlTurnAround(float angle,float turnaround_time)
{
  uint16_t winch_neutal_time;
  turnWinch( angleToMicrosec(angle-_winch_angle_offset) );
  if(fabs(angle)>MIN_CALLBACK_ANGLE){
    winch_neutal_time=WINCH_CALLBACK_TIME(angle)+ turnaround_time; 
      //Serial.print("callback time=");
      //Serial.println(winch_neutal_time);
      if(_winchTimer.getNumTimers()>0)_winchTimer.deleteTimer(_timerId);
      _timerId=_winchTimer.setTimeout(winch_neutal_time, _pCallback);
    }
  CANSAT_LOG("Turn Around winch");
  CANSAT_LOG1(angle);
}

void CansatNavigation::winchControlGetWind()
{
  uint16_t winch_neutal_time;
  float angle=0;
  float wind_time=6000;
  turnWinch( angleToMicrosec(angle-_winch_angle_offset) );
  if(fabs(angle)>MIN_CALLBACK_ANGLE){
    winch_neutal_time=WINCH_CALLBACK_TIME(angle)+ wind_time; 
      //Serial.print("callback time=");
      //Serial.println(winch_neutal_time);
      if(_winchTimer.getNumTimers()>0)_winchTimer.deleteTimer(_timerId);
      _timerId=_winchTimer.setTimeout(winch_neutal_time, _pCallback);
    }
  CANSAT_LOG("Turn winch for get wind");
  CANSAT_LOG1(angle);
}

void CansatNavigation::winchNeutral(){
  turnWinch( angleToMicrosec(0-_winch_angle_offset) );
}
float CansatNavigation::getControlAngle()
{
  return _control_angle;
}

void CansatNavigation::printNavigationInfo()
{

  CANSAT_LOG("** Navi br:cr:dst:alt  **");
  Serial.print( _bearing_angle_wrap180);Serial.print(F(":"));
  Serial.print( _course_angle_wrap180);Serial.print(F(":"));
  Serial.print( _distance_from_destination);Serial.print(F(":"));
  Serial.println( _diff_altitude);

  Serial.print(F("** Turn = "));
  Serial.println( _control_angle);
}
void CansatNavigation::turnWinch(uint16_t microsec)
{
  // 1500 neutral position
  // 900 fully counter-clockwise, 2100 fully clockwise
  if(microsec<900 || microsec>2100){
    return;
  }
  _winch_servo.writeMicroseconds(microsec);

}

uint16_t CansatNavigation::angleToMicrosec(float angle)
{
  uint16_t angle_micro;  //900~2100
  //the angle is -179~180

  angle=0-angle;
  if(angle<= -179 || angle>=180){
    CANSAT_LOG("limit winch angle");
    Serial.println(angle);
    if(angle>=180)angle=180;
    if(angle<=-179)angle=-179;
    //return;
  }

  //angle_micro=(uint16_t)map(angle,-179,180,900,2100); //max 3 turn too much turn
  //angle_micro=(uint16_t)map(angle,-179,180,1100,1900);  // max 2 turn
  //angle_micro=(uint16_t)map(angle,-179,180,1200,1800);  // max 1.5 turn
  //angle_micro=(uint16_t)map(angle,-179,180,1350,1650);  // max 0.75 turn
  angle_micro=(uint16_t)map(angle,-179,180,WINCH_MIN,WINCH_MAX);
  //Serial.print(WINCH_MIN);Serial.println(WINCH_MAX);
  return angle_micro;
}
/* 
MngNavigation::MngNavigation()
{

}

void MngNavigation::attach(int pin)
{
  _winch_servo_pin=pin;
  _winch_servo.attach(pin);
}
void MngNavigation::turnWinch(uint16_t microsec)
{
  // 1500 neutral position
  // 900 fully counter-clockwise, 2100 fully clockwise
  if(microsec<900 || microsec>2100){
    return;
  }
  _winch_servo.writeMicroseconds(microsec);

}

uint16_t MngNavigation::angleToMicrosec(float angle)
{
  uint16_t angle_micro;  //900~2100
  //the angle is -179~180
  angle=0-angle;
  if(angle< -179 || angle>180){
    Serial.print(F("Wrong winch angle"));
    Serial.println(angle);
    return;
  }
  //angle_micro=(uint16_t)map(angle,-179,180,900,2100); //max 3 turn too much turn
  //angle_micro=(uint16_t)map(angle,-179,180,1100,1900);  // max 2 turn
  //angle_micro=(uint16_t)map(angle,-179,180,1200,1800);  // max 1.5 turn
  //angle_micro=(uint16_t)map(angle,-179,180,1350,1650);  // max 0.75 turn
  angle_micro=(uint16_t)map(angle,-179,180,WINCH_MIN,WINCH_MAX);
  //Serial.print(WINCH_MIN);Serial.println(WINCH_MAX);
  return angle_micro;
}

*/
