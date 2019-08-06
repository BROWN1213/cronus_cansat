int kError;
String allowturn="allowturn";
boolean cmdFirst;
boolean destination_locked;
float dest_lat,dest_lng,dest_alt;
void setupCmdMessege(){
  PFont pfont = createFont("Arial",20,true); // use true/false for smooth/no-smooth
  ControlFont font = new ControlFont(pfont,241);  
  cmdFirst=false;
  destination_locked=false;
  cp5.addButton("right")
     .setPosition(500,550)
     .setSize(30,30)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
     ;

  cp5.getController("right")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(30)
     .setText(">")
     ;     
  
  cp5.addButton("right2")
     .setPosition(535,550)
     .setSize(40,30)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
     ;

  cp5.getController("right2")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(30)
     .setText(">>")
     ;   
  
  
  cp5.addButton("neutral")
     .setPosition(465,550)
     .setSize(30,30)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
     ;

  cp5.getController("neutral")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(30)
     .setText("O")
     ;    
 
   cp5.addButton("left")
     .setPosition(430,550)
     .setSize(30,30)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
     ;

  cp5.getController("left")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(30)
     .setText("<")
     ;     
  
  cp5.addButton("left2")
     .setPosition(385,550)
     .setSize(40,30)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
     ;

  cp5.getController("left2")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(30)
     .setText("<<")
     ;   
 
  cp5.addButton("auto")
     .setPosition(340,550)
     .setSize(40,30)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
     ;

  cp5.getController("auto")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(14)
     .setText("AUTO")
     ;   
    
  cp5.addButton("manual")
     .setPosition(295,550)
     .setSize(40,30)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
     ;

  cp5.getController("manual")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(14)
     .setText("MANU")
     ;   

  cp5.addButton("setDestination")
     .setPosition(10,550)
     .setSize(45,30)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
     ;

  cp5.getController("setDestination")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(14)
     .setText("HOME")
     ;  
  cp5.addButton("allowturn")
     .setPosition(60,510)
     .setSize(80,30)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
     ;
  cp5.addButton("stopturn")
     .setPosition(60,550)
     .setSize(80,30)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
     ;
  cp5.getController("allowturn")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(14)
     .setText("TURNSTART")
     ;  
  cp5.getController("stopturn")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(14)
     .setText("TURNSTOP")
     ;  
  cp5.addButton("windcheck")
     .setPosition(10,510)
     .setSize(45,30)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
     ;
  cp5.getController("windcheck")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(14)
     .setText("WIND")
     ;  
 
  cmdFirst=true;
}


void cmdcontrolEvent(ControlEvent theEvent) 
{//
  
  if(cmdFirst) {
     
    if(flight_mode==1){//manual mode  ////    
      if(theEvent.getController().getName()=="right")sendCmdMessege("5,1;\r\n");  
      if(theEvent.getController().getName()=="right2")sendCmdMessege("5,2;\r\n"); 
      if(theEvent.getController().getName()=="neutral")sendCmdMessege("5,0;\r\n");
      if(theEvent.getController().getName()=="left")sendCmdMessege("5,-1;\r\n"); 
      if(theEvent.getController().getName()=="left2")sendCmdMessege("5,-2;\r\n"); 
    }////
    if(theEvent.getController().getName()=="windcheck")sendCmdMessege("7,1;\r\n");
    if(theEvent.getController().getName()=="allowturn")sendCmdMessege("6,1;\r\n");
    if(theEvent.getController().getName()=="stopturn")sendCmdMessege("6,0;\r\n");
    if(theEvent.getController().getName()=="auto")sendCmdMessege("4,0;\r\n"); 
    if(theEvent.getController().getName()=="manual")sendCmdMessege("4,1;\r\n"); 
      
    if(theEvent.getController().getName()=="setDestination"){///
        destination_locked=true;
        playHome();
        //There is some floating error
        //send:372875280,1270626890,40
        //receive :37.2875289,127.0626831,40.00 >> within 3m

        // cchamchi's house  
        //dest_lat=37.2875280;
        //dest_lng=127.062689;
        //dest_alt =40.0;
       // seohyun station
        //dest_lat=37.387078;
        //dest_lng=127.122669;
        //parking
        
       // dest_lat=37.387044;
        //dest_lng=127.122626;
       
        //37.387044, 127.122626
        //String latitude=nf(dest_lat,3,7);
        //String longitude=nf(dest_lng,3,7);        
        dest_lat=37.28985;//lat;
        dest_lng=127.0651;//lng;
        dest_alt=68.5;//alt;
        
        String latitude=nf(dest_lat,3,7);
        String longitude=nf(dest_lng,3,7);        
        String altitude = str(dest_alt);
        
        //String altitue = str(dest_alt);
        sendCmdMessege("3,"+latitude+","+longitude+","+altitude+ ";\r\n");     
        print("send:");print(latitude);print(",");print(longitude);print(",");println(altitude);
     }///
  }//if(cmdFirst)   ////
  
  


}//

void sendCmdMessege(String cmd){
  if(myPort!=null)myPort.write(cmd);
}

boolean automodeplayed=false;
boolean manumodeplayed=true;
void drawCmd(){
  textAlign(CENTER);
  fill(0,0,0);
  textSize(25);  
  String mode = (flight_mode==1)? "MANU":"AUTO" ;
  text(mode, 250,570); 
  
  if((flight_mode==1) && !manumodeplayed){
    playManumode();
    manumodeplayed=true;
    automodeplayed=false;
  }
  if((flight_mode==0) && !automodeplayed){
    playAutomode();
    manumodeplayed=false;
    automodeplayed=true;
  }  
}
