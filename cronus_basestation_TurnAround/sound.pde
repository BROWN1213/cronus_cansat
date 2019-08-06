import processing.sound.*;
SoundFile file;

void playConneted(){

  file = new SoundFile(this, "connected.wav");
  file.play();
  
}

void playLanding(){
  file = new SoundFile(this, "land.wav");
  file.play();  
}

void playGPSlocked(){
  file = new SoundFile(this, "gpslock.wav");
  file.play();  
}

void playSignalLost(){
  file = new SoundFile(this, "siglost.wav");
  file.play();  
}

void playSignalRecovered(){
  file = new SoundFile(this, "sigrecover.wav");
  file.play();  
}

void playAutomode(){
  file = new SoundFile(this, "automode.wav");
  file.play();  
}
void playManumode(){
  file = new SoundFile(this, "manumode.wav");
  file.play();  
}

void playHome(){
  file = new SoundFile(this, "home.wav");
  file.play();  
}


void playApproach(){
  file = new SoundFile(this, "approach.wav");
  //file.play();  
}
void playTurning(){
  file = new SoundFile(this, "TurningStart.wav");
  file.play();
}
