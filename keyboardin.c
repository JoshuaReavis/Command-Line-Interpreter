// Josh Reavis
// Summer 2016
// CSCI 352

// This program enhances the command line interpreter with dynamic command history capability.
// Used in conjuction with the history array passed in through CLI.c, betterfgets allows us to
// use up and down arrow keys in order to scroll through our command line history.

// Functions used in cmdhistory.c are part and parcel to running betterfgets. 

#include "ttymode.h"
#include "cmdhistory.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define readsize 1024

char *betterfgets(char **cmdhistory){
	tty_cbreak(STDIN_FILENO);

	int escflag = 0;
	int bracketflag = 0;
	int charecho = 1;
	char charinput;
	char *cmdinput = (char *)malloc((readsize)*sizeof(char));
	int numofchars = 0;
	int numhists = 0;
	for (int i = 0; cmdinput[i]; i++)
		cmdinput[i] = '\0';
	cmdinput[readsize-1] = '\0';

	while (cmdhistory[numhists])
		numhists++;
	int histposit = numhists;

	while ((charinput = getchar())){
		
		while (cmdhistory[numhists])
			numhists++;

		charecho = 1;

		// if the charinput is an escape key (indicating first part of up/down arrow)
		if (charinput == 27){
			escflag = 1;
			charecho = 0;
		}
		// if the charinput is '[' (indicating second part of up/down arrow)
		if (charinput == 91 && escflag){
			bracketflag = 1;
			charecho = 0;
		}
		// if the charinput is 'A' or 'B' (indicating third part of up/down arrow)
		if ((charinput == 65 || charinput == 66) && escflag && bracketflag){
			if (charinput == 65){
				// if we're not at the start of the history array, increment our position number down by 1
				// get rid of the current line and display the previous command in our history
				if (histposit >  0){
					histposit--;
					eraseline(&numofchars);
					uparrow(histposit, cmdhistory, &numofchars, cmdinput);
				}
				// if we're on the most oldest command, display the oldest command is up arrow is pressed again
				else{
					if (cmdhistory[histposit] != '\0'){
						eraseline(&numofchars);
						uparrow(histposit, cmdhistory, &numofchars, cmdinput);
					}
				}
				bracketflag = 0;			
				charecho = 0;

			}
			if (charinput == 66){
				// if we're not at the end of the history array, increment our position number up by 1
				// get rid of the current line and display the next command in our history
				if ((histposit < numhists-1)){
					histposit++;
					eraseline(&numofchars);
					downarrow(histposit, cmdhistory, &numofchars, cmdinput);
				}
				// if we're on the most recent command, simply delete the line
				else{
					eraseline(&numofchars);
				}
				bracketflag = 0;			
				charecho = 0;

			}
			escflag = 0;
		}
		// if the charinput is a backspace
		if (charinput == 8 || charinput == 127){
			// deletes last char
			if (numofchars != 0){
				// "\b \b" covers the operating systems where '\b' is non destructive
				putchar('\b');
				putchar(' ');
				putchar('\b');
			
				// make sure that our cmdinput string reflects this change
				numofchars--;
				cmdinput[numofchars] = '\0';
			}
			charecho = 0;

		}
		// if the charinput is Ctrl-D (EOT)
		if (charinput == 4){
			return "EOF";
		}
		// if the charinput is a newline (user hit enter)
		if (charinput == '\n'){
			// save cmd in history
			charecho = 0;
			putchar('\n');
			cmdinput[numofchars] = charinput;
			cmdhistory = addhist(cmdinput, cmdhistory);
			return cmdinput;
		}
		// adds the current char to the cmd line we are saving for history
		if (charecho){
			// will print out the bracket if the user is using tom foolery to pretend to use arrow command but actually wants to just type [ folllowed by a character
			if (bracketflag){
				cmdinput[numofchars] = '[';
				putchar('[');
				numofchars++;
				escflag = 0;
			}
			cmdinput[numofchars] = charinput;
			putchar(charinput);
			numofchars++;
		}
	}
	return " ";	
}

