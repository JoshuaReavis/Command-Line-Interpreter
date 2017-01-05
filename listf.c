// Josh Reavis
// Summer 2016
// Created for CSCI 352 Unix Software Development

// Replacement Unix ls command. Lists the contents of one or more directories
// specified as cmmnad line arguments in alaphabetical order.


#include "listf.h"
#include "CLI.h"
#include "pipe.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <ctype.h>
#define readsize 1024

// compare function whicn takes two dirents and finds which ones name is
// lexographically more than the other
// can be used by scandir
int compare(const struct dirent **leftentry, const struct dirent **rightentry){
	char *left = strdup((*leftentry)->d_name);
	char *right = strdup((*rightentry)->d_name);
	int i = 0;
	while (left[i]){
		left[i] = tolower(left[i]);
		i++;
	}
	i = 0;
	while (right[i]){
		right[i] = tolower(right[i]);
		i++;
	}
	int compare = strcmp(left, right);
//	free(left);
//	free(right);
	return compare;

}

int main(int argc, char *argv[]){
	static int l = 0;
	static int m = 0;
	static int a = 0;
	static int c = 0;


	// goes through and sets the appropriate flags
	for (int i = 1; argv[i]; i++){
		if (argv[i][0] == '-'){
			for (int j = 0; argv[i][j]; j++){
				switch(argv[i][j]){
					case 'l' :
						l = 1;
						break;
					case 'm' :
						m = 1;
						break;
					case 'a' :
						a = 1;
						break;
					case 'c' :
						c = 1;
						break;
					default : 
						break;
				}




			}
		}
	}
	
	// flag for if we have no dir args
	static int nodir = 1;

	if (l == 1){

		for (int i = 1; argv[i]; i++){
			if (strncmp(argv[i], "-", 1) != 0){
				printf("%s\n", argv[i]);

				char cwdbuf[readsize];
				char storecurrentpath[readsize];
				char storepath[readsize];

				if (realpath(argv[i], storepath) != NULL){
					realpath(getcwd(cwdbuf, sizeof(cwdbuf)), storecurrentpath);
					nodir = 0;
					chdir(storepath);
					char cwdbuf[readsize];
					struct dirent **entry_list;
					
					// sorts using scandir and my compare function 
					int numfiles = scandir(getcwd(cwdbuf, sizeof(cwdbuf)), &entry_list, NULL, compare);
					if (numfiles < 0) {
						perror("scandir");
						return EXIT_FAILURE;
					}

					// go through all of our files and print out the information overlord Bover has required of us
					for (int i = 0; i < numfiles; i++){
						struct dirent *entry;
						entry = entry_list[i];
						
						// create an empty statbuf and fill it with appropriate information using the dirent name and lstat function
						struct stat statbuf;
						lstat(entry->d_name, &statbuf);

						// print the file type
						if (strncmp(entry->d_name, ".", 1) != 0){

							if (S_ISREG(statbuf.st_mode))
								printf("-");
							else if (S_ISDIR(statbuf.st_mode))
								printf("d");
							else if (S_ISLNK(statbuf.st_mode))
								printf("l");
							else
								printf("o");
							// print the permissions
							if ((statbuf.st_mode & S_IRUSR) != 0)
								printf("r");
							else
								printf("-");
							if ((statbuf.st_mode & S_IWUSR) != 0)
								printf("w");
							else
								printf("-");
							if ((statbuf.st_mode & S_IXUSR) != 0)
								printf("x");
							else
								printf("-");

							if ((statbuf.st_mode & S_IRGRP) != 0)
								printf("r");
							else
								printf("-");
							if ((statbuf.st_mode & S_IWGRP) != 0)
								printf("w");
							else
								printf("-");
							if ((statbuf.st_mode & S_IXGRP) != 0)
								printf("x");
							else
								printf("-");

							if ((statbuf.st_mode & S_IROTH) != 0)
								printf("r");
							else
								printf("-");
							if ((statbuf.st_mode & S_IWOTH) != 0)
								printf("w");
							else
								printf("-");
							if ((statbuf.st_mode & S_IXOTH) != 0)
								printf("x");
							else
								printf("-");
							printf(" ");
							
							//Number of links to file
							printf("%3ld ", statbuf.st_nlink);

							//Name of file owner
							struct passwd *passwdbuf = getpwuid(statbuf.st_uid);
							printf("%5s ", passwdbuf->pw_name);

							// name of file owner's group
							struct group *groupbuf = getgrgid(statbuf.st_gid);
							printf("%5s ", groupbuf->gr_name);

							// file size in bytes
							printf("%6li ", statbuf.st_size);
							
							// setup for being able to access our st_time in the correct format
							struct tm *mtimes;
							struct tm *atimes;
							struct tm *ctimes;
							
							char mbuf[readsize];
							char abuf[readsize];
							char cbuf[readsize];

							mtimes = localtime(&statbuf.st_mtime);
							atimes = localtime(&statbuf.st_atime);
							ctimes = localtime(&statbuf.st_ctime);

							strftime(mbuf, sizeof(mbuf), "M%m/%d/%y-%H:%M", mtimes);
							strftime(abuf, sizeof(abuf), "A%m/%d/%y-%H:%M", atimes);
							strftime(cbuf, sizeof(cbuf), "C%m/%d/%y-%H:%M", ctimes);

							// print proper dates for flags selected
							if (l == 1 && a == 0 && c == 0)
								printf("%s ", mbuf);

							if (l == 1 && a == 1 && c == 0 && m == 0){
								printf("%s ", abuf);
							}

							if (l == 1 && a == 0 && c == 1 && m == 0){
								printf("%s ", cbuf);
							}

							if (l == 1 && a == 1 && c == 0 && m == 1){
								printf("%s ", mbuf);
								printf("%s ", abuf);
							}

							if (l == 1 && a == 1 && c == 1 && m == 0){
								printf("%s ", abuf);
								printf("%s ", cbuf);		
							}

							if (l == 1 && a == 0 && c == 1 && m == 1){
								printf("%s ", mbuf);
								printf("%s ", cbuf);
							}


							if (l == 1 && a == 1 && c == 1 && m == 1){
								printf("%s ", mbuf);
								printf("%s ", abuf);
								printf("%s ", cbuf);
							}				

							// file name
							printf("%s\n", entry->d_name);
						}
					} // for
					printf("\n");
				chdir(storecurrentpath);	
				} // fi
			} // fi
		} // for

		// handles case if listf is given no dir
		if (nodir == 1){
			char cwdbuf[readsize];
			struct dirent **entry_list;
			printf("%s\n", getcwd(cwdbuf, sizeof(cwdbuf)));

			// sorts using scandir and my compare function
			int numfiles = scandir(getcwd(cwdbuf, sizeof(cwdbuf)), &entry_list, NULL, compare);
			if (numfiles < 0) {
				perror("scandir");
				return EXIT_FAILURE;
			}
		
			// go through all of our files and print out the information overlord Bover has required of us
			for (int i = 0; i < numfiles; i++){
				struct dirent *entry;
				entry = entry_list[i];

				// create an empty statbuf and fill it with appropriate information using the dirent name and lstat function
				struct stat statbuf;
				lstat(entry->d_name, &statbuf);
				if (strncmp(entry->d_name, ".", 1) != 0){
					
					// print the file type
					if (S_ISREG(statbuf.st_mode))
						printf("-");
					else if (S_ISDIR(statbuf.st_mode))
						printf("d");
					else if (S_ISLNK(statbuf.st_mode))
						printf("l");
					else
						printf("o");
					// print the permissions
					if ((statbuf.st_mode & S_IRUSR) != 0)
						printf("r");
					else
						printf("-");
					if ((statbuf.st_mode & S_IWUSR) != 0)
						printf("w");
					else
						printf("-");
					if ((statbuf.st_mode & S_IXUSR) != 0)
						printf("x");
					else
						printf("-");

					if ((statbuf.st_mode & S_IRGRP) != 0)
						printf("r");
					else
						printf("-");
					if ((statbuf.st_mode & S_IWGRP) != 0)
						printf("w");
					else
						printf("-");
					if ((statbuf.st_mode & S_IXGRP) != 0)
						printf("x");
					else
						printf("-");

					if ((statbuf.st_mode & S_IROTH) != 0)
						printf("r");
					else
						printf("-");
					if ((statbuf.st_mode & S_IWOTH) != 0)
						printf("w");
					else
						printf("-");
					if ((statbuf.st_mode & S_IXOTH) != 0)
						printf("x");
					else
						printf("-");
					printf(" ");	
					//Number of links to file
					printf("%3ld ", statbuf.st_nlink);

					//Name of file owner
					struct passwd *passwdbuf = getpwuid(statbuf.st_uid);
					printf("%5s ", passwdbuf->pw_name);

					// name of file owner's group
					struct group *groupbuf = getgrgid(statbuf.st_gid);
					printf("%5s ", groupbuf->gr_name);

					// file size in bytes
					printf("%6li ", statbuf.st_size);


					// setup for being able to access our st_time in the correct format
					struct tm *mtimes;
					struct tm *atimes;
					struct tm *ctimes;
					
					char mbuf[readsize];
					char abuf[readsize];
					char cbuf[readsize];

					mtimes = localtime(&statbuf.st_mtime);
					atimes = localtime(&statbuf.st_atime);
					ctimes = localtime(&statbuf.st_ctime);

					strftime(mbuf, sizeof(mbuf), "M%m/%d/%y-%H:%M", mtimes);
					strftime(abuf, sizeof(abuf), "A%m/%d/%y-%H:%M", atimes);
					strftime(cbuf, sizeof(cbuf), "C%m/%d/%y-%H:%M", ctimes);


				
					// dates
					if (l == 1 && a == 0 && c == 0)
						printf("%s ", mbuf);

					if (l == 1 && a == 1 && c == 0 && m == 0){
						printf("%s ", abuf);
					}

					if (l == 1 && a == 0 && c == 1 && m == 0){
						printf("%s ", cbuf);
					}

					if (l == 1 && a == 1 && c == 0 && m == 1){
						printf("%s ", mbuf);
						printf("%s ", abuf);
					}

					if (l == 1 && a == 1 && c == 1 && m == 0){
						printf("%s ", abuf);
						printf("%s ", cbuf);		
					}

					if (l == 1 && a == 0 && c == 1 && m == 1){
						printf("%s ", mbuf);
						printf("%s ", cbuf);
					}


					if (l == 1 && a == 1 && c == 1 && m == 1){
						printf("%s ", mbuf);
						printf("%s ", abuf);
						printf("%s ", cbuf);
					}

					// file name
					printf("%s\n", entry->d_name);
				}
			} //rof
				

		 }// fi
	} // fi

	nodir = 1;
	// general case for without l flag but we are given directory(ies)
	if (l == 0){
		char cwdbuf[readsize];
		
		// go through each of the args and if it is not a flag, perform the print on name
		for (int i = 1; argv[i]; i++){

			if (strncmp(argv[i], "-", 1) != 0){
				char storecurrentpath[readsize];
				char storepath[readsize];

				if (realpath(argv[i], storepath) != NULL){
					printf("%s\n", argv[i]);
					nodir = 0;
					realpath(getcwd(cwdbuf, sizeof(cwdbuf)), storecurrentpath);
					chdir(storepath);

					struct dirent **entry_list;
					
					// sort using scandir and my compare function
					int numfiles = scandir(getcwd(cwdbuf, sizeof(cwdbuf)), &entry_list, NULL, compare);
					if (numfiles < 0) {
						perror("scandir");
						return EXIT_FAILURE;
					}
					
					// print out the name of all of the files
					for (int i = 0; i < numfiles; i++){
						struct dirent *entry;
						entry = entry_list[i];
						if (strncmp(entry->d_name, ".", 1) != 0)
							printf("%s ", entry->d_name);
					}

				}
			chdir(storecurrentpath);
			}
		
		printf("\n");
		}
	}

	// general case without l flag or directories
	if (l == 0 && nodir){
		char cwdbuf[readsize];
		struct dirent **entry_list;
		printf("%s\n", getcwd(cwdbuf, sizeof(cwdbuf)));

		// sort scandir and my compare function
		int numfiles = scandir(getcwd(cwdbuf, sizeof(cwdbuf)), &entry_list, NULL, compare);
		if (numfiles < 0) {
			perror("scandir");
			return EXIT_FAILURE;
		}
		
		for (int i = 0; i < numfiles; i++){
			struct dirent *entry;
			entry = entry_list[i];

			if (strncmp(entry->d_name, ".", 1) != 0){
				printf("%s ", entry->d_name);
			}

		}
		printf("\n");
	}

	return 0;

}


