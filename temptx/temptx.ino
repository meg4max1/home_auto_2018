#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>

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

#define NUMBER_OF_SENSORS 3

Adafruit_AM2320 am2320 = Adafruit_AM2320();


const String sensorName[NUMBER_OF_SENSORS] = {"airT", "waterT", "relH"};

double sensorValue[NUMBER_OF_SENSORS];

RF24 radio(7,8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);
#define nodeID 1

unsigned long lastsent = 0;
#define INTERVAL 2000

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tempSensors.begin();
  am2320.begin();
 
  mesh.setNodeID(nodeID);
  mesh.begin(0x40,RF24_250KBPS,60000);
  

}

String getJSON() {
  String jsonObject = "{\"node\":\"teich\",\"dat\":[";
  for(int i = 0; i < NUMBER_OF_SENSORS; i++){
    jsonObject = jsonObject + "{\"name\":\"" + sensorName[i] + "\",\"val\":" + sensorValue[i] + "},";
  }
  jsonObject = jsonObject + "],}";
  return jsonObject;
}

void getSensorVals(){
    tempSensors.requestTemperatures();  //Request new temp readings from the dallas temperature sensors (ds18b20)
    double airtd = tempSensors.getTempCByIndex(0);
    double airta = am2320.readTemperature();
    if (airtd == -127.00 || airtd == 0.00){
      sensorValue[0] = airta;
    }
    else{
      sensorValue[0] = ((2*airtd + airta)/3);
    }
    sensorValue[1] = tempSensors.getTempCByIndex(1);
    sensorValue[2] = am2320.readHumidity();
}

void loop() {
  mesh.update();
  
  if((millis()) >= (lastsent + INTERVAL)){
    getSensorVals();
    String jsonObject = getJSON();  //build a new JSON String with new sensor values
    mesh.write( jsonObject.c_str() , 'V', jsonObject.length()); // send a "value" Type Message containing the jsonObject
    Serial.println(jsonObject);
    lastsent = millis();
  }

  
  if (!mesh.checkConnection()) {
    mesh.renewAddress();
  }
}

