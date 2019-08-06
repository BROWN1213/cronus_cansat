float lat,lng,alt,speed_ms,course,num_sat;
boolean gps_new_messege=false;

PImage img;

boolean on=true;
int gps_status;
float prev_lon,prev_lat,prev_course;
boolean first_map=false;
int blink_count=-1;



PShape satellite;

PShape connect;
int isconneted=-1;

boolean gps_played=false;
boolean connect_played=false,signal_lost=false;
float zoom_v ;
boolean zoom_pressed;
String densityPM="0",windDirection="N",windSpeed="0";

void setupSatellite(){
  satellite=loadShape("satellite.svg");
  zoom_v=15;
  zoom_pressed=false;
  //Create the new GUI


  cp5.addButton("IN")
     //Set the position of the button : (X,Y)
     .setPosition(550,100)
     //Set the size of the button : (X,Y)
     .setSize(30,30)
     //Set the pre-defined Value of the button : (int)
     .setValue(0)
     //set the way it is activated : RELEASE the mouseboutton or PRESS it
     .activateBy(ControlP5.RELEASE);
     ;
  PFont pfont = createFont("Arial",20,true); // use true/false for smooth/no-smooth
  ControlFont font = new ControlFont(pfont,241);
  cp5.getController("IN")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(30)
     .setText("+")
     ;     
  
  cp5.addButton("OUT")
     //Set the position of the button : (X,Y)
     .setPosition(550,140)
     //Set the size of the button : (X,Y)
     .setSize(30,30)
     //Set the pre-defined Value of the button : (int)
     .setValue(0)
     //set the way it is activated : RELEASE the mouseboutton or PRESS it
     .activateBy(ControlP5.RELEASE);
     ;
  cp5.getController("OUT")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(30)
     .setText("-")
     ;      


     
}
public void IN(int value){
  zoom_v+=0.5;
  zoom_pressed=true;
}

public void OUT(int value){
 zoom_v-=0.5;
 zoom_pressed=true;
}


void satellite_info(){
  textAlign(LEFT);
  fill(0,0,0);
  textSize(25);  
  text(str(speed_ms)+"m/s", 390, 40); 

  text(cardinal(course), 510, 40); 
  text(str(int(distance))+"m", 510, 70);   
  text(str(int(ground_alt))+"m", 560, 40); 
  
  //satellite.disableStyle();
  //fill(0, 102, 153);
  
  textAlign(CENTER);
  if(gps_status<=1){
    satellite.disableStyle();
    fill(200, 3, 3);
    shape(satellite, 260, 10, 40, 40); 
    textSize(15);    
    text("Search..", 330, 40); 

  }else{
    satellite.disableStyle();
    fill(0, 102, 255);
    shape(satellite, 260, 10, 40, 40); 
    textSize(25);
    text(str(int(num_sat)), 330, 40); 

  }  

  if( (gps_status>=3) && (gps_played==false)){
    playGPSlocked();
    gps_played=true;
  }
  if(gps_status<2)gps_played=false;
  
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
  
}
void checkConnection(){
   if( (isconneted >0) &&  (connect_played==false) ){
    playConneted();
    connect_played=true;
  }
 
  if( (isconneted ==0) &&  (connect_played==true) &&(signal_lost==false) ){
    playSignalLost();
    signal_lost=true;
  }     
  if( (isconneted ==0) &&  (connect_played==true) &&(signal_lost==true) ){
    signal_lost=true;
    reConnect();
  } 
  if( (isconneted >0) &&  (connect_played==true) &&(signal_lost==true )){
    playSignalRecovered();
    signal_lost=false;
  }   
}
void loadMap(){
  
  //pushMatrix();
  //translate(2.7*width/5, 0.8*height/5);
  String token = "";  //delete token when upload code on github

  
  //cchamchi house
  String longitude = "127.1144943";
  String latitude = "37.384931";
  //
  //String longitude = "127.006744";
  //String latitude = "037.558231";  
  //gohung KARI
 // String latitude = "34.611020";
 // String longitude = "127.207196";   
  
  String course_N = "0";
  if(num_sat>=3){
    longitude=nf(lng,3,7);
    latitude=nf(lat,3,7);
    //course_N=nf(-course,3,2);
    course_N=str(course);
  }

  String zoom = str(zoom_v);
  String pitch = "0";
  //https://www.mapbox.com/api-documentation/#styles
  String style = "outdoors-v11"; // "streets-v11" "outdoors-v11" "dark-v9" "light-v9" "satellite-streets-v10" "navigation-preview-day-v2"
  
  String url;
          //float alt=37.387078;
        //float lng=127.122669; 
  if(destination_locked){
    String target ="url-https%3A%2F%2Fwww.mapbox.com%2Fimg%2Frocket.png("+str(dest_lng)+","+str(dest_lat)+")";
    url="https://api.mapbox.com/styles/v1/mapbox/"+style+"/static/"+target+"/"+longitude+","+latitude+","+zoom+","+course_N+","+pitch+"/600x600?access_token="+token;
    
  }else{
  
    url="https://api.mapbox.com/styles/v1/mapbox/"+style+"/static/"+longitude+","+latitude+","+zoom+","+course_N+","+pitch+"/600x600?access_token="+token;    
  }


  if(abs(prev_lon-lng)>0.0001 || (first_map==false) ||abs(prev_lat-lat)>0.0001||abs(prev_course=course)>10.0||zoom_pressed){
    img =  loadImage(url,"jpg");  
    
    first_map=true;
    zoom_pressed=false;
  }
  image(img,0,0);
  prev_lon=lng;
  prev_lat=lat;
  prev_course=course;
  
  if(blink_count>0) {
    blink_count--;
    fill(203,67,53);
    ellipse(300, 300, 15, 15);

  }
  

  
 // popMatrix();
  
}

String directions[] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};

String cardinal (float course)
{

  int direction = (int)((course + 11.25f) / 22.5f);
  return directions[direction % 16];
}
