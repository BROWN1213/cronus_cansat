

void setupLED(){
  // initialize digital pin LEDs as an output.
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  LEDbooting();
}

void LEDbooting(){
  LEDOffAll();
  LEDOn(LED0);
  delay(200);
  LEDOn(LED1);
  delay(200);
  LEDOn(LED2);
  delay(200);  
  LEDOn(LED3);
  delay(200);  
  LEDOffAll();
    
}

bool led0onoff;
void LED0toggle(){
  if(led0onoff){
    LEDOn(LED0);
    led0onoff=false;
  }else{
    LEDOff(LED0);
    led0onoff=true;
  }
}
bool led1onoff;
void LED1toggle(){
  if(led1onoff){
    LEDOn(LED1);
    led1onoff=false;
  }else{
    LEDOff(LED1);
    led1onoff=true;
  }
}
void LEDOn(int pin){
  digitalWrite(pin,LOW);
}
void LEDOff(int pin){
  digitalWrite(pin,HIGH);
}
void LEDOffAll(){
  digitalWrite(LED0,HIGH);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);
}
void LEDOnAll(){
  digitalWrite(LED0,LOW);
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
}
void LEDBlink(int pin,uint32_t timems){
  uint32_t start_time=millis();
  
  while((millis()-start_time)<timems){
    LEDOn(pin);
    delay(100);
    LEDOff(pin);
    delay(100);
  }
}
void LEDBlinkAll(uint32_t timems){
  uint32_t start_time=millis();
  
  while((millis()-start_time)<timems){
    LEDOnAll();
    delay(100);
    LEDOffAll();
    delay(100);
  }
}
