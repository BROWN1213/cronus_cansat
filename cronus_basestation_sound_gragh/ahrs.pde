PGraphics buffer, canvas;
PShape objShape;
float roll,pitch,yaw,ax,ay,az;



void setup3d(){
  canvas = createGraphics(400, 400, P3D);
  objShape = loadShape("cansat.obj");
}

void draw3d(){
  pushMatrix();
  canvas.beginDraw();
  canvas.background(0);
  canvas.directionalLight(126, 126, 126, 0, 0, -1);
  canvas.ambientLight(102, 102, 102);
  canvas.translate(400*2.5/5, 2.5*400/5, 0);   
  canvas.scale(2);
  canvas.rotateX(radians(pitch));
  canvas.rotateY(radians(-yaw));
  canvas.rotateZ(radians(roll));
  canvas.shape(objShape);
  canvas.endDraw();
  image(canvas, 600, 0);
  popMatrix();
}
