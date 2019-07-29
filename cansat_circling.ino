#include "src/cansat_circling.h"

CansatCircling cansatCircling;
float beforealt=0,curalt=0;

void updatecircling(){
  cansatCircling.updateDis(cansatGPS.location().distance);  
  cansatCircling.updateVair(cansatGPS.ground_speed_ms());
  beforealt=curalt;
  curalt=cansatGPS.location().alt/100.;
  cansatCircling.updateVfall(beforealt,curalt);  
}
