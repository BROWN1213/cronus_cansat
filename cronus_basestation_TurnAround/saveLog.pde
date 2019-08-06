// https://www.instructables.com/id/Data-Logging-SensorsInputs-With-Processing/

Table gpsLog;
Table imuLog;
Table serialLog;
Table otherValueLog;
String filename;
int isCheckWind=0;
String gpsTime="not defined yet";

boolean save_first;
void setupFileLog(){
  
  otherValueLog = new Table();
  otherValueLog.addColumn("PM2.5");
  otherValueLog.addColumn("Wind Direction");
  otherValueLog.addColumn("Wind Speed");
  serialLog = new Table();
  serialLog.addColumn("sysTime");
  serialLog.addColumn("serialLOG");
  
  gpsLog = new Table();
  gpsLog.addColumn("gpsTime");
  gpsLog.addColumn("sysTime"); 
  gpsLog.addColumn("Lat");
  gpsLog.addColumn("Lng");
  gpsLog.addColumn("Alt");  
  gpsLog.addColumn("speed");  
  gpsLog.addColumn("course");  
  gpsLog.addColumn("bearing");  
  gpsLog.addColumn("dist");   
  gpsLog.addColumn("ctlangle");
  gpsLog.addColumn("isWindCheck");
  
  
  imuLog = new Table();
  imuLog.addColumn("sysTime");
  imuLog.addColumn("roll");
  imuLog.addColumn("pitch");
  imuLog.addColumn("yaw");
  imuLog.addColumn("ax");
  imuLog.addColumn("ay");
  imuLog.addColumn("az");
  imuLog.addColumn("sqrt");
  imuLog.addColumn("average");
  
  
  cp5.addButton("saveToCSV")
     .setValue(0)
     .setPosition(10,20)
     .setSize(100,19)
     ;
   PFont pfont = createFont("Arial",20,true); // use true/false for smooth/no-smooth
  ControlFont font = new ControlFont(pfont,241);
  cp5.getController("saveToCSV")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(12)
     ;       
     
  save_first=true;  
}
void appendOtherValueLog(){
  TableRow newRow = otherValueLog.addRow();  
  newRow.setString("PM2.5", densityPM);
  newRow.setString("Wind direction", windDirection);
  newRow.setString("Wind", windSpeed);
}
void appendGpsLog(){
  TableRow newRow = gpsLog.addRow();  
  newRow.setString("gpsTime", gpsTime);
  newRow.setString("sysTime", str(day()) + ":" + str(hour()) + ":" + str(minute()) + ":" + str(second())+":" + str(millis()));
  newRow.setString("Lat", str(lat));
  newRow.setString("Lng", str(lng));
  newRow.setString("Alt", str(alt));
  newRow.setString("speed", str(speed_ms));
  newRow.setString("course", str(course));
  newRow.setString("bearing", str(bearing));
  newRow.setString("dist", str(distance));
  newRow.setString("ctlangle", str(control_angle));  
  newRow.setString("isCheckWind", str(isCheckWind));
  
  
        
}

void appendImuLog(){
  
  //add a new row for each value
  TableRow newRow = imuLog.addRow();  
  newRow.setString("sysTime", str(day()) + ":" + str(hour()) + ":" + str(minute()) + ":" + str(second())+":" + str(millis()));
  newRow.setString("roll", str(roll));
  newRow.setString("pitch", str(pitch));
  newRow.setString("yaw", str(yaw));
  newRow.setString("ax", str(ax));
  newRow.setString("ay", str(ay));
  newRow.setString("az", str(az));
  newRow.setString("sqrt", str(root));
  newRow.setString("average", str(avg));
  
  
}

void appendSerialLog(String value){

  
  TableRow newRow = serialLog.addRow(); 
  newRow.setString("sysTime", str(day()) + ":" + str(hour()) + ":" + str(minute()) + ":" + str(second())+":" + str(millis()));
  newRow.setString("serialLOG", value);
}

public void saveToCSV(int theValue){
  if(save_first){
    
    println("save");
    filename = "data/" +"GPS"+ str(month()) + "-" + str(day()) + "--" + str(hour()) + "-" + str(minute()) + ".csv";
    //save as a table in csv format(data/table - data folder name table)
    saveTable(gpsLog, filename);
    filename = "data/" +"Values"+ str(month()) + "-" + str(day()) + "--" + str(hour()) + "-" + str(minute()) + ".csv";
    saveTable(otherValueLog,filename);
    filename = "data/" +"IMU"+ str(month()) + "-" + str(day()) + "--" + str(hour()) + "-" + str(minute()) + ".csv";
    //save as a table in csv format(data/table - data folder name table)
    saveTable(imuLog, filename);
    
    filename = "data/" +"Serial"+ str(month()) + "-" + str(day()) + "--" + str(hour()) + "-" + str(minute()) + ".csv";
    //save as a table in csv format(data/table - data folder name table)
    saveTable(serialLog, filename);  
    //exit();  
  }
  save_first=true;
}
