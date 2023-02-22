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
        printf("word = %s - total %d\n", current_word,current_count);
        qapply(current_queue,print_queue_element);
        printf("------------------\n");
    }else if(current_word== NULL){
        printf("word\n");
    }else{
			printf("word error!\n");
    }
}

void print_query_array(void *p){
    char *qp = (char *) p;
    printf("%s ",qp);
}

void clean_query_array(queue_t *p){
    void *q = qget(p);
    while(( q != NULL)){
        free(q);
        q = qget(p);
    }
}

void clean_idCountpair_queue(void *p){
    idCountPair_t *qp = (idCountPair_t *) p;
    char *id = qp->id;
    free(id);
}

void removeWordAndQueue(void *p){
    wordCountPair_t *qp = (wordCountPair_t *) p;
    char *current_word = qp->word;
	queue_t *current_queue = qp->page_queue;
    qapply(current_queue, clean_idCountpair_queue);
	qclose(current_queue);
	free(current_word);
}

bool hsearch_word(void *elementp, const void* searchkeyp){
	wordCountPair_t* element = (wordCountPair_t*)elementp;
    char *ep = element->word;
    char *skp = (char*)searchkeyp;
    return strcmp(ep, skp) == 0;
}

void query_search_in_index(queue_t *p, hashtable_t *h){
    char *s = qget(p);
    int and, len, count;
    while(s != NULL){
        and = strcmp(s, "and");
        len = strlen(s);
        void *search = hsearch(h, hsearch_word, s, strlen(s));
        if( search != NULL && and !=0 && len >2){
            wordCountPair_t* wcp = (wordCountPair_t*)search;
            count = wcp->count;
            printf("%s: %d ", s, count);
        }
        s = qget(p);    //keep interating all the words in queryArray
    }
}

int main(void){
    hashtable_t *index_hash = hopen(999);           //hash for index
    indexload("../pages/", "index",index_hash);     //load index
    char input[MAXSIZE];
    char* token; 
    char delimiter[]=" \t\n"; 
    queue_t *queryArray;
    int c;
    int invalid = 0;

    printf("> ");
    scanf("%99[^\n]",input);
    queryArray = qopen();

    while (input != NULL){
       
        
        if (scanf("%99[^\n]", input) == EOF) {
            printf("\n");
            qclose(queryArray);
            happly(index_hash, removeWordAndQueue);
            hclose(index_hash);
            return 0;
        }
        
         token = strtok(input, delimiter); 

	    while(token != NULL){
            for(int i=0; i< strlen(token) && strcmp(&token[i],"\n"); i++) {
			    if (!isalpha(token[i])) {
				    printf("[invalid query]\n");
				    invalid = 1;
                    break;
			    }
			    token[i] = tolower(token[i]);
		    }
            char *s = malloc(sizeof(char)*(strlen(token) + 1));
            strcpy(s, token);
		    qput(queryArray,s);
            token = strtok(NULL, " ");
		}
        if(invalid != 1){
            printf("query keyword: ");
            qapply(queryArray,print_query_array);
            printf("\n");
            query_search_in_index(queryArray, index_hash);
        }
        
        //clean the queryArray 
        clean_query_array(queryArray);
        qclose(queryArray);
        while ((c = getchar()) != '\n');
        invalid = 0;
        memset(input, 0, MAXSIZE*sizeof(char*));
        
        //start a new input
        printf("\n> ");
   		scanf("%99[^\n]",input); 
  	 	token = strtok(input, delimiter);
        queryArray = qopen(); 
	}
    qclose(queryArray);
    hclose(index_hash);
    return 0;
}