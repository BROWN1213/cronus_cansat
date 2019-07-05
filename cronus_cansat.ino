#include "src/cansat_system.h"
#include<SimpleTimer.h>   //Task scheduler 


// the timer scheduler object
SimpleTimer schedule_timer;
int heartbeat_timer_id;

void setup() {
  Serial.begin(115200);
  Serial.println("Cansat start");
  CansatSystemInit(); 
  setupLED();
  setupBT();
  LEDBlinkAll(2000);

  //setupAHRS();
  setupGPS();//DO NOT setup AHRS after GPS, They use serial1
  //updateGPS();
  //setupCamera();
  //setupNavigation();
  //setNavigationMode(1);

  heartbeat_timer_id=schedule_timer.setInterval(1000, heartbeat); //1Hz  

}

void loop() {
  // put your main code here, to run repeatedly:
  //LEDBlinkAll(1000);
  //updateAHRS();
  updateGPS();
  //gpsPassthrough();
  
  schedule_timer.run();
  //testWinch();
}

void heartbeat(){
  checkBattery();
  LED0toggle();
  
}
