#include "RF24Mesh/RF24Mesh.h"
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>

RF24 radio( 22, 0, BCM2835_SPI_SPEED_8MHZ);
RF24Network network(radio);
RF24Mesh mesh(radio,network);

int main(int argc, char** argv) {

  // Set the nodeID to 0 for the master node
  mesh.setNodeID(0);
  mesh.begin();
  

  while(1){
	  mesh.update();
	  
	  mesh.DHCP();
	  
	    while(network.available()){

		RF24NetworkHeader header;
		network.peek(header);
		
			switch(header.type){
			