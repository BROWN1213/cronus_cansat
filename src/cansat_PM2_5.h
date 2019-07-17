/**
* @file 
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2019 Asgardia
* @date Mar 2019
* @brief ...
*/



/*  Sharp Optical Dust Sensor GP2Y1010AU0F

출처: https://bbangpan.tistory.com/51 [빵판닷컴 (bbangpan.com)]

*/

#ifndef CANSAT_PM2_5_H
#define CANSAT_PM2_5_H


#define SAMPLING_TIME 280
#define DELTA_TIME 40
#define SLEEP_TIME 9680
#define VOC 0.8 // 0~1.5 voltage at no dust 
#define K_SEN 0.005 //  0.0035~0.0065 V/(ug/m^3) sensitivity
class CansatPM2_5{
  public:
    CansatPM2_5(int Vo, int V_LED);
    void begin();
    float calulatePM2_5();
    float dustDensity ;
  
  private:
    
    int _Vo;
    int _V_LED;
    long _last_millis;
    float _prev_dustDensity;


};





#endif
