#include "src/cansat_location.h"


CansatLocation cansatLocation;

void setupLocation(){

    if(cansatLocation.testVincenty()){
        Serial.println("vincenty OK");

    }else{
        Serial.println("check vincenty!!");
    }

}