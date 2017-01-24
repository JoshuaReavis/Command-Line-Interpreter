// Josh Reavis
// Summer 2016

// This program allows for the up and down arrow functionalities which display
// previous command lines entered by the user, adds individual command lines into
// the history, and contains the eraseline function to erase an entire line.

// Functions in this program are called in keyboardin.c

#include "cmdhistory.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define readsize 1024

// adds the last command to the end of our history array of commands
char **addhist(char *newcmdline, char **historycmds){
	int i = 0;
	while (historycmds[i]){
		i++;
	}
	historycmds[i] = newcmdline;
	return historycmds;
}

// takes in the current position in history 
// via incrementing to the next older command right before the call to up arrow
// and displays that command 
void uparrow(int position, char **historycmds, int *numchars, char *cmdinput){
	*numchars = 0;
	for (int i = 0; historycmds[position][i] && historycmds[position][i] != '\n'; i++){
		putchar(historycmds[position][i]);
		cmdinput[*numchars] = historycmds[position][i];
		(*numchars)++;
	}
}

// takes in the current position in the history 
// via incrementing to next most recent command right before the call to down arrow
// and displays that command
void downarrow(int position, char **historycmds, int *numchars, char *cmdinput){
	*numchars = 0;
	for (int i = 0; historycmds[position][i] && historycmds[position][i] != '\n'; i++){	
		putchar(historycmds[position][i]);
		cmdinput[*numchars] = historycmds[position][i];
		(*numchars)++;
	}
}

// erases the entire line given how many chars are currently in it
void eraseline(int *numchars){
	while(*numchars != 0){
		putchar('\b');
		putchar(' ');
		putchar('\b');
		(*numchars)--;
	}
}
