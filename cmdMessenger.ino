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
  kTurnAround, //6
};

void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kFalling, OnFalling);
  cmdMessenger.attach(kHomePosition, OnHomePosition);
  cmdMessenger.attach(kFlyMode, OnFlyMode); 
  cmdMessenger.attach(kManualControl, OnManualControl);
  cmdMessenger.attach(kTurnAround,OnTurnAround);  
}
void cmdMessengerRun(){
  cmdMessenger.feedinSerialData();
}

void setupCmdMessenger(){
  //setup Command Messenger 
  attachCommandCallbacks();
  Serial.println(F("CmdMessenger begin"));
  cmdMessenger.printLfCr();  
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(kError,F("Command without attached callback"));
}


// Callback function that sets falling
void OnFalling()
{
 
}

void OnHomePosition(){

  GpsCoordinates dest;
//  There is some floating error
//  send:372875280,1270626890,40
//  receive :37.2875289,127.0626831,40.00 >> within 3m
  dest.lat=cmdMessenger.readFloatArg();
  dest.lng=cmdMessenger.readFloatArg();
  dest.alt=cmdMessenger.readFloatArg();
  setDestinationLocation(dest.lat,dest.lng,dest.alt);
  dest=cansatLocation.getDestination();
  Serial.print(dest.lat,7);Serial.print(",");Serial.print(dest.lng,7);Serial.print(",");
  Serial.println(dest.alt);


  cmdMessenger.sendCmd(kAcknowledge,F("Home Position updated"));


}

void OnFlyMode(){
  // Read led state argument, 
  
  int trigger;
  trigger = cmdMessenger.readInt16Arg();
  if(trigger==0){ //automode
    if(isAutoMode()){
       Serial.println(F("Navigation already automode")); 
    }else{
       setNavigationMode(0);
    }    
    cmdMessenger.sendCmd(kAcknowledge,F("Navigation:auto"));

  }
  if(trigger==1){ //manual mode
    if(!isAutoMode()){
       Serial.println(F("Navigation already manualmode")); 
    }else{
       setNavigationMode(1);
    }    
    cmdMessenger.sendCmd(kAcknowledge,F("Navigation:manual"));
  }  
}


void OnManualControl(){
  // Read led state argument, 
  float trigger;
  
  if(isAutoMode()){
    cmdMessenger.sendCmd(kAcknowledge,F("Control fail..Set manualmode first!!!")); 
    return;  
  }

  trigger = cmdMessenger.readInt16Arg();
  winchControl((float)trigger*(float)89.);
  cmdMessenger.sendCmd(kAcknowledge,"manual Contol Ok"); 
  Serial.print("Control angle="); 
  Serial.println((float)trigger*(float)89.); 
  
}
void OnTurnAround(){
  if(cmdMessenger.readInt16Arg()==1){ 
    if(turnaround_toggle){
      turnaround_button=true;      
    } 
  }
}
