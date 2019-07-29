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

void CansatCircling::updateDis(float cansatdistance)
{
    distance=cansatdistance;
    return;


}
void CansatCircling::updateVair(float cansatspeed){
    Vair=cansatspeed;
    return;
}
void CansatCircling::updateVfall(float beforeAlt, float afterAlt){
    Vfall=beforeAlt-afterAlt;
    return;
}

