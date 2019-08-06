import processing.serial.*;
import controlP5.*;



Serial myPort;
DropdownList menu1, menu2;
ControlP5 cp5;
float root;
float avg;

void setupSerial(){
  cp5 = new ControlP5(this);

  menu1 = cp5.addDropdownList("port")
    .setPosition(10,60)
    .setSize(200,500);    

  customize(menu1);
  
  

}

void customize(DropdownList ddl) {
  // a convenience function to customize a DropdownList
  PFont pfont = createFont("Arial",20,true); // use true/false for smooth/no-smooth
  ControlFont font = new ControlFont(pfont,12);
  //ddl.setFont(font);
  ddl.setBackgroundColor(color(190));
  ddl.setItemHeight(15);
  ddl.setBarHeight(17);
  //ddl.captionLabel().set("port");
  ddl.getCaptionLabel().getStyle().marginTop = 3;
  ddl.getCaptionLabel().getStyle().marginLeft = 3;
  ddl.getValueLabel().getStyle().marginTop = 3;
  
  for(int i=0;i<Serial.list().length;i++)
    ddl.addItem(Serial.list()[i], i);
  

   
  //ddl.scroll(0);
  ddl.setColorBackground(color(60));
  ddl.setColorActive(color(255, 128));
  
  
}

void controlEvent(ControlEvent theControlEvent) 
{
  
  if (theControlEvent.isFrom(menu1))
  { int list_num= (int)theControlEvent.controller().getValue();
    String port_name=Serial.list()[list_num];
    println(port_name);
    myPort = new Serial(this, port_name, 115200);
    myPort.bufferUntil('\n');   
    delay(1000);
    String cmd="AT\r\n";
    myPort.write(cmd); 
    delay(1000);
    cmd="ATD\r\n";
    myPort.write(cmd);   
    
  }
  if(theControlEvent.isController())
    cmdcontrolEvent(theControlEvent); // toss to cmdMessege  


}

void reConnect(){
    String cmd="AT\r\n";
    myPort.write(cmd); 
    delay(1000);
    cmd="ATD\r\n";
    myPort.write(cmd);  
    delay(1000);
}


void serialEvent(Serial port) //Reading the datas by Processing.
{
   String input = port.readStringUntil('\n');
   //println(input);
   appendSerialLog(input);
   
  if(input.indexOf("%") == 0){ //header
     input=input.substring(2,input.length()-2); 
     //println(input);
     input = trim(input);
     String[] values = split(input, ",");
     //values[0]: class, values[1]: num, 
      if(int(values[0])==1){ //IMU
      
        roll=float(values[2]);
        pitch=float(values[3]);
        yaw=float(values[4]);
        ax=float(values[5]);
        ay=float(values[6]);
        az=float(values[7]);
        root=float(values[8]);
        avg=float(values[9]);
        
        
        g_xAccel.addVal(float(values[5]));
        g_yAccel.addVal(float(values[6]));
        g_zAccel.addVal(float(values[7]));        
        //print("[");print(millis());print("]");
        //print(roll);print(",");
        //print(pitch);print(",");
        //println(yaw);
        //print("[");print(millis());print("]");
        //print(float(values[5]));print(",");
        //print(float(values[6]));print(",");
        //println(float(values[7]));

        appendImuLog();
      }
      if(int(values[0])==2){ //GPS
        lat=float(values[2]);
        lng=float(values[3]);
        alt=float(values[4]);
        num_sat=float(values[5]);
        speed_ms=float(values[6]);
        course=float(values[7]);
        
        gps_new_messege=true; //
        blink_count=20;
        print("[");print(millis());print("gps");print("]");
        print(lat);print(",");
        print(lng);print(",");
        println(alt);
        appendGpsLog();
        //print(num_sat);print(",");
        //print(speed_km);print(",");
        //println(course);         
      }
     
      if(int(values[0])==3){ //battery
        bat_level=int(values[2]);
        flight_mode=int(values[3]); 
        //print("batt=");println(bat_level);
        isconneted=120;      
      }
      if(int(values[0])==4){ //gps searching
        gps_status=int(values[2]);
        //print("batt=");println(bat_level);    
//#define GPS_FIX_TYPE_NO_GPS 0
//#define GPS_FIX_TYPE_NO_FIX 1
//#define GPS_FIX_TYPE_2D_FIX 2
//#define GPS_FIX_TYPE_3D_FIX 3
//#define GPS_FIX_TYPE_DGPS 4
//#define GPS_FIX_TYPE_RTK_FLOAT 5
//#define GPS_FIX_TYPE_RTK_FLOAT 6
//#define GPS_FIX_TYPE_RTK_FIXED 7
    }

      if(int(values[0])==5){ //location
        distance=float(values[2]);
        bearing=float(values[3]);
        print("[");print(millis());print("loc");print("]");
        print(distance);print(",");
        println(bearing);        
      }    
      if(int(values[0])==6){ //navigation
        control_angle=float(values[2]);
        //ground_alt=float(values[3]); 

        //print("[");print(millis());print("nav");print("]");
        //print(control_angle);print(",");
        //println(ground_alt);        
      } 
      if(int(values[0])==7){  
        densityPM= values[2];
        appendOtherValueLog();
      }
      if(int(values[0])==8){ // turnaround
        isTurnAround=boolean(values[2]);
      }
      if(int(values[0])==9){ // turnaround
        isGetWind=boolean(values[2]);
        isCheckWind=int(values[3]);        
      }
  }else {
    println(input);
  }
   
}
