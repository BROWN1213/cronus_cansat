#include "src/cansat_BT.h"

static CansatHwSerial _BTport(Serial);
CansatBT<CansatHwSerial> cansatBT(_BTport);

void setupBT(){
  while(!cansatBT.isConnected()){

  }
  cansatBT.sendln("BT connected");
  cansatBT.sendln("Welcome to Cansat");
}

void testBT(){
  cansatBT.echo();
  cansatBT.send("Hello world: ");
  cansatBT.sendln((int)millis());

}
