
void setup(){
  size(1000, 600,P2D);
  setupSerial(); // setupSerial at first to share ControlP5 cp5 ocject
  setup3d();
  setupSatellite();
  setupbattery();

  setupGraph();
  setupFileLog();
  setupNav();
  setupCmdMessege();
    
}

void draw(){
  background(200);
  loadMap();
  drawNav(); //draw next loadmap
  isTurnOkay();
  isGetWindOk();
  satellite_info();
  drawBattery();
  draw3d(); 
  checkConnection();
  drawGraph();
  //fill(0, 255, 255);
  //rect(600,400,380,180);
  drawCmd();
}
