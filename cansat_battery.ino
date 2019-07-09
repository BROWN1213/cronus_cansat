#define battery_pin 7   //AT7 pin
#define PROCESSING
//8.46v = 415  8.24 =407 8.15 396 7.96 398 7.78 388  7.6 376 6.9 325
// 415(4)395(3)375(2)355(1)345(0): warning


// 
#define L4 395
#define L3 375
#define L2 355
#define L1 345
#define V_NBUF 5
int v_buf[V_NBUF]; // 5sec data ,checkBattery() called every 1sec
int v_offset=0,v_length=0;
void checkBattery(){
  int v_level=0;
  int v_value = analogRead(battery_pin);
  float v_sum;
  // buffering V_NBUF data
  v_buf[v_offset]=v_value;
  v_offset++;
  v_offset %= V_NBUF;
  if( v_length<V_NBUF )v_length++;

 //calculate average v_value
  v_sum=0;
  for(int i=0;i<V_NBUF;i++) v_sum += v_buf[i];
  float avg_v=v_sum/float(v_length);
 
 // classify v_level
   if(avg_v>L4){
    v_level=4;
   }else if(avg_v>L3){
    v_level=3;
   }else if(avg_v>L2){
    v_level=2;
   }else if(avg_v>L1){
    v_level=1;
   }else{
    v_level=0;
   }

  
  

#ifdef SERIAL_MONITOR   
  Serial.print("Battery= ");Serial.println(v_level);
#endif

#ifdef PROCESSING
  Serial.print("%,3,1,");  // header,class,num data
  Serial.println(v_level);

#endif  
}
