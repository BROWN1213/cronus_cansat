#include "src/cansat_sd.h"

CansatSD cansatSD;
uint32_t log_time_index;
ImuPacket array_imu_datas[10];

//void appendImuData(ImuPacket* array_imu_datas,uint16_t offset){
//  ImuPacket imu_datas; 
//
//  
//  imu_datas.gpsTime=cansatGPS.time();
//  imu_datas.systemTime=millis();
//  imu_datas.roll=cansatIMU.imu_datas[0];
//  imu_datas.pitch=cansatIMU.imu_datas[1];
//  imu_datas.yaw=cansatIMU.imu_datas[2];
//  imu_datas.ax=cansatIMU.imu_datas[3];
//  imu_datas.ay=cansatIMU.imu_datas[4];
//  imu_datas.az=cansatIMU.imu_datas[5];
//  imu_datas.motion_acceleration=calMotionAcceleration();
//  *(array_imu_datas+offset)=imu_datas;
//  
//}

void setupSD(){
    cansatSD.begin(SS);
    log_time_index=0;

    cansatSD.setFileName(cansatGPS.date(),cansatGPS.time());
    if(!(cansatSD.saveLabel(LOG_LABEL)&cansatSD.saveLabelImu(IMU_LABEL))){
        Serial.println(F("!!SD problem"));
        LEDBlinkAll(2000);
    }    
}

bool saveGpsControlData(){
  //takes 25ms

  LogPacket log_datas;  
  Location gpsLocation=cansatGPS.location();
  
  log_datas.date=cansatGPS.date();
  log_datas.time=cansatGPS.time();
  log_datas.time_index=log_time_index++;
  log_datas.mode=isAutoMode();
  log_datas.latitude=gpsLocation.lat;
  log_datas.longitude=gpsLocation.lng;
  log_datas.altitude=gpsLocation.alt;
  log_datas.numsat=cansatGPS.num_sats();
  log_datas.speed=cansatGPS.ground_speed_ms();
  log_datas.cource= cansatGPS.ground_course();
  log_datas.bearing=cansatLocation.bearing_angle;
  log_datas.distance=cansatLocation.distance;
  log_datas.controlangle=cansatNavigation.getControlAngle();
  return(cansatSD.saveData(&log_datas)); 

}
