#include "RF24Mesh/RF24Mesh.h"  
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
RF24 radio( 22, 0, BCM2835_SPI_SPEED_8MHZ);  
RF24Network network(radio);
RF24Mesh mesh(radio,network);

const uint8_t channel = 0x40;	//nrf24l01 type channel of the mesh network

std::string jsonValues;

int main(int argc, char** argv) {
  
	mesh.setNodeID(0);	// Set the nodeID to 0 for the master node

	printf("start\n");
	mesh.begin(channel,RF24_250KBPS,60000);		// Connect to the mesh, connection speed 250kbps
	radio.printDetails();	//print Radio Settings,for debugging
while(1) {
  
    mesh.update();	// Call network.update as usual to keep the network updated
	mesh.DHCP();	// In addition, keep the 'DHCP service' running on the master node so addresses will be assigned to the sensor nodes
  
  while(network.available()){	// Check for incoming data from the sensors
    RF24NetworkHeader header;
    network.peek(header);
    char buf[456];
    switch(header.type){	//check which kind of message was received
    
		case 'V': 
				network.read(header,&buf,456);	//received value from sensor node,max size 456 bytes, same as set in RF24Network_config.h
                printf("received V type header\n");
				jsonValues = buf;
				printf(jsonValues);
                break;
		case 'R': 
				network.read(header,&buf,456); 	//received data request from actuator or display node, max size 456 bytes
                printf("received R type header\n");
                break;		 
		default:
				network.read(header,0,0); 	//unknown message type
                printf("received unknown message type\n"); 
                break;
    }
  memset(&buf[0],0,sizeof(buf));
  }
delay(2);
  }
return 0;
}
      
      