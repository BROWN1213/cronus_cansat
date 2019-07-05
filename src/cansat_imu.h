/**
* @file 
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2019 Asgardia
* @date June 2019
* @brief ...
*/

#ifndef CANSAT_IMU_H
 #define CANSAT_IMU_H

#include "cansat_Serial.h"


#define IMU_DATA_SIZE 64
template <class T>
class CansatIMU
    : public CansatTransportSerial<T>
{
public:
    CansatIMU(T& stream)
        : CansatTransportSerial<T>(stream)
    {}

    void begin(uint32_t baud) {
        this->stream.begin(baud);
        buf_offset=0;
        _data_start=false;
    }
    void begin() {
        this->stream.begin(115200);
        buf_offset=0;
        _data_start=false;
    }
    bool read(){


        char *addr;
        char c;
        bool valid_data=false;
        this->stream.RxModePortSet(RxMode_IMU_PORT);

        int16_t rbytes=this->stream.available();
        while(rbytes--){
            this->stream.read((_imu_data+buf_offset),1);
            if(_imu_data[buf_offset]=='\n'){ //LF end of string
                if (_data_start){
                addr=strtok(_imu_data,",");
                for(int i=0;i<9;i++){    //caution ... be sure to send 9 IMU data
                    imu_datas[i]=atof(addr);
                    addr=strtok(NULL,",");
                }
                _data_start=false;
                valid_data=true;
                }
            }else if(_imu_data[buf_offset]=='*'){ //start of string
                buf_offset=-1;
                _data_start=true;
            }
            // ordinary characters
            buf_offset++;
        if(buf_offset>=IMU_DATA_SIZE)buf_offset=0;
        }


        return valid_data;

    }


    void rx_empty(void)
    {
        char a;
        while(this->stream.available() > 0) {
            this->stream.read(&a,1);
        }
    }
    int connected() { return this->conn && this->stream; }
    int available() { return this->stream.available(); }

    void getHeading(void)
    {
        heading=180*atan2(imu_datas[7],imu_datas[6])/PI;
        if(heading <0) heading +=360;
    }

    void getTiltHeading(void)
    {
        float pitch = imu_datas[1];
        float roll = imu_datas[0];
        float Mxyz[3]={imu_datas[6],imu_datas[7],imu_datas[8]};

        

        float xh = Mxyz[0] * cos(pitch) + Mxyz[2] * sin(pitch);
        float yh = Mxyz[0] * sin(roll) * sin(pitch) + Mxyz[1] * cos(roll) - Mxyz[2] * sin(roll) * cos(pitch);
        float zh = -Mxyz[0] * cos(roll) * sin(pitch) + Mxyz[1] * sin(roll) + Mxyz[2] * cos(roll) * cos(pitch);
        tiltheading = 180 * atan2(yh, xh)/PI;
        if(yh<0)    tiltheading +=360;
    }

    float imu_datas[9]; //[roll,pirch,yaw,x_acc,y_acc_z_acc,magx,magy,magz]
    float heading;
    float tiltheading;
private:

    void parsingAccelgyro(float *buf){
        char *tokenFrame = NULL;
        char *token =NULL;


        tokenFrame = strtok(_imu_data, "*");
        tokenFrame = strtok(NULL, "*"); //skip first frame data
        int i=0;
        for (token = strtok(tokenFrame, ","); token != NULL; token = strtok(NULL, ",")){

            char *unconverted;
            buf[i]= strtod(token, &unconverted);
            i++;
        }
    }

    char	_imu_data[IMU_DATA_SIZE];
    size_t	_buff_size=IMU_DATA_SIZE;
    uint8_t buf_offset;
    bool _data_start;
};



typedef CansatTransportSerial<HardwareSerial> CansatHwSerial;



#endif