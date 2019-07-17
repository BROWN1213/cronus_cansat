/**
* @file 
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2019 Asgardia
* @date Mar 2019
* @brief ...
*/

#include "cansat_PM2_5.h"
#include "cansat_Debug.h"
#include "Arduino.h"


CansatPM2_5::CansatPM2_5(int Vo, int V_LED){
  _Vo=Vo;
  _V_LED=V_LED;
  dustDensity = 0;
  _last_millis=0;
  _prev_dustDensity=0;
}
void CansatPM2_5::begin(){
  pinMode(_V_LED,OUTPUT);
}
float CansatPM2_5::calulatePM2_5(){
  float Vo_value,voltage;
  long current_millis=millis();
  if((current_millis-_last_millis) < SLEEP_TIME ){
    _last_millis=current_millis;
    digitalWrite(_V_LED,LOW);  //power on LED
    delayMicroseconds(SAMPLING_TIME);

    Vo_value =analogRead(_Vo); //read a dust Vo_value

    delayMicroseconds(DELTA_TIME);
    digitalWrite(_V_LED,HIGH); //turn off LED
    //delayMicroseconds(SLEEP_TIME);

    voltage = Vo_value*(5.0/1024.0);  // 0 - 5V mapped to 0 - 1023 integer values
    // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
    // Chris Nafis (c) 2012
    // dustDensity = 0.172 * voltage - 0.0999;  //mg/m^3
    // 
    //https://m.blog.naver.com/PostView.nhn?blogId=iamamaker&logNo=221126455583&proxyReferer=https%3A%2F%2Fwww.google.com%2F
    //Serial.println(voltage);
    if(voltage<VOC)voltage=VOC; //To avoid minus
    dustDensity = (voltage - VOC)/K_SEN;  //ug/m^3
  
    //smooting filter
    dustDensity=0.9*_prev_dustDensity+0.1*dustDensity;
    _prev_dustDensity=dustDensity;

    return dustDensity;
  }else{
    CANSAT_LOG("PM2.5 update need at least 1second ");
    return(-1);
  }
  _last_millis=current_millis;
}