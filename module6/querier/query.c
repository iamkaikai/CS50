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
#include <webpage.h>
#include <hash.h>
#include <pageio.h>
#include <indexio.h>

#define MAXSIZE 100 

void print_queue_element(void* p){
    idCountPair_t *qp = (idCountPair_t *) p;
    char *current_id = qp->id;
	int current_count = qp->count;
    if(current_id != NULL){
		// printf("current_id = %s\n",current_id);
        printf("id = %s; count = %d\n", current_id, current_count);
    }else{
        printf("word error!\n");
	}
}

void print_hash_element(void *p){
    wordCountPair_t *qp = (wordCountPair_t *) p;
    char *current_word = qp->word;
		int current_count = qp->count;
		queue_t* current_queue = qp->page_queue;
		if(current_word != NULL){
			printf("%s: %d\n", current_word,current_count);
			qapply(current_queue,print_queue_element);
		}else if(current_word== NULL){
			printf("word\n");
    }else{
			printf("word error!\n");
    }
}

bool hsearch_word(void *elementp, const void* searchkeyp){
	wordCountPair_t* element = (wordCountPair_t*)elementp;
    char *ep = element->word;
    char *skp = (char*)searchkeyp;
    return strcmp(ep, skp) == 0;
}

int main(void){

    char input[MAXSIZE];
    char* token; 
    char* printArray[MAXSIZE];
    char delimiter[]=" \t"; 

    printf("> ");
    fgets(input,99,stdin);                          //input    
    token = strtok(input, delimiter);               //split the first word from input
    int qry_count = 0;                              //count how many words of input are there
    hashtable_t *index_hash = hopen(999);           //hash for index
    indexload("../pages/", "index",index_hash);     //load index

    while (input != NULL){
    
        int invalid = 0;                            //keep track of invalid input
        if (feof(stdin)) {
            printf("\n");
            return 0;
        }
	
        while(token != NULL){
            for(int i=0; i< strlen(token) && strcmp(&token[i],"\n"); i++) {
			    if (!isalpha(token[i])) {
				    printf("[invalid query]\n");
				    invalid = 1;
                    break;
			    }
			    token[i] = tolower(token[i]);
		    }
            printArray[qry_count] = token; 
            token = strtok(NULL, " "); 
            qry_count++;
		}
    
        if(invalid != 1){
            printf("1!!!!!!!!\n");
            for(int j=0; j<qry_count; j++){
                //if last word
                printf("j!!!!!!!!\n");   
                if(j == qry_count-1){ 
                    printf("2!!!!!!!!\n");           
                    if(strlen(printArray[j])>3 && strcmp(printArray[j], "and\n") != 0){
                         
                        printArray[strcspn(printArray[j], "\n")] = 0;
                        printf("%s %lu --------\n", printArray[j], strlen(printArray[j]));       //////this is for step1 grading
                        printf("3!!!!!!!!\n");

                        // if(hsearch(index_hash,hsearch_word,printArray[j],strlen(printArray[j])-1)){
                        //     printf("4!!!!!!!!\n");
                        //     printf("word '%s' found!\n", printArray[j]);
                        //     // happly(index_hash,print_hash_element);
                        // }
                    }
                }else{
                    printf("5!!!!!!!!\n");
                    if(strlen(printArray[j])>2 && strcmp(printArray[j], "and") != 0){
                        printf("6!!!!!!!!\n");
                        // printf("%s ", printArray[j]);       ////////this is for step1 grading
                        if(hsearch(index_hash,hsearch_word,printArray[j],strlen(printArray[j]))){
                            printf("7!!!!!!!!\n");
                            printf("word '%s' found!\n", printArray[j]);
                            // happly(index_hash,print_hash_element);
                        }           
                    }
                }
            }
        }

        // reset printArray and start a new line for a new input
        memset(printArray, 0, MAXSIZE*sizeof(char*));
        qry_count = 0;
		printf("\n> ");
   		fgets(input,99,stdin); 
  	 	token = strtok(input, delimiter); 
	}
    return 0;
}