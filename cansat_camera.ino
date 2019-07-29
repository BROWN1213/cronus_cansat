/*
#include "src/cansat_vc0703.h"
#include "src/PacketSerial.h"
VC0703 cansatCAM(&Serial1);

PacketSerial myPacketSerial;
//bool isGps_data_fix=false;  // Means gps data is valid
//void setup() {
//  Serial.begin(115200);
//  //Serial.println("Cansat start");
//  myPacketSerial.setStream(&Serial);
//  CansatSystemInit(); 
//  setupCamera();
//}
//void loop(){
//  
//}
void setupCamera(){
  if (cansatCAM.begin()) {
   // Serial.println("Camera Found:");
  } else {
   // Serial.println("No camera found?");
    return;
  }

  
  // Print out the camera version information (optional)
  char *reply = cansatCAM.getVersion();
  if (reply == 0) {
   // Serial.print("Failed to get version");
  } else {
   // Serial.println("-----------------");
   // Serial.print(reply);
   // Serial.println("-----------------");
  }  

  //cansatCAM.setImageSize(VC0703_640x480);        // biggest
  //cansatCAM.setImageSize(VC0703_320x240);        // medium
  cansatCAM.setImageSize(VC0703_160x120); // small  
  // You can read the size back from the camera (optional, but maybe useful?)
  uint8_t imgsize = cansatCAM.getImageSize();
  //Serial.print("Image size: ");
  if (imgsize == VC0703_640x480) Serial.println("640x480");
  if (imgsize == VC0703_320x240) Serial.println("320x240");
  if (imgsize == VC0703_160x120) Serial.println("160x120");  
  //Serial.println("Snap in 3 secs...");
  delay(3000);
  int32_t time = millis();
  cansatCAM.takePicture();
 // if (! cansatCAM.takePicture()) 
    //Serial.println("Failed to snap!");
//  else 
    //Serial.println("Picture taken!");  
  time = millis() - time;
  //Serial.print(time); Serial.println(" ms elapsed");  
   
  // Get the size of the image (frame) taken  
  uint16_t jpglen = cansatCAM.frameLength();
//  Serial.print(jpglen, DEC);
//  Serial.print(" byte image.");  
  char  s_header = "\n";
  //Serial.write(&s_header,1);
  time = millis();
         // Make an array.
         uint8_t myPacket[3] = { 255, 10 ,2};
    
        // Send the array.
        myPacketSerial.send(myPacket, 3);
  //Serial.print("%,8,");Serial.print(jpglen);Serial.print("\n");
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cansatCAM.readPicture(bytesToRead);
//    Serial.print("%,9,");  // header,class,num data
//    Serial.print(bytesToRead);Serial.print(",");
//    for (int i=0;i<bytesToRead-1;i++){
//    Serial.write(buffer+i, 1);//Serial.print(",");
//    }
//    Serial.write(buffer+bytesToRead-1, 1);Serial.print("\n");
    //myPacketSerial.send(buffer, bytesToRead);
    jpglen -= bytesToRead;

    delay(50);
  }  
  Serial.print("%,10");Serial.print("\n");
  time = millis() - time;
  //Serial.print(time); Serial.println(" ms elapsed");  
}

void updateCamera(){

  
}
*/
