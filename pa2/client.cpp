// Author: Maxwell Young
// Created for Fall 2021 Data Comm. for helping a student with serialization

// To test this code, you need two shells open on Pluto
// run ./server
// run ./client localhost


#include <stdlib.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>   // defines types (like size_t)
#include <sys/socket.h>  // defines socket class
#include <netinet/in.h>  // defines port numbers for (internet) sockets, some address structures, and constants
#include <netdb.h> 
#include <iostream>
#include <fstream>
#include <arpa/inet.h>   // if you want to use inet_addr() function
#include <string.h>
#include <unistd.h>
#include "packet.h" // include packet class
#include <math.h>


using namespace std;

//#define packetLen 50

int main(int argc, char *argv[]){

int packetLen=50;
struct hostent *s; 
s = gethostbyname(argv[1]);
  
struct sockaddr_in server;
int mysocket = 0;
socklen_t slen = sizeof(server); 
  
if ((mysocket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
	cout << "Error in creating socket.\n";
    
memset((char *) &server, 0, sizeof(server));
server.sin_family = AF_INET;
server.sin_port = htons(7123);
bcopy((char *)s->h_addr, 
	(char *)&server.sin_addr.s_addr,
	s->h_length);
      
  	  
  // now going to send serialize and send 3 packets
char payload[50];// payload data
char spacketA[packetLen];  // for holding serialized packet  
  memset(spacketA, 0, packetLen); // serialize the packet to be sent

std::ifstream myfile; 
//myfile.open(argv[3]);
myfile.open("file.txt");
int counter = 0;
packet rcvdPacket(0,0,0,payload);
char serialized[50];

memset(payload,0,packetLen);
while(1){

	if(counter == 2){
		counter = 0;
	}
	myfile.read(payload,packetLen);

	std::cout << "Payload contains: " << payload << std::endl;
		

  	packet mySendPacket(1, counter, strlen(payload), payload); // make the packet to be serialized and sent
  	mySendPacket.serialize(spacketA); // serialize so spacket contains serialized contents of mySendPacket's payload
  	if(sendto(mysocket, spacketA, 50, 0, (struct sockaddr *)&server, slen)==-1){
		std::cout << "Error in sendto function.\n";
	}

	counter += 1;
	
	if(recvfrom(mysocket, serialized, 50, 0, (struct sockaddr *)&server, &slen)==-1){
		std::cout << "Error in sendto function.\n";
	}

    	cout << "Received packet and deserialized to obtain the following: " << endl << endl;
	rcvdPacket.deserialize(serialized);	
   	rcvdPacket.printContents();


	if(rcvdPacket.getType() == 2){
		break;
    	}

	if(myfile.eof() == 1){
		memset(payload,0,packetLen);
		packet mySendPacket(3, counter, strlen(payload), payload);
		mySendPacket.serialize(spacketA);

		if(sendto(mysocket, spacketA, 50, 0, (struct sockaddr *)&server, slen)==-1){
			std::cout << "Error in sendto function.\n";
		}
	}
  }

close(mysocket);
return 0;
}
