#include<iostream>
#include<cmath>
#include<math.h>
#include<random>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[]){

int devices = std::atoi(argv[1]); 
int logs = 0;
int counter = 1;
int counter1 = 1;
int counter2 = 1;
int breakCondition = 0;	
int breakCondition1 = 0;	
int breakCondition2 = 0;	
int device[devices];
int device1[devices];
int device2[devices];

// Linear Backoff

while(1){
	int windowSize[counter + 1]; 
	int check = counter + 1;

	for(int a = 0; a < check; a++){
		windowSize[a] = a;
	}

	for(int i =0; i <devices;i++){
		// Generating a random number from stackoverflow.com/questions/7560114/random-number-c-in-some-range
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0,counter);
		
		if(device[i] != -1){
			device[i] = distr(gen);
		}

		for(int b = 0; b < check; b++){
			if(device[i] == windowSize[b]){
       				device[i] = -1;
				windowSize[b] = -1;
			}	
		}

		if(device[i]==-1){
			breakCondition += 1;		
		}
		if(breakCondition == devices){
			break;
		}	
	}

	if(breakCondition == devices){
		break;
	}			
	
	breakCondition = 0;		
	counter += 1;
}	
std::cout << "This is the amount of Windows required for Linear Backoff: " << counter << std::endl;

// Binary Exponential Backoff
while(1){
	int windowSize1[int(exp2(counter1))];
	int check1 = int(exp2(counter1));

	for(int a = 0; a < check1; a++){
		windowSize1[a] = a;
	}

	for(int i =0; i <devices;i++){
		// Generating a random number from stackoverflow.com/questions/7560114/random-number-c-in-some-range
		std::random_device rd1;
		std::mt19937 gen1(rd1());
		std::uniform_int_distribution<> distr(0,check1);
		
		if(device1[i] != -1){
			device1[i] = distr(gen1);
		}

		for(int b = 0; b < check1; b++){
			if(device1[i] == windowSize1[b]){
       				device1[i] = -1;
				windowSize1[b] = -1;
			}	
		}

		if(device1[i]==-1){
			breakCondition1 += 1;		
		}
		
		if(breakCondition1 == devices){
			break;
		}	
	}
	
	if(breakCondition1 == devices){
		break;
	}			
	
	breakCondition1 = 0;		
	counter1 += 1;
}	
std::cout << "This is the amount of Windows required for Binary Exponential Backoff: " << counter1 << std::endl;


// LogLog Backoff
while(1){
	logs = 1+ floor(1/log2(log2(4*counter2))*exp2(counter2));

	int windowSize2[counter2*logs]; 

	int check2 = counter2*logs;

	for(int a = 0; a < check2; a++){
		windowSize2[a] = a;
	}

	for(int i =0; i <devices;i++){
		// Generating a random number from stackoverflow.com/questions/7560114/random-number-c-in-some-range
		std::random_device rd2;
		std::mt19937 gen2(rd2());
		std::uniform_int_distribution<> distr(0,check2);

		if(device2[i] != -1){
			device2[i] = distr(gen2);
		}

		for(int b = 0; b < check2; b++){
			if(device2[i] == windowSize2[b]){
       				device2[i] = -1;
				windowSize2[b] = -1;
			}	
		}

		if(device2[i]==-1){
			breakCondition2 += 1;		
		}
		
		if(breakCondition2 == devices){
			break;
		}	
	}
	
	if(breakCondition2 == devices){
		break;
	}			
	
	breakCondition2 = 0;		
	counter2 += 1;
}	
std::cout << "This is the amount of Windows required for Log Log Backoff: " << counter2 << std::endl;


return 0;
}
