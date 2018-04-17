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

#define NUMBER_OF_SENSORS 4

Adafruit_AM2320 am2320 = Adafruit_AM2320();


String sensorName[NUMBER_OF_SENSORS] = 
                       {"teich_airtemp_dallas",
                        "teich_watertemp",
                        "teich_airtemp_am2320",
                        "teich_relativeHumidity"};
double sensorValue[NUMBER_OF_SENSORS];

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
  am2320.begin();
  
  mesh.setNodeID(nodeID);
  mesh.begin(channel,RF24_250KBPS,60000);
  

}

String getJSON() {
  String jsonObject = "{\n\"node\":\"teichNode\",\n\"data\":[\n";
  for(int i = 0; i < NUMBER_OF_SENSORS; i++){
    jsonObject = jsonObject + "{\"name\":\"" + sensorName[i] + "\",\n\"value\":" + sensorValue[i] + "\n},\n";
  }
  jsonObject = jsonObject + "],\n}";
  return jsonObject;
}

void getSensorVals(){
    tempSensors.requestTemperatures();  //Request new temp readings from the dallas temperature sensors (ds18b20)
    sensorValue[1] = tempSensors.getTempCByIndex(0);
    sensorValue[2] = tempSensors.getTempCByIndex(1);
    sensorValue[3] = am2320.readTemperature();
    sensorValue[4] = am2320.readHumidity();
}

void loop() {
  mesh.update();
  
  if((now = millis()) >= (lastsent + interval)){
    getSensorVals();
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

