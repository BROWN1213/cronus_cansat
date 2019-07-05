/**
* @file 
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2019 Asgardia
* @date June 2019
* @brief ...
*/

#ifndef CANSAT_SD_H
#define CANSAT_SD_H


#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>



#define LOG_LABEL "date,time,time_index,mode,longitude,latitude,altitude,numsat,speed,cource,bearing,distance,controlangle"
#define IMU_LABEL "gpstime,systime,roll,pitch,yaw,ax,ay,az,acceleration"
struct LogPacket {
  int32_t date;
  float time;
  uint32_t time_index;
  String mode;
  float latitude;
  float longitude;
  float altitude;
  uint8_t numsat;
  float speed;
  float cource;
  float bearing;
  float distance;
  float controlangle;
};
struct ImuPacket {
  int32_t gpsTime;
  uint32_t systemTime;
  float roll;
  float pitch;
  float yaw;
  float ax;
  float ay;
  float az;
  float motion_acceleration;
};

class CansatSD: SdFat
{
  public:
    CansatSD()
        :SdFat()
    {}
    bool begin(int cs_pin);
    bool setFileName(String sfilename);
    bool setFileName(int32_t date,int32_t times);
    bool saveData(LogPacket *log_packet);
    bool saveDataImu(ImuPacket *imu_packet);
    bool saveDataImu(ImuPacket *array_imu_packet,uint16_t size);
    String logPacketToString(LogPacket *log_packet);
    String imuPacketToString(ImuPacket *ip);
    bool saveLabel(String label);
    bool saveLabelImu(String label);

    File fileOpen(String sfilename);
    void fileClose(File fp);
    //void writeSd(File fp,String sdata_string);
    bool createFile(String sfilename);

  private:
    String _slog_file_name;
    String _sImu_file_name;
//to be going to add:ACC, falling point, parachute opening point
};


#endif