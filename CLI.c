// Josh Reavis
// Summer 2016
// Created for CSCI 352 Unix Software Development

// Shell Command Line Interpreter which can operate all of the bash commands
// as well as some commands I have made.
// This includes calc, listf, fref, cd, pwd, and exit.

// Calc is a basic calculator, listf is a replacement for the ls command and
// fref is a replacement for grep. See calc, listf, fref c files for more details.

// This CLI is capable of storing a history of commands (finitely many).
// It is also possible to edit a command in progress, but the left and right arrow keys do not function as bash normally does, rather one has to hit the backspace and retype from there.

// The CLI also supports piping between two processes, check pipe.c for more details.

#include "CLI.h"
#include <string.h>
#include "calc.h"
#include "tokenizer.h"
#include "keyboardin.h"
#include "ttymode.h"
#include "pipe.h"
#define readsize 1024
#define inout 40

int inputflag = 0;
int outputflag = 0;
int pipeflag = 0;

// custom concatenate function
char *concat(char *string1, char *string2){
	char *result = malloc(strlen(string1)+strlen(string2)+1);
	strcpy(result, string1);
	strcat(result, string2);
	return result;
}

// forkexec function for my own CLI cmds
// and that of bash cmds
// takes in our tokenized input
int forkexec(char **commandarray){
	// commence piping sequence
	if (pipeflag){
		int p[2];
		pipe(p);
		piped(commandarray, p);
	}
	// exec fork as normal if no pipe
	else{
		int childexit;
		pid_t child = fork();


		if (child < 0){
			return 1;
		}

		if (child == 0){
			char storepath[readsize];
			// checks to see if command is one of our own external cmds
			if ((strncmp(commandarray[0], "fref", 4) == 0) || (strncmp(commandarray[0], "listf", 5) == 0) || (strncmp(commandarray[0], "calc", 4) == 0)){
				realpath(commandarray[0], storepath);
				execvp(storepath, commandarray);
			}
			// we're using a bash cmd
			else{
				execvp(commandarray[0], commandarray);
			}
			return 0;
		}

		else{
			wait(&childexit);
			return 0;
		}
	}
	return 0;
}

// ground work done of setting up proper paths and preparing for I/O redirection so that we can properly forkexec
void forkexecprepare(char *input, char *output, char *cwdswapbuf, char **tokensarray, char *CLIdir){
	// open stream to files if flags
	if (inputflag)
		freopen(input, "r", stdin);
	if (outputflag)
		freopen(output, "w", stdout);
	
	// directory swap so we know where to call listf
	getcwd(cwdswapbuf, sizeof(cwdswapbuf));
	chdir(CLIdir);
	forkexec(tokensarray);
	chdir(cwdswapbuf);

	// reset stream
	if (inputflag)
		freopen("/dev/tty", "r", stdin);
	if (outputflag)
		freopen("/dev/tty", "w", stdout);
}

// simple cd function
// takes in our tokenized input of directories
int cd(char **directoryarray){
	char storepath[readsize];
	if (directoryarray[1] == NULL){
		chdir(getenv("HOME"));
		char cwdbuf[readsize];
		printf("cwd changed to %s\n", getcwd(cwdbuf, sizeof(cwdbuf)));
	}
	else if (realpath(directoryarray[1], storepath) != NULL){
		chdir(storepath);		
		printf("cwd changed to %s\n", storepath);
	}
	else
		printf("No such directory : %s\n", directoryarray[1]);
	return 0;
}

// simple pwd function
int pwd(){
	char cwdbuf[readsize];
	if (getcwd(cwdbuf, sizeof(cwdbuf)) != NULL)
		printf("%s\n", getcwd(cwdbuf, sizeof(cwdbuf)));
	return 0;
}

int main(int argc, char *argv[]){

char *bashinput = (char *)malloc((readsize)*sizeof(char));
char **tokensarray;
char **cmdhistory = (char **)malloc((readsize)*sizeof(char *));

// stores the absolute path to the CWD so that we get access all of our functions if we leave the directory
char CLIdir[readsize];
getcwd(CLIdir, sizeof(CLIdir));
char cwdswapbuf[readsize];

char *input = (char*)malloc((inout)*sizeof(char));
char *output = (char*)malloc((inout)*sizeof(char));	

	for (int i = 0; cmdhistory[i]; i++)
		cmdhistory[i] = '\0';
	printf("$> ");
	
	// loops through until ctrl D or exit
	while ((bashinput = betterfgets(cmdhistory))){
		tty_reset(STDIN_FILENO);
		pipeflag = 0;

	// We need to start off our tokenization differently if we have a pipe 
		if (strstr(bashinput, "|") != NULL){
			pipeflag = 1;
			tokensarray = tokenize(bashinput, "|");
			tokensarray[1] = concat(tokensarray[1], "\n");
		}
		// normal tokenization
		else
			tokensarray = tokenize(bashinput, " ");

		inputflag = 0;
		outputflag = 0;

		// check for input and output and set flags
		for (int i = 1; tokensarray[i]; i++){
			if (strlen(tokensarray[i]) > 1 && tokensarray[i][0] == '<'){
				inputflag = 1;
				input = tokensarray[i];
				strcpy(input, &input[1]);
			}
			else if (strlen(tokensarray[i]) == 1 && tokensarray[i][0] == '<'){
				inputflag = 1;
				input = tokensarray[i+1];
			}
			if (strlen(tokensarray[i]) > 1 && tokensarray[i][0] == '>'){
				outputflag = 1;
				output = tokensarray[i];
				strcpy(output, &output[1]);
			}
			else if (strlen(tokensarray[i]) == 1 && tokensarray[i][0] == '>'){
				outputflag = 1;
				output = tokensarray[i+1];
			}
		}
	
		if (strncmp(bashinput, "EOF", 3) == 0)
			exit(0);

		// run cd
		if ((strncmp(bashinput, "cd", 2) == 0) && !pipeflag)
			cd(tokensarray);

		// run pwd
		else if ((strncmp(bashinput, "pwd", 3) == 0) && !pipeflag)
			pwd(); 
		
		// exit
		else if (strncmp(bashinput, "exit", 4) == 0)
			exit(0);
		
		
		// interpet command and run as bash input or my own external function
		else{
			forkexecprepare(input, output, cwdswapbuf, tokensarray, CLIdir); 
		}
	
		printf("$> ");
	}
	return 0;	
}
