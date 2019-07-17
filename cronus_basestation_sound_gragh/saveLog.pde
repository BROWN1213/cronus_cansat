// https://www.instructables.com/id/Data-Logging-SensorsInputs-With-Processing/

Table gpsLog;
Table imuLog;
Table serialLog;
String filename;


boolean save_first;
void setupFileLog(){
  

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
  
  imuLog = new Table();
  imuLog.addColumn("sysTime");
  imuLog.addColumn("roll");
  imuLog.addColumn("pitch");
  imuLog.addColumn("yaw");
  imuLog.addColumn("ax");
  imuLog.addColumn("ay");
  imuLog.addColumn("az");
  
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

void appendGpsLog(){
  
  
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
}

void appendSerialLog(String value){

  
  TableRow newRow = serialLog.addRow(); 
  newRow.setString("sysTime", str(day()) + ":" + str(hour()) + ":" + str(minute()) + ":" + str(second())+":" + str(millis()));
  newRow.setString("serialLOG", value);
}

public void saveToCSV(int theValue){
  if(save_first){
    
    println("save");
    String filename = "data/" +"GPS"+ str(month()) + "-" + str(day()) + "--" + str(hour()) + "-" + str(minute()) + ".csv";
    //save as a table in csv format(data/table - data folder name table)
    saveTable(gpsLog, filename);
  
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
