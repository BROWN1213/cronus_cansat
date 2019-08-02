#define HEIGHT_PARAMETER 3
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
float height,cansat_height;
float time_land,estimate_landing_point;
float distance_delta,threshold_height,turn_height;
float turnaround_angle,turnaround_time;
bool turnaround_toggle=false;
bool turnaround_button=false;
int cnt111=0;
void updateturnaround(){
  /*
     * distance=cansatLocation.distance;                 
    velocity_air=cansatGPS.ground_speed_ms();
    current_altitude=cansatGPS.location().alt/1000.;
    velocity_fall=averageVelocityFall(current_altitude);  //average
    height=cansatLocation.getGroundAltitude();
    */
    height=40;
    distance=180;
    velocity_air=10;
    velocity_fall=2;
    time_land=height/velocity_fall;
    estimate_landing_point=time_land*velocity_air;     //ELP Calculate
    distance_delta=estimate_landing_point-distance;    //200-180  ==> 20
    Serial.print("Distance=");
    Serial.println(distance);
    Serial.print("velocity_air=");
    Serial.println(velocity_air);
    Serial.print("velocity_fall=");
    Serial.println(velocity_fall);
    Serial.print("current_altitude=");
    Serial.println(current_altitude);
    Serial.print("height=");
    Serial.println(height);
    Serial.print("estimate_landing_point=");
    Serial.println(estimate_landing_point);
    Serial.print("distance_delta=");
    Serial.println(distance_delta);
    Serial.print("ground_alt=");
    Serial.println(ground_alt);
    if(distance_delta>THRESHOLD_DISTANCE){
      turn_height=velocity_fall*TURNHEIGHT_PARAMETER;
      threshold_height=HEIGHT_PARAMETER*turn_height;    //threshold height calculate  30
      Serial.print("threshold_height=");
      Serial.println(threshold_height);
      if(height>threshold_height){
        turnaround_angle=getTurnaround_Angle(distance_delta);    //turn around angle calcuete
        turnaround_time=getTurnaround_Time(distance_delta);
        if(turnaround_angle<DISTANCE_PARAMETER*THRESHOLD_DISTANCE&&THRESHOLD_DISTANCE<turnaround_angle&&turnaround_time<DISTANCE_PARAMETER*THRESHOLD_DISTANCE&&THRESHOLD_DISTANCE<turnaround_time){
          Serial.print("turnaround_angle=");
          Serial.println(turnaround_angle);
          Serial.print("turnaround_time=");
          Serial.println(turnaround_time);
          turnaround_toggle=true;
          /*
          cnt111++;
          Serial.print("cnt111=");
          Serial.println(cnt111);
          if(cnt111==10){
            turnaround_button=true;
          }
          */
        }
      }else{
        turnaround_toggle=false;
      }
    }else{
      turnaround_toggle=false;
    }
  /*if(isGpsLocked()){
    
  }*/
}
void turnaround(){
  if(turnaround_toggle==true){
      if(turnaround_button==true){
        cansatNavigation.winchControlTurnAround(turnaround_angle,turnaround_time*1000); //5000ms = 5s
        Serial.println("Cansat Turn Around!");
        turnaround_toggle=false;
        turnaround_button=false;
        turnaround_count=true;
      }
    }
  /*if(isGpsLocked()){
    
  }*/
}
float getTurnaround_Angle(float distance_delta){
  float Turnaround_angle=map(distance_delta,THRESHOLD_DISTANCE,DISTANCE_PARAMETER*THRESHOLD_DISTANCE,ANGLE_MAX,ANGLE_MIN);  //d delta , 10, 30, 55 ,105
  return Turnaround_angle;
}

float getTurnaround_Time(float distance_delta){
  float Turnaround_time=map(distance_delta,THRESHOLD_DISTANCE,DISTANCE_PARAMETER*THRESHOLD_DISTANCE,TIME_MIN,TIME_MAX);       // 10 30 5 10
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
