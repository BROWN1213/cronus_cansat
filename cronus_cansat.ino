#include "src/cansat_system.h"
#include<SimpleTimer.h>   //Task scheduler 

#define TIME_MARGIN 700  //700ms

// the timer scheduler object
SimpleTimer schedule_timer;
int heartbeat_timer_id;
bool isGpsDataNew=false;
void setup() {
  Serial.begin(115200);
  Serial.println("Cansat start");
  CansatSystemInit(); 
  setupLED();
  //setupBT();
  LEDBlinkAll(2000);

  //setupAHRS();
  setupGPS();//DO NOT setup AHRS after GPS, They use serial1
  //setupCamera();
  //setupNavigation();
  //setNavigationMode(1);

  heartbeat_timer_id=schedule_timer.setInterval(1000, heartbeat); //1Hz  

}

void loop() {
  // put your main code here, to run repeatedly:
  //LEDBlinkAll(1000);
  //updateAHRS();
  uint32_t loop_start_time;
  updateGPS();
  //gpsPassthrough();

  if(isGpsDataNew){ //main task every 1sec
    loop_start_time=millis(); 
    reconnectAhrs();
    while((millis()-loop_start_time)<TIME_MARGIN ){
      updateAHRS();
      schedule_timer.run();
    }


    
  }//if(isGpsDataNew)

  
  schedule_timer.run();
  //testWinch();
}

void heartbeat(){
  checkBattery();
  LED0toggle();
  gpsSearchingCheck();
  
}
