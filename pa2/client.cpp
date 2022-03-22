// Author: Justin Herndon using Maxwell Young's code as an example

#include <stdlib.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
#include <fstream>
#include <arpa/inet.h> 
#include <string.h>
#include <unistd.h>
#include "packet.h"
#include <math.h>

using namespace std;

int main(int argc, char *argv[]){
 
std::ifstream myfile; 
std::ofstream clientseqnum; 
std::ofstream clientack; 

int packetLen=30;

// This is the definition for the CE Socket or the scoket used for the client sending to the server
struct hostent *em_host;
em_host = gethostbyname(argv[1]);
int CESocket = socket(AF_INET, SOCK_DGRAM, 0);  

if(CESocket < 0){
  	cout << "Error: failed to open datagram socket.\n";
  } 
struct sockaddr_in CE; 
socklen_t CE_length = sizeof(CE);
bzero(&CE, sizeof(CE)); 
CE.sin_family = AF_INET;	
bcopy((char *)em_host->h_addr, (char*)&CE.sin_addr.s_addr, em_host->h_length);
char * end;
int em_rec_port = strtol(argv[2], &end, 10); 
CE.sin_port = htons(em_rec_port); 
  
if ((CESocket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
	cout << "Error in creating socket.\n";


// This is the definition for the socket used to send receive acks from the server

int ECSocket = socket(AF_INET, SOCK_DGRAM, 0);  
if(ECSocket < 0){
	cout << "Error: failed to open datagram socket.\n";
}

struct sockaddr_in EC; 
socklen_t EC_length = sizeof(EC);
bzero(&EC, sizeof(EC)); 
EC.sin_family = AF_INET;	
EC.sin_addr.s_addr = htonl(INADDR_ANY);	
char * end2;
int cl_rec_port = strtol(argv[3], &end2, 10); 
EC.sin_port = htons(cl_rec_port);            

// This is where the timout and socket option is set to allow for timeouts
int timeout = 2000;

setsockopt(CESocket,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));


//This is where the sockets are bound and the connection is created 
if (bind(ECSocket, (struct sockaddr *)&EC, EC_length) == -1){
	cout << "Error in binding.\n";
}

// Below is where my packet length is definied, my sent and receive 
char payload[30];
char spacketA[packetLen];

myfile.open(argv[4]);
clientseqnum.open("clientseqnum.log");
clientack.open("clientack.log");
packet rcvdPacket(0,0,0,payload);
char serialized[30];

memset(payload,0,packetLen);
memset(spacketA, 0, packetLen);

int counter = 0;

// This is the main loop where packets are sent and received
while(1){

	if(counter == 2){
		counter = 0;
	}
	// handles file input and serialization
	myfile.read(payload,packetLen);
		
  	packet mySendPacket(1, counter, strlen(payload), payload);
  	mySendPacket.serialize(spacketA);

	clientseqnum << counter << "\n";
	
	// used to send a data packet and a receive for the ack
  	if(sendto(CESocket, spacketA, 30, 0, (struct sockaddr *)&CE, CE_length)==-1){
		std::cout << "Error in sendto function.\n";
	}
	
	if(recvfrom(ECSocket, serialized, 30, 0, (struct sockaddr *)&EC, &EC_length)==-1){
		std::cout << "Error in sendto function.\n";
	}
		
	rcvdPacket.deserialize(serialized);
	
	clientack << rcvdPacket.getSeqNum() << "\n"; 	
	
	// condition used to end the connection
	if(rcvdPacket.getType() == 2){
		break;
    	}
	
	// condition used to tell the server this is the last data packet
	if(myfile.eof() == 1){
		memset(payload,0,packetLen);
		packet mySendPacket(3, counter, strlen(payload), payload);
		mySendPacket.serialize(spacketA);

  		if(sendto(CESocket, spacketA, 30, 0, (struct sockaddr *)&CE, CE_length)==-1){
			std::cout << "Error in sendto function.\n";
		}
	}
	
	counter += 1;
  }

// cleanup
clientseqnum.close();
clientack.close();
myfile.close();

close(CESocket);
close(ECSocket);

return 0;
}
