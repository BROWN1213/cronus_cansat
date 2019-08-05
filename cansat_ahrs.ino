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

void freeFallDetection(){
  float motionAcceleration=averageMotionAcc(calMotionAcceleration());
  if (motionAcceleration<0.3){
     Serial.println(F("Freefall detected"));
     schedule_timer.setTimeout(7000, setAutoMode);
  }  
}

void setAutoMode(){
  setNavigationMode(0);
}

float calMotionAcceleration(){
  float ax=cansatIMU.imu_datas[3];
  float ay=cansatIMU.imu_datas[4];
  float az=cansatIMU.imu_datas[5];
  return pow((ax*ax+ay*ay+az*az),0.5);
}     



#define MA_NBUF 3
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
