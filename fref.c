// Josh Reavis
// Summer 2016

// Utility similar to grep which will search through one or more files
// and report any lines in those files that contain a string matching
// a specified regular expression.
// Utilizes the functions defined in regexp.c

#include "fref.h"
#include "CLI.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "regexp.h"
#define readsize 1024
#define inputsize 60

int main(int argc, char *argv[]){

char *commandargs = (char*)malloc((inputsize)*sizeof(char));
char *regexp = (char*)malloc((inputsize)*sizeof(char));
char **tokensarray;

char input[readsize];

int noargs = 0;
int start = 2;

	// if less than two args, prompt user for regular exp and filelist, grab it from input, and set noargs flag
	if (argc < 2){
		printf("Not enough arguments, please give a regular expression and filelist\n");
		fgets(commandargs, readsize, stdin);
		noargs = 1;
	}
	// otherwise proceed as normal grabbing args and puttin them into a single char *
	else{
		// put the args onto a char * so that we may tokenize
		for (int i = 0; argv[i]; i++){
			strcat(commandargs, argv[i]);
			strcat(commandargs, " ");
		}
	}
	
	// tokenizes the commandargs, respects quote statements as one token
	tokensarray = tokenize(commandargs, " ");
	free(commandargs);
	
	// grab correct token for regexp based off of noargs flag
	if (noargs)
		strncpy(regexp, &tokensarray[0][1], strlen(tokensarray[0])-2);
	else
		strncpy(regexp, &tokensarray[1][1], strlen(tokensarray[1])-2);


	if (noargs)
		start = 1;

	// run the match on all of our files given by user
	for (int i = start; tokensarray[i]; i++){

		// open appropriate file
		FILE *fp = fopen(tokensarray[i], "r");

		// check if file was found, if so grab each line and perform a match
		// if not let the user know
		if (fp == NULL)
			printf("File %s not found.", tokensarray[i]);
		else{

			int counter = 0;
			while (fgets(input, sizeof(input), fp)){
				counter++;
				if (match(regexp, input)){
					printf("%s:%d %s", tokensarray[i], counter, input);
				}
			}
			fclose(fp);
		}
	}
	
	// free everyone just like abraham lincoln
	free(regexp);
	for (int i = 0; tokensarray[i]; i++)
		free(tokensarray[i]);
	free(tokensarray);
	
	return 0;

}
