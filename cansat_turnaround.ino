float previous_altitude=0,current_altitude=0;
float distance,velocity_air,velocity_fall;
float height,cansat_height,ground_height;
float time_land;
void updateturnaround(){
  distance=cansatLocation.distance;  
  velocity_air=cansatGPS.ground_speed_ms();
  previous_altitude=current_altitude;
  current_altitude=cansatGPS.location().alt/100.;
  velocity_fall=previous_altitude-current_altitude;
  cansat_height=current_altitude;
  height=cansat_height-ground_height;
  
}
