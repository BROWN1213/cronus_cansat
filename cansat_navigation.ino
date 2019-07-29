#include "src/cansat_navigation.h"
#include "src/cansat_Debug.h"

CansatNavigation cansatNavigation;

void setupNavigation(){
    cansatNavigation.begin(D0,winchCallback);

}

void timerRun(){
  cansatNavigation.timerRun();
  
}
void winchCallback(){
  cansatNavigation.winchNeutral();
  CANSAT_LOG("winch callback");
}

void setNavigationMode(bool mode){
    //mode 0 :auto
    //mode 1 :manual
    cansatNavigation.setNavigationMode(mode);

}

bool getNavigationMode(){
    return cansatNavigation.getNavigationMode();
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
