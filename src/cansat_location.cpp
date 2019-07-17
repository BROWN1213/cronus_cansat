
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file       location.c
 * @author     Dongwon Lee
 * @copyright  Copyright (c) 2018 Dongwon Lee
 * @date       April 2018
 * @brief
 *
*/


/**
* @file 
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2019 Asgardia
* @date June 2019
* @brief ...
*/




#include "cansat_location.h"
#include "cansat_debug.h"

#define     PI 3.14159265358979

float degreesToRadians(float degrees) {
  return degrees *(PI/180.);  // () 가 매우 중요
}
float radiansToDegrees(float radians) {
  return radians *(180./PI);  // () 가 매우 중요
}

CansatLocation::CansatLocation(){

  distance=0;
  bearing_angle=0;
  estimated_lat=0;
  estimated_lon=0;
  estimated_bearing=0;
  _destination_coord.lat=0;
  _destination_coord.lng=0;
  _destination_coord.alt=0;
  _current_coord.alt=0;
  _current_coord.lat=0;
  _current_coord.lng=0;
  destination_locked=false;
}




/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
/* Vincenty Inverse Solution of Geodesics on the Ellipsoid (c) Chris Veness 2002-2010             */
/*                                                                                                */
/* from: Vincenty inverse formula - T Vincenty, "Direct and Inverse Solutions of Geodesics on the */
/*       Ellipsoid with application of nested equations", Survey Review, vol XXII no 176, 1975    */
/*       http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf                                             */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

/**
 * Calculates geodetic distance between two points specified by latitude/longitude using
 * Vincenty inverse formula for ellipsoids
 *
 * @param   {Number} lat1, lon1: first point in decimal degrees
 * @param   {Number} lat2, lon2: second point in decimal degrees
 * @returns (Number} distance in metres between points
 */
bool CansatLocation::cal_distance_bearing(){

  //float lat1=point1[0];float lon1=point1[1];
  //float lat2=point2[0];float lon2=point2[1];
  float lat1=_current_coord.lat;float lon1=_current_coord.lng;
  float lat2=_destination_coord.lat;float lon2=_destination_coord.lng;
  float a = 6378137, b = 6356752.314245,  f = 1/298.257223563;  // WGS-84 ellipsoid params
  float L = degreesToRadians((lon2-lon1));
  float U1 = atan((1-f) * tan(degreesToRadians(lat1)));
  float U2 = atan((1-f) * tan(degreesToRadians(lat2)));
  float sinU1 = sin(U1), cosU1 = cos(U1);
  float sinU2 = sin(U2), cosU2 = cos(U2);
  float cosSqAlpha,cos2SigmaM,sinSigma,sinLambda,cosLambda,cosSigma,sigma ;

  float lambda = L, lambdaP, iterLimit = 100;

  if(!destination_locked){
    CANSAT_LOG("destination NULL")
    return false;
  }
  do {
    sinLambda = sin(lambda);
    cosLambda = cos(lambda);
    sinSigma = sqrt((cosU2*sinLambda) * (cosU2*sinLambda) +
      (cosU1*sinU2-sinU1*cosU2*cosLambda) * (cosU1*sinU2-sinU1*cosU2*cosLambda));
    if (sinSigma==0) return 0;  // co-incident points

    cosSigma = sinU1*sinU2 + cosU1*cosU2*cosLambda;
    sigma = atan2(sinSigma, cosSigma);
    float sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
    cosSqAlpha = 1 - sinAlpha*sinAlpha;
    cos2SigmaM = cosSigma - 2*sinU1*sinU2/cosSqAlpha;
      if (isnan(cos2SigmaM)) cos2SigmaM = 0;  // equatorial line: cosSqAlpha=0 (§6)
    float C = (f/16)*cosSqAlpha*(4+f*(4-3*cosSqAlpha));
    lambdaP = lambda;
    lambda = L + (1-C) * f * sinAlpha *
      (sigma + C*sinSigma*(cos2SigmaM+C*cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)));
  } while ((fabs(lambda-lambdaP) > 1e-12) && (--iterLimit > 0));

  if (iterLimit==0) return false;  // formula failed to converge

  float uSq = cosSqAlpha * (a*a - b*b) / (b*b);
  float A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
  float B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));
  float deltaSigma = B*sinSigma*(cos2SigmaM+B/4*(cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)-
    B/6*cos2SigmaM*(-3+4*sinSigma*sinSigma)*(-3+4*cos2SigmaM*cos2SigmaM)));
  float s = b*A*(sigma-deltaSigma);

  //s = s.toFixed(3); // round to 1mm precision
  //results[0]= s;
  distance=s;

  // note: to return initial/final bearings in addition to distance, use something like:
  float fwdAz = atan2(cosU2*sinLambda,  cosU1*sinU2-sinU1*cosU2*cosLambda);
  float revAz = atan2(cosU1*sinLambda, -sinU1*cosU2+cosU1*sinU2*cosLambda);
  //results[1]=radiansToDegrees(fwdAz);
  bearing_angle=radiansToDegrees(fwdAz);
  if(bearing_angle<0)bearing_angle+=360.;

  //results[2]=radiansToDegrees(revAz);
  //if(results[2]<0)results[2]+=360.;
  //return { distance: s, initialBearing: fwdAz.toDeg(), finalBearing: revAz.toDeg() };
  return true;
}

/**************************************************************************
 * Module: vincenty (direct).
 *
 * Calculate WGS 84 destination given starting lat/long (degrees),
 * bearing (degrees) & distance (Meters).
 *
 * from: Vincenty direct formula - T Vincenty, "Direct and Inverse
 * Solutions of Geodesics on the Ellipsoid with application of
 * nested equations", Survey Review, vol XXII no 176, 1975
 *       http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf
 *
 * Ported from web java script implementation.  Code standard is a bit
 * odd, but it's efficient and closely parallels the alg equations.
 *
 * Doug Summers    Nov 2010
 *
 ************************************************************/
void CansatLocation::cal_destination(float point1[],float bearing, float dist){
//double lat1, double lon1, double bearing, double dist,
//                  double *lat2out, double *lon2out)
  // WGS-84 ellipsiod
  float a=6378137.0, b=6356752.3142, f=1/298.257223563;
  float alpha1,sinAlpha, sinAlpha1, cosAlpha1, cosSqAlpha;
  float sigma, sigma1, cos2SigmaM, sinSigma, cosSigma, deltaSigma, sigmaP;
  float tanU1, cosU1, sinU1, uSq;
  float A, B, C, L, lambda;
  float tmp, lat2;
  float revAz;   /* unused but retained for alg completeness */
  float lat1=point1[0],lon1=point1[1];

  /* code body */

  alpha1 = degreesToRadians(bearing);
  sinAlpha1 = sin(alpha1);
  cosAlpha1 = cos(alpha1);

  tanU1 = (1-f) * tan(degreesToRadians(lat1));
  cosU1 = 1 / sqrt((1 + tanU1*tanU1));
  sinU1 = tanU1*cosU1;
  sigma1 = atan2(tanU1, cosAlpha1);
  sinAlpha = cosU1 * sinAlpha1;
  cosSqAlpha = 1 - sinAlpha*sinAlpha;
  uSq = cosSqAlpha * (a*a - b*b) / (b*b);
  A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
  B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));

  sigma = dist / (b*A);
  sigmaP = 2*PI;
  while (fabs(sigma-sigmaP) > 1e-12) {
    cos2SigmaM = cos(2*sigma1 + sigma);
    sinSigma = sin(sigma);
    cosSigma = cos(sigma);
    deltaSigma = B*sinSigma*(cos2SigmaM+B/4*(cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)-
                 B/6*cos2SigmaM*(-3+4*sinSigma*sinSigma)*(-3+4*cos2SigmaM*cos2SigmaM)));
    sigmaP = sigma;
    sigma = dist / (b*A) + deltaSigma;
  }

  tmp = sinU1*sinSigma - cosU1*cosSigma*cosAlpha1;
  lat2 = atan2(sinU1*cosSigma + cosU1*sinSigma*cosAlpha1,
      (1-f)*sqrt(sinAlpha*sinAlpha + tmp*tmp));
  lambda = atan2(sinSigma*sinAlpha1,
                 cosU1*cosSigma - sinU1*sinSigma*cosAlpha1);
  C = f/16*cosSqAlpha*(4+f*(4-3*cosSqAlpha));
  L = lambda - (1-C)*f*sinAlpha*(sigma+C*sinSigma*(cos2SigmaM+C*cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)));

  // final bearing
   //dest[2] = atan2(sinAlpha, -tmp);
   estimated_bearing = atan2(sinAlpha, -tmp);

  // algorithm convention uses Deg outputs
  //dest[0] = radiansToDegrees(lat2) ;
  estimated_lat = radiansToDegrees(lat2) ;
  //dest[1] = lon1+(radiansToDegrees(L));
  estimated_lon = lon1+(radiansToDegrees(L));
}





/*
    point[lat,lng] @dd,ddddddd
    result[0]=distance in meters
    result[1]=inital bearing (deg)
    result[2]=final bearing (deg)
*/






void CansatLocation::setDestination(float lat, float lng, float alt)
{
  _destination_coord.alt=alt;
  _destination_coord.lat=lat;
  _destination_coord.lng=lng;
  destination_locked=true;
}
GpsCoordinates CansatLocation::getDestination()
{
   GpsCoordinates coord;
   coord.alt=_destination_coord.alt;
   coord.lat=_destination_coord.lat;
   coord.lng=_destination_coord.lng;
   return(coord);

}

void CansatLocation::updateCurrentCoord(float lat, float lng, float alt)
{
  _current_coord.alt=alt;
  _current_coord.lat=lat;
  _current_coord.lng=lng;
}

bool CansatLocation::testVincenty()
{
  float dest_lat=37.287413;
  float dest_lng=127.062302;
  float curr_lat=37.359498;
  float curr_lng=127.105321;
  //answer is  dist(m)=8862.51  bearing=205.50
  setDestination(dest_lat,dest_lng,0);
  updateCurrentCoord(curr_lat,curr_lng,0);
  cal_distance_bearing();
  //Serial.println(abs(_distance-8862.51));
  if( (abs(distance-8862.51)<0.01) ){
    return true;
  }
}

float CansatLocation::getGroundAltitude(){

  return(_current_coord.lat-_destination_coord.alt);
}