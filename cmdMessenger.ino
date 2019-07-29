#include <CmdMessenger.h>  // CmdMessenger
CmdMessenger cmdMessenger = CmdMessenger(Serial);

enum
{
  kAcknowledge, //0
  kError, //1
  kFalling, // 2
  kHomePosition, //3
  kFlyMode, //4
  kManualControl, //5
  
};

void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kFalling, OnFalling);
  cmdMessenger.attach(kHomePosition, OnHomePosition);
  cmdMessenger.attach(kFlyMode, OnFlyMode); 
  cmdMessenger.attach(kManualControl, OnManualControl);
  
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(kError,F("Command without attached callback"));
}


// Callback function that sets falling
void OnFalling()
{
  // Read led state argument, interpret string as boolean
  int trigger;
  trigger = cmdMessenger.readInt16Arg();
  if(trigger==1){
    cmdMessenger.sendCmd(kAcknowledge,F("Hooking position"));
    //tskFreeFall.hookingPosition();
  }
  if(trigger==2){
    cmdMessenger.sendCmd(kAcknowledge,F("free falling"));
    //tskFreeFall.releasePosition();
  }
  if(trigger==0){
    cmdMessenger.sendCmd(kAcknowledge,F("ready position"));
    //tskFreeFall.readyPosition();
  }  
}

void OnHomePosition(){

  if(cansatGPS.num_sats()>=6){  
    float lat,lng,alt;
    lat=cansatGPS.location().lat;
    lng=cansatGPS.location().lng;
    alt=cansatGPS.location().alt;
    cansatLocation.setDestination(lat,lng,alt);
    cmdMessenger.sendCmd(kAcknowledge,F("Home Position updated"));
    cansatBT.send(F("lat:lng:alt= "));
    cansatBT.send(lat);
    cansatBT.send(lng);
    cansatBT.sendln(alt);
  }else{
    cmdMessenger.sendCmd(kAcknowledge,F("Home Position Fail"));
    Serial.println(F("We need at least 6 satellites ")); 
    
  }
}

void OnFlyMode(){
  // Read led state argument, 
  
  int trigger;
  trigger = cmdMessenger.readInt16Arg();
  if(trigger==0){ //auto mode
    if(!getNavigationMode()){
       Serial.println(F("Navigation already automode ")); 
    }else{
       setNavigationMode(0);
    }    
    cmdMessenger.sendCmd(kAcknowledge,F("Navigation:auto"));

  }
  if(trigger==1){ //manual mode
    if(getNavigationMode()){
       Serial.println(F("Navigation already manualmode ")); 
    }else{
       setNavigationMode(1);
    }    
    cmdMessenger.sendCmd(kAcknowledge,F("Navigation:manual"));
  }  
}


void OnManualControl(){
  // Read led state argument, 
  float trigger;
  
  if(!getNavigationMode()){
    cmdMessenger.sendCmd(kAcknowledge,F("Control fail..Set manualmode first!!!")); 
    return;  
  }

  trigger = cmdMessenger.readInt16Arg();
  winchControl((float)trigger*(float)89.);
  cmdMessenger.sendCmd(kAcknowledge,"manual Contol Ok"); 
  Serial.print("Control angle="); 
  Serial.println((float)trigger*(float)89.); 
  
}
