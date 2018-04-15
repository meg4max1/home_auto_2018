#include <SpacebrewYun.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>

#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <EEPROM.h>

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensors(&oneWire);
double temp[2] = {-99,-99};

//RF24 radio(7,8);
//RF24Network network(radio);
//RF24Mesh mesh(radio, network);
#define nodeID 1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //tempSensors.begin();

  //mesh.setNodeID(nodeID);
  //mesh.begin();
  
 /* radio.begin();                          
  radio.setChannel(0x30);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(1);                     
  radio.setRetries(2,15);                  
  radio.setCRCLength(RF24_CRC_8);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);*/

}

String getJSON() {
  String jsonObject = "{\n\"node\":\"teichNode\",\n\"data\":[\n";
  for(int i = 0; i < sizeof(temp)/sizeof(*temp); i++){
    jsonObject = jsonObject + "{\"name\":\"temp" + i + "\",\n\"value\":" + temp[i] + "\n},\n";
  }
  jsonObject = jsonObject + "],\n}";
}

void loop() {
  //mesh.update();
  
  tempSensors.requestTemperatures();
  temp[0] = tempSensors.getTempCByIndex(0);
  temp[1] = tempSensors.getTempCByIndex(1);
  
  Serial.print(getJSON());
  //radio.write( &temp ,sizeof(temp));
  delay(1000);
  
  /*if (!mesh.checkConnection()) {
    Serial.println("Renewing Address");
    mesh.renewAddress();
  }*/
}

