// Joshua Reavis
// Summer 2016
// This program takes a string and splits it along delimiters, creating tokens inside of an array of strings

#include <stdio.h>
#include <stdlib.h>
#define readsize 1024

char **tokenize(char *line, char *delimiters){
	int num_token = 0;
	int prevIsDelim = 1;
	int isdelim = 0;
	int strbeg = -1;
	int strend = 0;
	int startquote = 0;

	// corectly finds the number of tokens
	for (int i = 0; line[i]; i++){ 

		// case if we find quote
		if (startquote){
			if (line[i] == '\"'){
				num_token++;
				startquote = 0;
			}
		}
		else if (line[i] == '\"' && !startquote){
			startquote = 1;	
		}


		// case if we dont find quote
		else{
			for (int j = 0; delimiters[j]; j++){
				if (line[i] == delimiters[j]){
					isdelim = 1;	
				}
			}
			
   
			if (line[i] == '\n')
				isdelim = 1;

			else if (isdelim == 1 && prevIsDelim == 0){
				num_token++;
				prevIsDelim = 1;
			}
			else if (isdelim == 1 && prevIsDelim == 1){
				prevIsDelim = 1;
			}
			else if (isdelim == 0){
				prevIsDelim = 0;
			}
			isdelim = 0;
		}
	}
	
	prevIsDelim = 1;
	isdelim = 0;
	char **finalArray = (char**)malloc((num_token+1)*sizeof(char *));
	finalArray[num_token] = NULL;
	int final_count = 0;
	int strleng = 0;
	startquote = 0;
	
	for (int i = 0; line[i]; i++){
		if (line[i] == '\"' && startquote){
			strend = i;
			strleng = strend - strbeg;
			finalArray[final_count] = (char*)malloc((strleng+1)*sizeof(char));

			for (int k = 0; k < (strleng+1); k++){
				finalArray[final_count][k] = line[strbeg + k];
			}
			finalArray[final_count][strleng+1] = '\0';

			final_count++;
			strend = 0;
			strbeg = -1;
			
			prevIsDelim = 1;
			startquote = 0;
		}
		else if (line[i] == '\"' && !startquote){
			startquote = 1;
			strbeg = i;
		}
		else if (startquote){
		}
		else{
			
			if (i == 0)
				strbeg = -1;
			for (int j = 0; delimiters[j]; j++){
				if (line[i] == delimiters[j])
					isdelim = 1;
			}
			if (line[i] == '\n')
				isdelim = 1;

			if (!isdelim && strbeg == -1) 
				strbeg = i;

			if (isdelim == 1 && prevIsDelim == 0 && strbeg != -1){
				strend = i-1;
				strleng = strend - strbeg;
				finalArray[final_count] = (char*)malloc((strleng+1)*sizeof(char));

				for (int k = 0; k < (strleng+1); k++){
					finalArray[final_count][k] = line[strbeg + k];
				}
				finalArray[final_count][strleng+1] = '\0';

				final_count++;
				strend = 0;
				strbeg = -1;
				//sneakytrick = 0;
				
				prevIsDelim = 1;
			}

			if (isdelim == 1 && prevIsDelim == 1){
				prevIsDelim = 1;
			}

			if (isdelim == 0){
				prevIsDelim = 0;
			}

			isdelim = 0;
		}
	//	isdelim = 0;


	}
	
	return finalArray;
}

// returns a count of the number of strings in the null-terminated
// array (excluding the null element)
int token_count(char **tokens){
	int count = 0;
	for (int i = 0; tokens[i]; i++)
		count++;
	return count;
}


