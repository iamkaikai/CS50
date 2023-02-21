/* query.c --- 
 * 
 * 
 * Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
 * Created: Mon Feb 20 19:09:03 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <queue.h>

#define MAXSIZE 100 

int main(void){

    char input[MAXSIZE];
    char* token; 
    char* printArray[MAXSIZE];
    char delimiter[]=" \t"; 

    printf("> ");
    fgets(input,99,stdin);               //input    
    token = strtok(input, delimiter);    //split the first word from input

    int j = 0; 
    while (input != NULL){
        if (feof(stdin)) {
            printf("\n");
            return 0;
        }
	    while(token != NULL){
            for(int i=0; i< strlen(token) && strcmp(&token[i],"\n"); i++) {
			    if (!isalpha(token[i])) {
				    printf("[invalid query]\n");
				    return 1;
			    }
			    token[i] = tolower(token[i]);
		    }
		    printArray[j] = token; 
		    token = strtok(NULL, " ");
		    printf("%s ", printArray[j]);
		    j++;
		}
        // new line for new input
		printf("> ");
   		fgets(input,99,stdin); 
  	 	token = strtok(input, delimiter); 
	}
    return 0;
}