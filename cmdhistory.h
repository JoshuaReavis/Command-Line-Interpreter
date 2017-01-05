char **addhist(char *newcmdline, char **historycmds);

void uparrow(int position, char **historycmds, int *numchars, char *cmdinput);

void downarrow(int position, char **historycmds, int *numchars, char *cmdinput);

void eraseline(int *numchars);
