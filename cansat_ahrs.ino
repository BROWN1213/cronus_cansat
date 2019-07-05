#include "src/cansat_imu.h"

#define READY_S 0
#define LAUNCHING 1
#define FREEFALL 2
#define PARAFOIL_TV 3  //terminal velocity
#define LANDING 4

static CansatHwSerial _IMUport(Serial1);

CansatIMU<CansatHwSerial> cansatIMU(_IMUport);
int flying_status;  // 0:ready 1 : launching, 2: freefall, 3:parafoil   4: landing
void setupAHRS(){
    cansatIMU.begin(115200);
    flying_status=READY_S;

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
      Serial.print("%,1,3,");  // header,class,num data
      Serial.print(cansatIMU.imu_datas[0]);Serial.print(',');
      Serial.print(cansatIMU.imu_datas[1]);Serial.print(',');
      Serial.println(cansatIMU.imu_datas[2]);
#endif

  } 

}

bool detectFlyingStatus(){

}

float calMotionAcceleration(){

  return 1;  //pow( (x*x+y*y+z*z),0.5)
}
