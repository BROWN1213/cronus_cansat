import processing.serial.*;
import controlP5.*;


Serial myPort;
DropdownList menu1, menu2;
ControlP5 cp5;

void setupSerial(){
  cp5 = new ControlP5(this);

  menu1 = cp5.addDropdownList("port")
    .setPosition(0.2*width/5, 0.5*height/5)
    .setSize(200,500);    

 customize(menu1);

}

void customize(DropdownList ddl) {
  // a convenience function to customize a DropdownList
  PFont pfont = createFont("Arial",20,true); // use true/false for smooth/no-smooth
  ControlFont font = new ControlFont(pfont,12);
  ddl.setFont(font);
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
    
    String cmd="AT\r\n";
    myPort.write(cmd); 
    delay(1000);
    cmd="ATD\r\n";
    myPort.write(cmd);     
  }

}



void serialEvent(Serial port) //Reading the datas by Processing.
{
   String input = port.readStringUntil('\n');
   //println(input);
   
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
        
        //print(roll);print(",");
        //print(pitch);print(",");
        //println(yaw);
                
      }
      if(int(values[0])==2){ //GPS
        lat=float(values[2]);
        lng=float(values[3]);
        alt=float(values[4]);
        num_sat=float(values[5]);
        speed_km=float(values[6]);
        course=float(values[7]);
        
        gps_new_messege=true; //
        blink_count=20;
        //print(lat);print(",");
        //print(lng);print(",");
        //println(alt);
        
        //print(num_sat);print(",");
        //print(speed_km);print(",");
        //println(course);         
      }
     
      if(int(values[0])==3){ //battery
        bat_level=int(values[2]);
        //print("batt=");println(bat_level);
        isconneted=70;      
      }
      if(int(values[0])==4){ //gps searching
        is_gps_searching=int(values[2])*20;
        //print("batt=");println(bat_level);    
      }

  }
   
}
