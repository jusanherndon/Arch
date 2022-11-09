#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fstream>
#include <arpa/inet.h>
#include <string.h>
#include <cstring>
#include <unistd.h>

int main(int argc, char *argv[]){

	std::ifstream myfile; 
	int counter = 0;
	char payload[6];
	char message[] = "1248"; //add arguement to take in coordinates
	struct hostent *s;
	struct sockaddr_in server;
	struct in_addr a;

	s = gethostbyname(argv[2]);

	int mysocket = 0;
	socklen_t slen = sizeof(server);

	if((mysocket= socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		std::cout << "Error in creating socket\n";
	}
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(std::atoi(argv[1]));
	bcopy((char *)s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);
	
	printf("name: %s\n", s->h_name);
	
	while(*s->h_aliases){
		printf("alias: %s",*s->h_aliases++);
	}
	while(*s->h_addr_list){
		bcopy(*s->h_addr_list++, (char *) &a, sizeof(a));
		printf("address: %s\n", inet_ntoa(a));
	}

// Logic that handles getting lines from file and creating a payload
	myfile.open(argv[3]);

	while(1){
		
		myfile.read(payload,4); 
		payload[5]= 0;
		payload[6]= 0;

		std::cout << "Payload contains: " << payload << std::endl;
		
		if(myfile.eof() == 1) {
			break;
		}

		if(sendto(mysocket,payload,6,0, (struct sockaddr *)&server,slen)==-1){
			std::cout << "Error in sendto function.\n";
		}
		
		counter += 1;
	}

	char finish[6];
	
	memset(finish,0,6);

	finish[5] = 1;
	
	if(sendto(mysocket,finish,6,0, (struct sockaddr *)&server,slen)==-1){
			std::cout << "Error in sendto function.\n";
		}

	myfile.close();

	char ack[10];

	recvfrom(mysocket, ack, 10,0, (struct sockaddr *)&server, &slen);
	close(mysocket);
	return 0;
}
