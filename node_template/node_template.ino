#include <SPI.h>
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <EEPROM.h>

/*
#define NUMBER_OF_SENSORS 3

const String sensorName[NUMBER_OF_SENSORS] = {};
double sensorValue[NUMBER_OF_SENSORS];
*/
RF24 radio(7,8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);
#define nodeID 1

unsigned long lastsent = 0;
#define INTERVAL 2000

void setup() {
  // put your setup code here, to run once:
  
 
  mesh.setNodeID(nodeID);
  mesh.begin(0x40,RF24_250KBPS,60000);
  

}
/*
String getJSON() {
  String jsonObject = "{\"node\":\"teich\",\"data\":[";
  for(int i = 0; i < NUMBER_OF_SENSORS; i++){
    jsonObject = jsonObject + "{\"name\":\"" + sensorName[i] + "\",\"val\":" + sensorValue[i] + "},";
  }
  jsonObject = jsonObject + "],}";
  return jsonObject;
}
*/


void loop() {
  mesh.update();
  


  
  if (!mesh.checkConnection()) {
    mesh.renewAddress();
  }
}

