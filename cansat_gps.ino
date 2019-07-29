#include "src/cansat_gps.h"

 static CansatHwSerial _GPSport(Serial1);

 CansatGPS<CansatHwSerial> cansatGPS(_GPSport);

bool gps_searching=true;
void setupGPS() {
  // put your setup code here, to run once:
  cansatGPS.begin(9600);
  Serial.println(F("GPS begin"));

}

void reconnectGPS(){
  cansatGPS.begin(9600);
  cansatGPS.rx_empty();
}

bool isGpsLocked(){

  return (cansatGPS.status() >= 2) ? true : false;
}
void gpsSearchingCheck(){
      Serial.print("%,4,1,");  // header,class,num data
      Serial.println(cansatGPS.status());  
}

bool gpsAvailable(){
  return (cansatGPS.available()&&(cansatGPS.status()>0));
}

#define PROCESSING
void updateGPS() {
  // put your main code here, to run repeatedly:

  if(cansatGPS.read()){ 

   
      gps_searching=false;
      isGps_data_fix=true;
      
      LED1toggle();
     
#ifdef SERIAL_MONITOR    
       Serial.print("date=");
       Serial.println(cansatGPS.date());
       Serial.print("time=");
       Serial.println(cansatGPS.time()/100.);
       Serial.print("lat=");
       Serial.println((cansatGPS.location().lat* 1.0e-7f),7);
       Serial.print("lon=");
       Serial.println((cansatGPS.location().lng* 1.0e-7f),7);       
       Serial.print("altitude(m)=");
       Serial.println(cansatGPS.location().alt/100.); 
       Serial.print("num_sat=");
       Serial.println(cansatGPS.num_sats()); 
       Serial.print("speed(km/s)=");
       Serial.println(cansatGPS.ground_speed_km());
       Serial.print("cource=");
       Serial.println(cansatGPS.ground_course());
       Serial.println("*********************************");    
 #endif
#ifdef PROCESSING
      Serial.print("%,2,6,");  // header,class,num data
      Serial.print((cansatGPS.location().lat* 1.0e-7f),7);Serial.print(',');
      Serial.print((cansatGPS.location().lng* 1.0e-7f),7);Serial.print(',');
      Serial.print(cansatGPS.location().alt/100.);Serial.print(',');
      Serial.print(cansatGPS.num_sats());Serial.print(',');
      Serial.print(cansatGPS.ground_speed_ms());Serial.print(',');
      Serial.println(cansatGPS.ground_course());
      
#endif
  
  }

}

void gpsPassthrough(){
  while(Serial1.available()){
    Serial.write(Serial1.read());
  }
}
