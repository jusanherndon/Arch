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
  

std::ofstream myfile;
std::ofstream logFile;

// This is the definition of the packet Length and the first expected sequence number of the fist packet
int expectedSeqNum = 0;
int packetLen=30;

// This is the definition of the receiving socket from the client to this server 
int ESSocket = socket(AF_INET, SOCK_DGRAM, 0);  
if(ESSocket < 0){
	cout << "Error: failed to open datagram socket.\n";
}

struct sockaddr_in ES; 
socklen_t ES_length = sizeof(ES);
bzero(&ES, sizeof(ES)); 
ES.sin_family = AF_INET;	
ES.sin_addr.s_addr = htonl(INADDR_ANY);	
char * end;
int sr_rec_port = strtol(argv[2], &end, 10); 
ES.sin_port = htons(sr_rec_port);           
		
if (bind(ESSocket, (struct sockaddr *)&ES, ES_length) == -1)
	cout << "Error in binding.\n";		

struct hostent *em_host; 
	
em_host = gethostbyname(argv[1]);

if(em_host == NULL){    
	cout << "Failed to obtain server.\n";
	exit(EXIT_FAILURE);
}
//This is the definition of sending socket from the server to the client
int SESocket = socket(AF_INET, SOCK_DGRAM, 0);

if(SESocket < 0){
	cout << "Error in trying to open datagram socket.\n";
	exit(EXIT_FAILURE);
}
		
struct sockaddr_in SE;	
memset((char *) &SE, 0, sizeof(SE));
SE.sin_family = AF_INET;
bcopy((char *)em_host->h_addr, (char*)&SE.sin_addr.s_addr, em_host->h_length);
int em_rec_port = strtol(argv[3], &end, 10);
SE.sin_port = htons(em_rec_port);
	
char payload[30];
char serialized[30];  
char spacketA[packetLen];  
packet rcvdPacket(0,0,0,payload);

char ack[]="Got all that data, thanks!";

memset(serialized, 0, 30); 
memset(payload, 0, 30);

myfile.open(argv[4]);	
logFile.open("arrival.log");


while(1){
	if(expectedSeqNum == 2){
		expectedSeqNum = 0;	
	}
	 // code to receive a packet and to write to a file if it is the expected sequence number 
	if(recvfrom(ESSocket, serialized, packetLen, 0, (struct sockaddr *)&ES, &ES_length)==-1)
     		cout << "Failed to receive.\n";
	
	rcvdPacket.deserialize(serialized);	

    	if(expectedSeqNum == rcvdPacket.getSeqNum()){
		myfile << payload;
		logFile << rcvdPacket.getSeqNum() << "\n";
	}
	// logic to break when the packet is of type three from the dcoumentation
	if(rcvdPacket.getType() == 3){
		break;
    	}
    	
	// code to create the ack packet to send to the client	
	packet mySendAck(0, rcvdPacket.getSeqNum(), strlen(ack), ack);
	mySendAck.serialize(spacketA);

	if(sendto(SESocket, spacketA, packetLen, 0, (struct sockaddr *)&SE, sizeof(struct sockaddr_in))==-1){
		std::cout << "Error in sendto function.\n";
	}

	expectedSeqNum += 1;
}

myfile.close();
logFile.close();

// packet creation to send the last packet to stop the client from going on forever
memset(payload,0,packetLen);
packet mySendAck(2, rcvdPacket.getSeqNum(), strlen(ack), ack);
mySendAck.serialize(spacketA);

if(sendto(SESocket, spacketA, packetLen, 0, (struct sockaddr *)&SE, sizeof(struct sockaddr_in))==-1){
	std::cout << "Error in sendto function.\n";
}

close(SESocket);
close(ESSocket);

return 0;
}
