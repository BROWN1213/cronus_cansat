#include "src/cansat_navigation.h"
#include "src/cansat_Debug.h"

CansatNavigation cansatNavigation;

float ground_alt;

void setupNavigation(bool mode){
  int offset_angle=-6;
  cansatNavigation.begin(D0,winchCallback,offset_angle);
  setNavigationMode(mode);
  cansatNavigation.winchNeutral();
  Serial.println(F("Navigation begin"));
}

void updateNavigation(){
  ground_alt=cansatLocation.getGroundAltitude();
  //update navigation parameters
  if(destination_locked){
    cansatNavigation.updateNavigationParamers(location_distance,location_bearing,cansatGPS.ground_course(),ground_alt);
    //update control angle
    cansatNavigation.updateControlAngle();
  
    // turn winch
    if(isAutoMode()&& (cansatGPS.status()>=2)){ //GPS_FIX_TYPE_2D_FIX
      cansatNavigation.winchControl(0); // ignore angle when auto mode
    }
    //send info to base station
    Serial.print("%,6,2,");  // header,class,num data
    Serial.print(cansatNavigation.getControlAngle());Serial.print(',');
    Serial.println(ground_alt); 
    
  }
}
void timerRun(){
  cansatNavigation.timerRun();
  
}
void winchCallback(){
  cansatNavigation.winchNeutral();
  CANSAT_LOG("winch callback");
}

void setNavigationMode(bool mode){
    //mode 0 : auto
    //mode 1 : manual
    cansatNavigation.setNavigationMode(mode);

}

bool isAutoMode(){
    return !(cansatNavigation.getNavigationMode());
}

void winchControl(float angle){
    cansatNavigation.winchControl(angle);
}

uint32_t prev_time;

void testWinch(){
  uint32_t t_now=millis();
  if(t_now-prev_time>8000){
    prev_time=t_now;
    winchControl(50);
    
  }
  timerRun();
}
