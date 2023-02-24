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
//global variables
queue_t* rankQ;
char* globalWord;
int globalRank = 0;
int array_len = 0;                                 //count the length of the queryArray
char *pages_path = "../../module5/pages/";   //location of your index page

typedef struct idNode {   
    int rank;                                                                     
    char* id;
    char* url;
    queue_t* wordCountQueue;
}idNode_t;

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

void print_innerQ(void* p){
    wordCountPair_t *qp = (wordCountPair_t *) p;
    char *current_word = qp->word;
	int current_count = qp->count;
    printf("word:%s count:%d \n",current_word,current_count);
}

void print_rankQ(void *p){
    idNode_t* element = (idNode_t*) p;
    printf("Rank = %d ",element->rank);
    printf("Page = %s :",element->id); 
    printf(" %s \n",element->url); 
}

void clean_Q_in_rankQ(void* p){
    wordCountPair_t* element = (wordCountPair_t*) p;
    free(element->word);
}
//subject to change
void clean_rankQ(void *p){
    idNode_t* element = (idNode_t*) p;
    qapply(element->wordCountQueue,clean_Q_in_rankQ);
    free(element->id);
    free(element->url);
    qclose(element->wordCountQueue);
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

bool qsearch_id(void *elementp, const void* searchkeyp){
	char* element = (char*)elementp;
    char *skp = (char*)searchkeyp;
    return strcmp(element, skp) == 0;
}

bool qsearch_rankQId(void *elementp, const void* searchkeyp){
    idNode_t* localIdNode = (idNode_t*) elementp;
    char* element = localIdNode->id;
    char *skp = (char*)searchkeyp;
    return strcmp(element,skp)==0;
}

void putId(void* p){
    idCountPair_t* icp = (idCountPair_t*)p;
    if(qsearch(rankQ,qsearch_rankQId,icp->id)==NULL){
        //possible leak
        idNode_t* localIdNode = malloc(sizeof(idNode_t));   //create a node to for id-based hash
        char* localId = malloc(sizeof(icp->id)+1);
        strcpy(localId,icp->id);
        char file_location[64];        
        sprintf(file_location, "%s%s", pages_path, icp->id);
        //printf("%s\n",file_location);
        FILE *fp1 = fopen(file_location, "r");
        char *url = malloc(sizeof(char)*256);
        fscanf(fp1, "%s\n", url);
        fclose(fp1);
        localIdNode->id = localId;
        localIdNode->url = url;
        localIdNode->wordCountQueue=qopen();
        qput(rankQ,localIdNode);                            //put the rank into a queue
    }
}

void putWordCount(void* p){
    idCountPair_t* icp = (idCountPair_t*)p;
    wordCountPair_t* localWordCountPair = malloc(sizeof(wordCountPair_t*)+4);
    char* localWord = malloc(sizeof(char)*(strlen(globalWord) + 1));
    strcpy(localWord,globalWord);
    localWordCountPair->word = localWord;
    int count =  icp->count;
    localWordCountPair->count = count;
    idNode_t* globalIdNode = qsearch(rankQ,qsearch_rankQId,icp->id);
    queue_t* globalQueue = globalIdNode->wordCountQueue;
    qput(globalQueue,localWordCountPair);
}

//for step3
void andGate(void* p){
    wordCountPair_t* localWordCountPair = (wordCountPair_t*) p;
    int count = localWordCountPair->count;
    globalRank += count;
}
void processRank(void* p){
    idNode_t* localIdNode = (idNode_t*) p;
    qapply(localIdNode->wordCountQueue,andGate);
    localIdNode->rank = globalRank;
    globalRank = 0;
}

void length_counter(void *p){
    array_len +=1;
}

int getQueueLength(queue_t *p){
    qapply(p, length_counter);
    return array_len;
}
bool formatScaner(queue_t *queryArray){
    queue_t *temp = qopen();
    char *s = qget(queryArray);
    int pos = 0;
    int end = getQueueLength(queryArray);
    int cur = 0;        //value: and =1; or = 2; others = 0;
    int prev = 0;
    bool result = true;
    while(s != NULL){
        int and = strcmp(s, "and");
        int or = strcmp(s, "or");
        if(and == 0){
            cur = 1;
        }else if(or == 0){
            cur = 2;
        }else{
            cur = 0;
        }
        //if and/or appears in the beginning or the end of the queue
        if((pos == 0 && cur == 1) || (pos == 0 && cur == 2) || \
           (pos == end && cur == 1) || (pos == end && cur == 2)){
            array_len = 0;
            result = false;
            break;
        //if and/or appears in the middle of the queue
        }else{
            if((prev == 1 && cur == 1) || (prev == 1 && cur == 2) ||\
               (prev == 2 && cur == 1) || (prev == 2 && cur == 2)){
                array_len = 0;
                result = false;
                break;
            }
        }
        prev = cur;
        pos++;
        qput(temp, s);  //put word back to the queue
        s = qget(queryArray);
    }
    // put word back
    char *w = qget(temp);
    while(w != NULL){
        qput(queryArray, w);
        w = qget(temp);
    }
    //free everthing
    free(s);
    qapply(temp,clean_query_array);
    qclose(temp);
    array_len = 0;
    return result;
}

void query_search_in_index(queue_t *queryQ, hashtable_t *h){
    char *s = qget(queryQ);
    int and, len;
    while(s != NULL){
        and = strcmp(s, "and");
        len = strlen(s);
        void *search = hsearch(h, hsearch_word, s, strlen(s));
        if( search != NULL && and !=0 && len >2){
            wordCountPair_t* wcp = (wordCountPair_t*)search;
            queue_t* page_queue = wcp->page_queue;
            globalWord = s; 
            qapply(page_queue,putId);               //get all the id
            qapply(page_queue,putWordCount);        //put value from index to idNode
            qapply(rankQ,processRank);              //count the rank for each idNode
        }
        free(s);
        s = qget(queryQ);    //keep interating all the words in queryArray
    }
}

int main(void){
    hashtable_t *index_hash = hopen(999);           //hash for index
    indexload(pages_path, "index",index_hash);     //load index
    char input[MAXSIZE];
    char* token; 
    char delimiter[]=" \t\n"; 
    queue_t *queryArray;
    queryArray = qopen();
    int c;                                          //word count to clean input
    int invalid = 0;                                //label for invalid input format

    printf("> ");                                   //accept input
    // scanf("%99[^\n]",input);
   
    while (true){
        rankQ = qopen();
        if (scanf("%99[^\n]", input) == EOF) {
            printf("\n");
            qapply(rankQ,clean_rankQ);
            qclose(rankQ);
            qclose(queryArray);
            happly(index_hash, removeWordAndQueue);
            hclose(index_hash);
            return 0;
        }
        token = strtok(input, delimiter); 
        while(token != NULL){
            for(int i=0; i< strlen(token) && strcmp(&token[i],"\n"); i++) {
			    if (!isalpha(token[i])) {
                    // printf("------------0-------------\n");
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
            // printf("------------1-------------\n");
            if(!formatScaner(queryArray)){
                // printf("------------2-------------\n");
                printf("[invalid query]\n");
                qclose(rankQ);
            }else{
                // printf("------------3-------------\n");
                printf("(query keyword): ");
                qapply(queryArray,print_query_array);
                printf("\n\n");
                query_search_in_index(queryArray, index_hash);
                qapply(rankQ,print_rankQ);
                qapply(rankQ,clean_rankQ);
                qclose(rankQ);
            }
        }else{
            // printf("------------4-------------\n");
            qapply(rankQ,clean_rankQ);
            qclose(rankQ);
        }
        // printf("------------5-------------\n");
        //clean the queryArray 
        clean_query_array(queryArray);
        qclose(queryArray);
        while ((c = getchar()) != '\n');
        invalid = 0;
        memset(input, 0, MAXSIZE*sizeof(char));
        
        //start a new input
        printf("\n> ");
   		scanf("%99[^\n]",input); 
        queryArray = qopen(); 
	}
    return 0;
}

