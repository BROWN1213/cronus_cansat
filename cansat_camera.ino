
#include "src/cansat_vc0703.h"


VC0703 cansatCAM(&Serial1);

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
  if (imgsize == VC0703_640x480) //Serial.println("640x480");
  if (imgsize == VC0703_320x240) //Serial.println("320x240");
  if (imgsize == VC0703_160x120) //Serial.println("160x120");  
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
  uint8_t s_header = 0x76;
  Serial.write(&s_header,1);
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cansatCAM.readPicture(bytesToRead);
    //Serial.print("%3");  // header,class,num data
    //Serial.write(&bytesToRead,1);
    Serial.write(buffer, bytesToRead);
    //Serial.print("\n");
    jpglen -= bytesToRead;
  }  
  //Serial.println("%4");
}

void updateCamera(){

  
}
