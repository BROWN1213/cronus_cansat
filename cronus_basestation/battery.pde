
PShape battery0,battery1,battery2,battery3,battery4;
PShape b_level;
int bat_level;

void setupbattery(){
  battery0=loadShape("batt0.svg");battery0.rotate(PI/2);
  battery1=loadShape("batt1.svg");battery1.rotate(PI/2);
  battery2=loadShape("batt2.svg");battery2.rotate(PI/2);
  battery3=loadShape("batt3.svg");battery3.rotate(PI/2);
  battery4=loadShape("batt4.svg");battery4.rotate(PI/2);
  connect=loadShape("connect.svg");  
}

void drawBattery(){

  if(bat_level==0)shape(battery0, 260, 26, 50, 25); 
  if(bat_level==1)shape(battery1, 260, 26, 50, 25); 
  if(bat_level==2)shape(battery2, 260, -4, 50, 25); 
  if(bat_level==3)shape(battery3, 260, -34, 50,25); 
  if(bat_level==4)shape(battery4, 260, -64, 50, 25); 

}
