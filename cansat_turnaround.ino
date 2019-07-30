#define HEIGHT_PARAMETER 8
#define TURNHEIGHT_PARAMETER 5
#define DISTANCE_PARAMETER 3
#define MA_NBUF_V 5
#define THRESHOLD_DISTANCE 10
#define ANGLE_MIN 55
#define ANGLE_MAX 105
#define TIME_MIN 5
#define TIME_MAX 10

int velocity_fall_Buf[MA_NBUF_V]={0,};   //init with zero
int ma_offset_v=0;
uint32_t prev_v_fall_time;
float previous_altitude=0,current_altitude=0;
float distance,velocity_air,velocity_fall;
float height,cansat_height,ground_height;
float time_land,estimate_landing_point;
float distance_delta,threshold_height,turn_height;
float turnaround_angle;
bool turnaround_toggle=false;

void updateturnaround(){
  if(turnaround_toggle==false){
    turnaround_time=1;
    distance=cansatLocation.distance;
    velocity_air=cansatGPS.ground_speed_ms();
    current_altitude=cansatGPS.location().alt/100.;
    velocity_fall=averageVelocityFall(current_altitude);  //average
    cansat_height=current_altitude;
    height=cansat_height-ground_height;
    time_land=height/velocity_fall;
    estimate_landing_point=time_land*velocity_air;     //ELP Calculate
    distance_delta=estimate_landing_point-distance;
    Serial.print("distance=");
    Serial.println(distance);
    Serial.print("height=");
    Serial.println(height);
    Serial.print("estimate landing point=");
    Serial.println(estimate_landing_point);
    Serial.print("distance_delta=");
    Serial.println(distance_delta);    
    Serial.println("*********************************");    
    if(distance_delta>THRESHOLD_DISTANCE&&distance!=0){
      turn_height=velocity_fall*TURNHEIGHT_PARAMETER;
      threshold_height=HEIGHT_PARAMETER*turn_height;     //threshold height calculate
      Serial.print("threshold_height=");
      Serial.println(threshold_height);
      if(height>threshold_height){
        turnaround_toggle=true;
        turnaround_angle=getTurnaround_Angle(distance_delta);    //turn around angle calcuete
        turnaround_time=getTurnaround_Time(distance_delta);
        Serial.print("turnaround_time=");
        Serial.println(turnaround_time);
      }else{
        turnaround_toggle=false;
      }
    }else{
      turnaround_toggle=false;
    }
  }else{
    turnaround_toggle=false;
    //cansatNavigation.winchControlTurnaround(turnaround_angle,turnaround_time);
    Serial.print("distance=");
    Serial.println(distance);
    Serial.print("height=");
    Serial.println(height);
    Serial.print("estimate landing point=");
    Serial.println(estimate_landing_point);
    Serial.print("distance_delta=");
    Serial.println(distance_delta);
    Serial.print("threshold_height=");
    Serial.println(threshold_height);
    Serial.print("turnaround_time=");
    Serial.println(turnaround_time);
    Serial.print("turnaround_toggle=");
    Serial.println(turnaround_toggle);
    Serial.println("Start Turn Around!");
    Serial.println("*********************************");
    
  }
}

float getTurnaround_Angle(float distance_delta){
  float Turnaround_angle=map(distance_delta,THRESHOLD_DISTANCE,DISTANCE_PARAMETER*THRESHOLD_DISTANCE,ANGLE_MIN,ANGLE_MAX); 
  return Turnaround_angle;
}

float getTurnaround_Time(float distance_delta){
  float Turnaround_time=map(distance_delta,THRESHOLD_DISTANCE,DISTANCE_PARAMETER*THRESHOLD_DISTANCE,TIME_MIN,TIME_MAX);
  return Turnaround_time;
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
