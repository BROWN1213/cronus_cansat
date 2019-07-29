#include "src/cansat_circling.h"

CansatCircling cansatCircling;

float beforealt=0,curalt=0;
float dis,Vair,Vfall;

void updatecircling(){
  dis=cansatCircling.updatedis(cansatGPS.location().distance);  
  Vair=cansatCircling.updateVair(cansatGPS.ground_speed_ms());
  beforealt=curalt;
  curalt=cansatGPS.location().alt/100.;
  Vfall=cansatCircling.updateVfall(beforealt,curalt);  
}
