#ifndef CANSAT_VINCENTY_H
#define CANSAT_VINCENTY_H

#include <Arduino.h>



struct GpsCoordinates {
float alt;                   //Altitude in meters
float lat;                   //Latitude in hddd.ddddddd
float lng;                   //Longitude in hddd.ddddddd
};

class CansatLocation{

    public:
    CansatLocation();
    bool cal_distance_bearing();
    void cal_destination(float point1[],float bearing, float dist);
    void setDestination(float lat, float lng, float alt);
    GpsCoordinates getDestination();
    void updateCurrentCoord(float lat, float lng, float alt);
    bool testVincenty();
    float getGroundAltitude();

    float distance;
    float bearing_angle;
    float estimated_lat,estimated_lon,estimated_bearing;
    bool destination_locked;

    private:
    GpsCoordinates _destination_coord;
    GpsCoordinates _current_coord;


};




#endif