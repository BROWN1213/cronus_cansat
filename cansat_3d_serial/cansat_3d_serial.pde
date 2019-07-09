PShape objShape;
float angle=0;
import processing.serial.*;

Serial myPort;
float roll,pitch,yaw;
void setup(){
  size(400, 400, P3D);
  // Load the obj model normally
  objShape = loadShape("cansat.obj");
  
    String[] port_name=Serial.list();
    println(port_name);
    myPort = new Serial(this, "/dev/cu.usbserial-AL029OU2", 115200);
    myPort.bufferUntil('\n');    
}

void draw(){
  background(0);
  directionalLight(126, 126, 126, 0, 0, -1);
  ambientLight(102, 102, 102);
  translate(width*2.5/5, 2.5*height/5, 0);
  scale(2);
  //rotate(PI/2);
  //rotateZ(PI/2);
  rotateX(radians(pitch));
  rotateY(radians(-yaw));
  rotateZ(radians(roll));
  shape(objShape);
  
  angle += 0.02;
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
        
        print(roll);print(",");
        print(pitch);print(",");
        println(yaw);
                
      }
     

  }
   
}
