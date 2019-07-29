float previous_altitude=0,current_altitude=0;
float distance,velocity_air,velocity_fall;

void updateturnaround(){
  distance=cansatLocation.distance;  
  velocity_air=cansatGPS.ground_speed_ms();
  previous_altitude=current_altitude;
  current_altitude=cansatGPS.location().alt/100.;
  velocity_fall=previous_altitude-current_altitude;
}
