#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>

int forkexecbash(char **commandarray);

int forkexecCLI(char **commandarray);

int cd(char **directoryarray);

int pwd();

char *concat(char *string1, char *string2);

void forkexecprepare(char *input, char *output, char *cwdswapbuf, char **tokensarray, char *CLIdir);
