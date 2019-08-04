#define HEIGHT_PARAMETER 3
#define DISTANCE_PARAMETER 3
#define MA_NBUF_V 5
#define TH_DISTANCE_MIN 10
#define TH_DISTANCE_MAX 30
#define ANGLE_FOR_MIN_TURN 55  //Fix to 55 angle for 1 minimum turn
#define ANGLE_FOR_MAX_TURN 105 //Fix to 105 angle for 1 maximum turn
#define TIME_FOR_MIN_TURN 5    //Fix to 5 second for 1 minimum turn
#define TIME_FOR_MAX_TURN 10   //Fix to 10 second for 1 maximum turn

int velocity_fall_Buf[MA_NBUF_V]={0,};   //init with zero
int ma_offset_v=0;
uint32_t prev_v_fall_time;
float distance,velocity_air,velocity_fall;
float height,cansat_height;
float time_land,estimate_landing_point;
float distance_delta,th_height,turn_height;
float turnaround_angle,turnaround_time;

void updateturnaround(){
  distance=cansatLocation.distance;                 
  velocity_air=cansatGPS.ground_speed_ms();
  height=cansatLocation.getGroundAltitude();
  velocity_fall=averageVelocityFall(height);  //Calculate Average Value
  
  ////////////// test code
  //height=40;
  //distance=180;
  //velocity_air=10;
  //velocity_fall=2;
  
  ////////////// test code  ELP=20,TH_height=30, turning_time=7
  
  time_land=height/velocity_fall;                    //Time_to_land Calculate
  estimate_landing_point=time_land*velocity_air;     //ELP Calculate
  distance_delta=estimate_landing_point-distance;    //d_delta Calculate
  Serial.print("%,8,1,");
  Serial.println(is_turnaround_started);
  Serial.print("Distance=");
  Serial.println(distance);
  Serial.print("velocity_air=");
  Serial.println(velocity_air);
  Serial.print("velocity_fall=");
  Serial.println(velocity_fall);
  Serial.print("height=");
  Serial.println(height);
  Serial.print("estimate_landing_point=");
  Serial.println(estimate_landing_point);
  Serial.print("distance_delta=");
  Serial.println(distance_delta);
  
  if(distance_delta>TH_DISTANCE_MIN){
    turn_height=velocity_fall*TIME_FOR_MIN_TURN;        //Height for 1 turn Calculate
    th_height=HEIGHT_PARAMETER*turn_height;             //Minimum Height to Turnaround Calculate
    Serial.print("threshold_height=");
    Serial.println(th_height);
    if(height>th_height){
      distance_delta=constrain(distance_delta,TH_DISTANCE_MIN,TH_DISTANCE_MAX);   
      turnaround_angle=getTurnaround_Angle(distance_delta);    //Turnaround Angle Calculate
      turnaround_time=getTurnaround_Time(distance_delta);      //Turnaround Time Calculate
      Serial.print("turnaround_angle=");
      Serial.println(turnaround_angle);
      Serial.print("turnaround_time=");
      Serial.println(turnaround_time);
      
      if(is_turnaround_started==false){
        if(turnaround_permission==true){
          cansatNavigation.winchNeutral();      //Move Winch Servo to Neutral
          cansatNavigation.winchControlTurnAround(turnaround_angle,turnaround_time*1000);  //Move Winch Servo to Turnaround(angle,ms)
          schedule_timer.setTimeout(turnaround_time*1000+1000,turnaroundtimeout);          //After Turnaround Time change is_turnaround_started to false
          Serial.println("Cansat Turn Around!");
          is_turnaround_started=true;
        }
      }
    }
  }
}

void turnaroundtimeout(){
  Serial.println("Cansat Turnaround end");
  is_turnaround_started=false;
}

float getTurnaround_Angle(float distance_delta){
  float Turnaround_angle=map(distance_delta,TH_DISTANCE_MIN,TH_DISTANCE_MAX,ANGLE_FOR_MAX_TURN,ANGLE_FOR_MIN_TURN);  //d_delta, 10, 30, 105,55
  return Turnaround_angle;
}

float getTurnaround_Time(float distance_delta){
  float Turnaround_time=map(distance_delta,TH_DISTANCE_MIN,TH_DISTANCE_MAX,TIME_FOR_MIN_TURN,TIME_FOR_MAX_TURN);     //d_delta, 10, 30, 5,10
  return Turnaround_time;
}

float averageVelocityFall(float current_altitude){
  float v_sum;
  float velocity_fall;
  float previous_altitude=0;
  uint32_t delta_time,t_now;
  t_now=millis();
  delta_time=t_now-prev_v_fall_time;
  prev_v_fall_time=t_now;       //update prev_v_fall_time
  if(prev_v_fall_time>0.001){
    velocity_fall=(previous_altitude-current_altitude)/delta_time*1000; // m/s
    previous_altitude=current_altitude;  //update prev_height
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
}
