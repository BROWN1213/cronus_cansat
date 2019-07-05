/**
* @file 
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2019 Asgardia
* @date June 2019
* @brief ...
*/

#ifndef CANSAT_Serial_H
#define CANSAT_Serial_H



#define RxMode_CAMERA_PORT 0x00  // 0000 0000 = 0
#define RxMode_GPS_PORT    0x01  // 0010 0001 = 1
#define RxMode_IMU_PORT    0x02  // 0100 0020 = 2

#define RxMode_M4_PORT     0x04  
#define RxMode_M5_PORT     0x05  
#define RxMode_M6_PORT     0x06  
#define RxMode_M7_PORT     0x07 

template <class T>
class CansatTransportSerial
{
public:
    CansatTransportSerial(T& stream)
        : stream(stream), conn(0)
    {}

    void begin(uint32_t baud) {
        stream.begin(baud);
    }

    bool connect() {
        CANSAT_LOG("Connecting...");
        return conn = true;
    }
    void disconnect() { conn = false; }

    size_t read(void* buf, size_t len) {
        return stream.readBytes((char*)buf, len);
    }
    size_t write(const void* buf, size_t len) {
        return stream.write((const uint8_t*)buf, len);
    }
    void RxModePortSet( unsigned char iMode)
    {
      unsigned char b = PINC  ;
      b     &= 0xF0  ; // direction is 0
      b     |= iMode ;
      PORTC = b      ;
    }
    bool connected() { return conn; }
    int available() { return stream.available(); }

protected:
    T&     stream;
    bool   conn;
};

template <class T>
class CansatTransportSerialChecked
    : public CansatTransportSerial<T>
{
public:
    CansatTransportSerialChecked(T& stream)
        : CansatTransportSerial<T>(stream)
    {}

    void begin(uint32_t baud) {
        this->stream.begin(baud);
        while (!this->stream) {
            // wait for serial port to connect. Needed for Leonardo only
        }
    }

    int connected() { return this->conn && this->stream; }
};

typedef CansatTransportSerial<HardwareSerial> CansatHwSerial;


#endif