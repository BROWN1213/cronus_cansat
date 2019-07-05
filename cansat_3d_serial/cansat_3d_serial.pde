PShape objShape;
float angle=0;
void setup(){
  size(400, 400, P3D);
  // Load the obj model normally
  objShape = loadShape("cansat.obj");
}

void draw(){
  background(0);
  directionalLight(126, 126, 126, 0, 0, -1);
  ambientLight(102, 102, 102);
  translate(width*2.5/5, 2.5*height/5, 0);
  scale(2);
  //rotate(PI/2);
  //rotateZ(PI/2);
  rotateX(angle*0.3);
  rotateY(angle*0.5);
  rotateZ(angle*0.3);
  shape(objShape);
  
  angle += 0.02;
}
