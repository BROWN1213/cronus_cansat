#include "src/cansat_imu.h"

static CansatHwSerial _IMUport(Serial1);

CansatIMU<CansatHwSerial> cansatIMU(_IMUport);
  
void setupAHRS(){
    cansatIMU.begin(115200);
    cansatStatus=-1;
    Serial.println(F("AHRS begin"));

}
void reconnectAhrs(){
  cansatIMU.begin(115200);
  cansatIMU.rx_empty();    
}

#define PROCESSING
void updateAHRS(){

  if(cansatIMU.read()){ 
#ifdef SERIAL_MONITOR
      Serial.print("roll/pitch/yaw/ax/ay/ax/mx/my/mz");  
      for(int i=0;i<8;i++){    
        Serial.print(cansatIMU.imu_datas[i]);Serial.print(',');
      }
      Serial.println(cansatIMU.imu_datas[8]);
      cansatIMU.getHeading();
      Serial.print("heading: ");
      Serial.println(cansatIMU.heading);
#endif
#ifdef PROCESSING
      Serial.print("%,1,6,");  // header,class,num data
      Serial.print(cansatIMU.imu_datas[0]);Serial.print(',');
      Serial.print(cansatIMU.imu_datas[1]);Serial.print(',');
      Serial.print(cansatIMU.imu_datas[2]);Serial.print(',');
      Serial.print(cansatIMU.imu_datas[3]);Serial.print(',');
      Serial.print(cansatIMU.imu_datas[4]);Serial.print(',');
      Serial.println(cansatIMU.imu_datas[5]);
#endif

  } 

}

void cansatStateM(int status){

  switch(status){
    case SETUP_COMPLETED :
       Serial.println(F("Setup Completed"));
       cansatStatus=READY_FOR_LAUNCH;
       
    break;
    case READY_FOR_LAUNCH :
       Serial.println(F("Ready For Launch"));
       cansatStatus=LAUNCH_DETECTED;
    break;
    case LAUNCH_DETECTED :
      cansatStatus=FREEFALL_DETECTED;
      Serial.println(F("Launch detected"));

    break;
    case FREEFALL_DETECTED :
      cansatStatus=PARACHUTE_DEVELPED;
      Serial.println(F("Freefall detected"));

    break;
     case PARACHUTE_DEVELPED :
      cansatStatus=AUTOPILOT_ACTIVATED;
      Serial.println(F("Parachte developed"));

    break;     
    case AUTOPILOT_ACTIVATED :
      //cansatStatus=LANDING;
      Serial.println(F("Autopilot activated"));

    break;  
    case TURN_AROUND :
     //cansatStatus=AUTOPILOT_ACTIVATED;
      Serial.println(F("Autopilot activated"));

    break;     
    case LANDING :
      Serial.println(F("Landing"));

    break;              
    default:
    break;
  }
  

}

float calMotionAcceleration(){
  float ax=cansatIMU.imu_datas[3];
  float ay=cansatIMU.imu_datas[4];
  float az=cansatIMU.imu_datas[5];
  return pow( (ax*ax+ay*ay+az*az),0.5);
}

#define MA_NBUF 7
int MotionAccBuf[MA_NBUF]; 
int ma_offset=0;
float averageMotionAcc(float motion_acc){

  float v_sum;
  // buffering V_NBUF data
  MotionAccBuf[ma_offset]=motion_acc;
  ma_offset++;
  ma_offset %= MA_NBUF;

 //calculate average 
  v_sum=0;
  for(int i=0;i<MA_NBUF;i++) v_sum += MotionAccBuf[i];
  float avg_v=v_sum/float(MA_NBUF);
  return avg_v;
}
