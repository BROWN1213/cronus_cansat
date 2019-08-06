boolean isTurnAround=false;
boolean isGetWind=false;
String Turnable="[X]";
String Checkable="[X]";
void isTurnOkay(){
  textSize(30);
  text(Turnable,160,575);
  
  
  if(isTurnAround){
    Turnable="[O]";
    
  }
  else{
    Turnable="[X]";
  }
 
}
void isGetWindOk(){
  textSize(30);
  text(Checkable,20,500);
  
  
  if(isGetWind){
    Checkable="[O]";
    
  }
  else{
    Checkable="[X]";
  }
 
}
