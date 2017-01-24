// Josh Reavis
// Summer 2016

// A simple calculator which takes single arithmetic operations from stdin
// and writes the formatted expression and result to stdout.

#include "calc.h"
#include "CLI.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define readsize 1024

int main(int argc, char *argv[]){
	 
	char input[readsize];
	long answer = 0; 
	char **outputArray;
	char operator;
	while (fgets(input, readsize, stdin) != NULL){

		// tokenize around operators	
		outputArray = tokenize(input, " +/-*");
		
		// checks what operator is, and performs proper calculation
	
		if (strstr(input, "+")){
			operator = '+'; 
			answer = strtol(outputArray[0], NULL, 10) + strtol(outputArray[1], NULL, 10);
			printf("%s %c %s = %ld\n", outputArray[0], operator, outputArray[1],  answer);
		}

		else if (strstr(input, "-")){
			operator = '-';
			answer = strtol(outputArray[0], NULL, 10) - strtol(outputArray[1], NULL, 10);
			printf("%s %c %s = %ld\n", outputArray[0], operator, outputArray[1],  answer);
		}

		else if (strstr(input, "*")){
			operator = '*';
			answer = strtol(outputArray[0], NULL, 10) * strtol(outputArray[1], NULL, 10);
			printf("%s %c %s = %ld\n", outputArray[0], operator, outputArray[1],  answer);
		}

		else if (strstr(input, "/")){
			operator = '/';
			answer = strtol(outputArray[0], NULL, 10) / strtol(outputArray[1], NULL, 10);
			printf("%s %c %s = %ld\n", outputArray[0], operator, outputArray[1],  answer);
		}

		else
			printf("Invalid input, please enter a simple calculation\n");
		
	}
	return 0;

}
