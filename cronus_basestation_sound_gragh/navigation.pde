float distance,bearing,control_angle,ground_alt;
int flight_mode;
int playApproach_cnt;
void setupNav(){
  distance=0;
  bearing=0;
  control_angle=0;
  ground_alt=0;
  flight_mode=1;
  playApproach_cnt=0;
}

void drawNav(){
  float length = 100;
  float x1=length*sin(radians(control_angle));
  float y1=-length*cos(radians(control_angle));
  
  strokeWeight(1.5);
  stroke(100, 100, 100);
  if( gps_status>=3) drawArrow(300,300,300+x1,300+y1,0,4,true);
  
  if(distance < 200 && playApproach_cnt==0 && destination_locked && (gps_status>=3)){
    playApproach();
    playApproach_cnt=200;
    //if(distance>70)playApproach_cnt=400;
    //else if (distance>40)playApproach_cnt=200;
    //else if(distance>10)playApproach_cnt=100;
    //else playApproach_cnt=100;
  }
  if(playApproach_cnt>0)playApproach_cnt--;
}

void drawArrow(float x0, float y0, float x1, float y1, float beginHeadSize, float endHeadSize, boolean filled) {

  PVector d = new PVector(x1 - x0, y1 - y0);
  d.normalize();
  
  float coeff = 1.5;
  
  strokeCap(SQUARE);
  
  line(x0+d.x*beginHeadSize*coeff/(filled?1.0f:1.75f), 
        y0+d.y*beginHeadSize*coeff/(filled?1.0f:1.75f), 
        x1-d.x*endHeadSize*coeff/(filled?1.0f:1.75f), 
        y1-d.y*endHeadSize*coeff/(filled?1.0f:1.75f));
  
  float angle = atan2(d.y, d.x);
  
  if (filled) {
    // begin head
    pushMatrix();
    translate(x0, y0);
    rotate(angle+PI);
    triangle(-beginHeadSize*coeff, -beginHeadSize, 
             -beginHeadSize*coeff, beginHeadSize, 
             0, 0);
    popMatrix();
    // end head
    pushMatrix();
    translate(x1, y1);
    rotate(angle);
    triangle(-endHeadSize*coeff, -endHeadSize, 
             -endHeadSize*coeff, endHeadSize, 
             0, 0);
    popMatrix();
  } 
  else {
    // begin head
    pushMatrix();
    translate(x0, y0);
    rotate(angle+PI);
    strokeCap(ROUND);
    line(-beginHeadSize*coeff, -beginHeadSize, 0, 0);
    line(-beginHeadSize*coeff, beginHeadSize, 0, 0);
    popMatrix();
    // end head
    pushMatrix();
    translate(x1, y1);
    rotate(angle);
    strokeCap(ROUND);
    line(-endHeadSize*coeff, -endHeadSize, 0, 0);
    line(-endHeadSize*coeff, endHeadSize, 0, 0);
    popMatrix();
  }
}
