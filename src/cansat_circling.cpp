/**
* @file 
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2019 Asgardia
* @date June 2019
* @brief ...
*/


#include "cansat_circling.h"

CansatCircling::CansatCircling()
{

}

float CansatCircling::updatedis(float cansatdistance)
{
    distance=cansatdistance;
    return distance;


}
float CansatCircling::updateVair(float cansatspeed){
    Vair=cansatspeed;
    return Vair;
}
float CansatCircling::updateVfall(float beforeAlt, float afterAlt){
    Vfall=beforeAlt-afterAlt;
    return Vfall;
}

