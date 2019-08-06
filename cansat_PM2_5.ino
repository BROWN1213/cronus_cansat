#include "src/cansat_PM2_5.h"

#define PM2_5_pin 4   //AT4 pin

CansatPM2_5 cansatPM2_5(PM2_5_pin,D1);  //GP2Y10 dust sensor

void setupPM2_5(){
  cansatPM2_5.begin();
  Serial.println(F("PM2.5 begin"));
}

void updatePM2_5(){
  //PM 2.5
  cansatPM2_5.calulatePM2_5();

    //processing
    Serial.print("%,7,1,");  //header
    Serial.println((float)cansatPM2_5.dustDensity);
}
