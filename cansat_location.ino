#include "src/cansat_location.h"

CansatLocation cansatLocation;

bool destination_locked;
float location_distance,location_bearing;

void setupLocation(){

    if(cansatLocation.testVincenty()){
        Serial.println(F("vincenty OK"));

    }else{
        Serial.println(F("vincenty error"));
    }
    destination_locked=false;
}

void setDestinationLocation(float lat, float lng, float alt){
  cansatLocation.setDestination(lat,lng,alt);
  destination_locked=true;
}
void setCurrentLocation(){
  float lat = cansatGPS.location().lat* 1.0e-7f;
  float lng = cansatGPS.location().lng* 1.0e-7f;
  float alt = cansatGPS.location().alt/1000.;
  cansatLocation.updateCurrentCoord(lat,lng,alt);
}
bool updateLocation(){
  //set currrnt GPS info
  setCurrentLocation();
  // calculate bearing and distance
  if(cansatLocation.cal_distance_bearing()){
    location_distance=cansatLocation.distance;
    location_bearing=cansatLocation.bearing_angle;
    //decide turn around or autp navigation and calculate wind velocity  
    //send info to base station
    Serial.print("%,5,2,");  // header,class,num data
    Serial.print(location_distance);Serial.print(',');
    Serial.println(location_bearing);
    return true; 
  }else{
    Serial.println(F("updateLocation Fail"));
    return false;
  }

  
}
