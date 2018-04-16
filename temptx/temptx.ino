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

RF24 radio(7,8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);
#define nodeID 1
const uint8_t channel = 0x40;

unsigned long lastsent = 0;
unsigned long now = 0;
const int interval = 10000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  tempSensors.begin();

  mesh.setNodeID(nodeID);
  mesh.begin(channel,RF24_250KBPS,60000);
  

}

String getJSON() {
  String jsonObject = "{\n\"node\":\"teichNode\",\n\"data\":[\n";
  for(int i = 0; i < sizeof(temp)/sizeof(*temp); i++){
    jsonObject = jsonObject + "{\"name\":\"temp" + i + "\",\n\"value\":" + temp[i] + "\n},\n";
  }
  jsonObject = jsonObject + "],\n}";
  return jsonObject;
}

void loop() {
  mesh.update();
  
  if((now = millis()) >= (lastsent + interval)){
    tempSensors.requestTemperatures();  //Request new temp readings from the dallas temperature sensors (ds18b20)
    temp[0] = tempSensors.getTempCByIndex(0);
    temp[1] = tempSensors.getTempCByIndex(1);
    String jsonObject = getJSON();  //build a new JSON String with new sensor values
    Serial.print(jsonObject);
    mesh.write( &jsonObject , 'V', sizeof(jsonObject)); // send a "value" Type Message containing the jsonObject
    now = lastsent;
  }

  
  if (!mesh.checkConnection()) {
    Serial.println("Renewing Address");
    mesh.renewAddress();
  }
}

