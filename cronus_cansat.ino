#include "src/cansat_system.h"
#include<SimpleTimer.h>   //Task scheduler 

#define TIME_MARGIN 700  //700ms

enum CansatStatus {
  SETUP_COMPLETED = 0,
  READY_FOR_LAUNCH = 1,
  LAUNCH_DETECTED = 2,
  FREEFALL_DETECTED = 3,
  PARACHUTE_DEVELPED = 4, //or 
  AUTOPILOT_ACTIVATED = 5,
  TURN_AROUND = 6,
  LANDING = 7,
};
CansatStatus cansatStatus;
// the timer scheduler object
SimpleTimer schedule_timer;

int heartbeat_timer_id;
int PM2_5_timer_id;
bool isGps_data_fix=false;  // Means gps data is valid
bool is_turnaround_started=false;
bool turnaround_permission=false;
bool getwind_permission=false;
void setup() {
  CansatSystemInit();   
  Serial.begin(115200);
  Serial.println("Cansat start");
  setupLED();
  //setupBT(); // actually we don't need a setupBT(), Serial.begin(115200) is enough
  //setupAHRS();
  setupGPS();//DO NOT setup AHRS after GPS, They use serial1
  //setupCamera();
  setupLocation();
  setupNavigation(1);// set manual mode 1 , auto 0
  setupPM2_5();
  heartbeat_timer_id=schedule_timer.setInterval(1000, heartbeat); //1Hz
  PM2_5_timer_id=schedule_timer.setInterval(5000, updatePM2_5);   //5Hz 
  setupCmdMessenger();
}

void loop() {

  uint32_t loop_start_time;
  
  bool loop_sync;
  
  updateGPS(); 
  // choose loop_sync depend on gps status
  // if there is no GPS then use gpsAvailable() for loop_sync
  loop_sync= isGpsLocked() ? isGps_data_fix:gpsAvailable();
  
  if( loop_sync){ //main task every 1sec
    loop_start_time=millis(); 
    if(isGpsLocked()){
      isGps_data_fix=false;
      if(updateLocation()){
        if(is_turnaround_started==false&&turnaround_permission==false&&getwind_permission==false) updateNavigation();
        updateturnaround();
        getwind();
      }     
    }
    timerRun();
    schedule_timer.run();
    cmdMessengerRun();
    reconnectAhrs();
        
    while((millis()-loop_start_time)<TIME_MARGIN ){
      updateAHRS();
      freeFallDetection();
      timerRun();
      cmdMessengerRun();
    }
    
    reconnectGPS();  
  }   //if(isGpsDataNew)
  schedule_timer.run();
  timerRun();
  cmdMessengerRun();
  cansatStatus =SETUP_COMPLETED;
}

void heartbeat(){
  checkBattery();
  LED0toggle();
  gpsSearchingCheck();
  
}
