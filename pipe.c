// Josh Reavis
// Summer 2016

// This program allows for the piping functionality of CLI.
// The parent process forks to two seperate children and establishes
// interprocess communication between them via half duplex piping.


#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tokenizer.h"
#define readsize 1024

// forks to left child of pipe and opens up the pipe for writing and closes read
int externalforkp1(char **cmdarray, int p[]){
	pid_t child = fork();

	if (child < 0){
		return 1;
	}

	if (child == 0){
		char storepath[readsize];
		// dup the write
		dup2(p[1], STDOUT_FILENO);
		// close read
		close(p[0]);

		// if we're using one of our own external cmds
		if ((strncmp(cmdarray[0], "fref", 4) == 0) || (strncmp(cmdarray[0], "listf", 5) == 0) || (strncmp(cmdarray[0], "calc", 4) == 0)){
			realpath(cmdarray[0], storepath);
			execvp(storepath, cmdarray);
		}
		// if we're using a bash cmd
		else{
			execvp(cmdarray[0], cmdarray);
		}
	}
	return 0;
}

// forks to the right child of the pipe and opens up the pipe for reading and closes write
int externalforkp2(char **cmdarray, int p[]){
	pid_t child = fork();

	if (child < 0){
		return 1;
	}

	if (child == 0){
		char storepath[readsize];
		// dup the read
		dup2(p[0], STDIN_FILENO);
		// close write
		close(p[1]);
		// if we're using one of our own external cmds
		if ((strncmp(cmdarray[0], "fref", 4) == 0) || (strncmp(cmdarray[0], "listf", 5) == 0) || (strncmp(cmdarray[0], "calc", 4) == 0)){
			realpath(cmdarray[0], storepath);
			execvp(storepath, cmdarray);
		}
		// if we're using a bash cmd
		else{
			execvp(cmdarray[0], cmdarray);
		}
	}
	return 0;
}

// pipe command which calls two helper functions to fork two children of the same parent
// pipe array is initiliazed before this function is called and passed through
int piped(char **cmdarray, int p[]){
	if (strncmp(cmdarray[0], "cd", 2) == 0)
		printf("This program does not have cd piping functionality.\n");	

	else if (strncmp(cmdarray[0], "pwd", 3) == 0)
		printf("This program does not have pwd piping functionality.\n");	

	else{
		// fork the two children
		externalforkp1(tokenize(cmdarray[0], " "), p);
		externalforkp2(tokenize(cmdarray[1], " "), p);
		
		// close any file descriptors for the parent
		close(p[0]);
		close(p[1]);

		// wait for our two children
		wait(NULL);
		wait(NULL);
	}
	return 0;
}
