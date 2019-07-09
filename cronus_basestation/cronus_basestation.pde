
void setup(){
  size(1000, 600,P2D);
  setup3d();
  setupSatellite();
  setupbattery();
  setupSerial();
}

void draw(){
  background(0);
  loadMap();
  satellite_info();
  drawBattery();
  draw3d(); 
}
