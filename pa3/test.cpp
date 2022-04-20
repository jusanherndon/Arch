#include<iostream>
#include<random>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[]){

int devices = std::atoi(argv[1]); 
int counter = 1;
int breakCondition = 0;	
int device[devices];

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

		device[i] = distr(gen);

		for(int b = 0; b < check; b++){
			if(device[i] == windowSize[b]){
       				device[i] = -1;
				windowSize[b] = -1;
			}	
		}

		if(device[i]==-1){
			breakCondition += 1;		

			std::cout << "Break Condition" << breakCondition << std::endl;
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

// Binary Exponential Backoff



// LogLog Backoff



return 0;
}
