float lat,lng,alt,speed_km,course,num_sat;
boolean gps_new_messege=false;

PImage img;

boolean on=true;
int is_gps_searching=-1;
float prev_lon,prev_lat,prev_course;
boolean first_map=false;
int blink_count=-1;

PShape satellite;

PShape connect;
int isconneted=-1;

void setupSatellite(){
  satellite=loadShape("satellite.svg");
}

void satellite_info(){
  textAlign(CENTER);
  fill(0,0,0);
  textSize(25);  
  text(str(int(speed_km))+"km/h", 500, 40); 


  
  //satellite.disableStyle();
  //fill(0, 102, 153);
  
  
  if(is_gps_searching>0){
    satellite.disableStyle();
    fill(200, 3, 3);
    shape(satellite, 280, 10, 40, 40); 
    textSize(15);    
    text("Searching", 370, 40); 

  }else{
    satellite.disableStyle();
    fill(0, 102, 255);
    shape(satellite, 280, 10, 40, 40); 
    textSize(25);
    text(str(int(num_sat)), 350, 40); 

  }  

  
  
  if(isconneted>0){
    connect.disableStyle();
    fill(0, 102, 255);
    shape(connect, 140, 20, 35, 25);
  }else{
    connect.disableStyle();
    fill(200, 3, 3);
    shape(connect, 140, 20, 35, 25);  
  }
  
  if(isconneted>0)isconneted--;
  if(is_gps_searching>0)is_gps_searching--;  
}

void loadMap(){
  
  //pushMatrix();
  //translate(2.7*width/5, 0.8*height/5);
  String token = "pk.eyJ1IjoiY2NoYW1jaGkiLCJhIjoiY2plNDM0ZW8xMXo3NTJ3anI5NW81Y2xxZyJ9.QLsDM8E8nyuK2GMfGuUFDw";
  //String longitude = "127.1144943 ";
  //String latitude = "37.384931";
  String longitude = "127.006744";
  String latitude = "037.558231";  
  
  String course_N = "0";
  if(num_sat>=3){
    longitude=nf(lng,3,7);
    latitude=nf(lat,3,7);
    //course_N=nf(-course,3,2);
    course_N=str(course);
  }

  String zoom = "15.2";
  String pitch = "0";
  //https://www.mapbox.com/api-documentation/#styles
  String style = "streets-v10"; // "streets-v10" "dark-v9" "light-v9" "satellite-streets-v10" "navigation-preview-day-v2"
  
  String url;

  
  url="https://api.mapbox.com/styles/v1/mapbox/"+style+"/static/"+longitude+","+latitude+","+zoom+","+course_N+","+pitch+"/600x600?access_token="+token;
  if(abs(prev_lon-lng)>0.0001 || (first_map==false) ||abs(prev_lat-lat)>0.0001||abs(prev_course=course)>10.0){
    img =  loadImage(url,"jpg");  
    
    first_map=true;
  }
  image(img,0,0);
  prev_lon=lng;
  prev_lat=lat;
  prev_course=course;
  
  if(blink_count>0) {
    blink_count--;
    fill(203,67,53);
    ellipse(300, 300, 20, 20);

  }
  

  
 // popMatrix();
  
}
