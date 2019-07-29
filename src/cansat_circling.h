/**
* @file 
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2019 Asgardia
* @date June 2019
* @brief ...
*/


#ifndef CANSAT_CIRCLING_H
#define CANSAT_CIRCLING_H

#include "Arduino.h"

class CansatCircling
{
  public:
    CansatCircling();
    float updatedis(float cansatdistance);
    float updateVair(float cansatspeed);
    float updateVfall(float beforeAlt, float afterAlt);
    float distance;
    float Vfall; //1초전 높이-현재 낙하거리
    float height;  //현재 높이-지면 높이
    float Vair;
    float ELP;   
  private:

};


#endif