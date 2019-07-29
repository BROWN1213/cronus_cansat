#define HEIGHT_PARAMETER 8
#define MA_NBUF_V 5

int velocity_fall_Buf[MA_NBUF_V]={0,};   //init with zero
int ma_offset_v=0;
uint32_t prev_v_fall_time;
float previous_altitude=0,current_altitude=0;
float distance,velocity_air,velocity_fall;
float height,cansat_height,ground_height;
float time_land,estimate_landing_point;
float distance_delta,threshold_distance,threshold_height,turn_height;
float turnaround_angle;

void updateturnaround(){
  distance=cansatLocation.distance;  
  velocity_air=cansatGPS.ground_speed_ms();
  current_altitude=cansatGPS.location().alt/100.;
  
  averageVelocityFall(current_altitude);  //average
  
  cansat_height=current_altitude;
  height=cansat_height-ground_height;
  time_land=height/velocity_fall;
  estimate_landing_point=time_land*velocity_air;
  distance_delta=estimate_landing_point-distance;
  turn_height=velocity_fall*5;
  threshold_height=HEIGHT_PARAMETER*turn_height;
  if(distance_delta>=20&&height>threshold_height){
    turnaround_angle=map(distance_delta,threshold_distance,3*threshold_distance,55,105);    
                   //선회 함
  }else{
                   //선회 안함 
  }
}

float averageVelocityFall(float current_altitude){
    float v_sum;
    float velocity_fall;
    uint32_t delta_time,t_now;
    t_now=millis();
    delta_time=t_now-prev_v_fall_time;
    prev_v_fall_time=t_now;
    previous_altitude=current_altitude;  //update prev_height
    velocity_fall=(previous_altitude-current_altitude)/delta_time*1000; // m/s
    // buffering V_NBUF data
    velocity_fall_Buf[ma_offset_v]=velocity_fall;
    ma_offset_v++;
    ma_offset_v %= MA_NBUF_V;
    //calculate average 
    v_sum=0;
    for(int i=0;i<MA_NBUF_V;i++) v_sum += velocity_fall_Buf[i];
    float avg_v=v_sum/float(MA_NBUF_V);
    return avg_v;
}
