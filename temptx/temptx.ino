#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensors(&oneWire);
double temp[2] = {-99,-99};

RF24 radio(7,8);
byte addresses[][6] = {"Temp","Tele"};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  tempSensors.begin();
Serial.print("---");
  radio.begin();                          
  radio.setChannel(0x30);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(1);                     
  radio.setRetries(2,15);                  
  radio.setCRCLength(RF24_CRC_8);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);

}

void loop() {
  // put your main code here, to run repeatedly:
  tempSensors.requestTemperatures();
  temp[0] = tempSensors.getTempCByIndex(0);
  temp[1] = tempSensors.getTempCByIndex(1);
  radio.write( &temp ,sizeof(temp));
  Serial.print(temp[0]);
  Serial.print("---");
  Serial.println(temp[1]);
  delay(1000);
  
}

