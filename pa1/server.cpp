#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]){

	struct sockaddr_in server;
	struct sockaddr_in client;
	int r_port;
	std::ofstream myfile;
	std::string myline;
	char r_port_char[64];
	int mysocket = 0;
	socklen_t clen = sizeof(client);
	char payload[6];
	char message[512];

	if((mysocket= socket(AF_INET, SOCK_DGRAM, 0)) ==-1 ){
		cout << "Error in socket creation.\n";
	}
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(std::atoi(argv[1]));
	server.sin_addr.s_addr = htons(INADDR_ANY);
	if(bind(mysocket, (struct sockaddr *)&server, sizeof(server)) ==-1){
		cout << "Error in binding.\n";
	}

	myfile.open("upload.txt");	

	memset(payload,0,6);
	while(1){
		if(recvfrom(mysocket,payload, 6, 0, (struct sockaddr *)&client, &clen)== -1){
			cout << "Failed to receive.\n";
		}
		
		if(payload[5] == 1){
			break;
		}
		if(payload[5] == 0){
			myfile << payload;
		}
	}
	
	myfile.close();

	char ack[] = "Got all that data, thanks";

	if(sendto(mysocket, ack, 20, 0, (struct sockaddr *)&client, clen) == -1){
		cout << "Error in sendto function.\n";
	}

	close(mysocket);
	return 0;
}
