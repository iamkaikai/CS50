/* indexer.c --- 
 * 
 * 
 * Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
 * Created: Sun Feb 12 15:55:17 2023 (-0500)
 * Version: 1.0
 * 
 * Description: indexer for tse 
 * 
 */

# include <stdio.h>
# include <stdlib.h>         
# include <string.h>
# include <stdbool.h>
# include <webpage.h>
# include <queue.h>
# include <hash.h>
# include <pageio.h>
# include <ctype.h>

int total = 0;
typedef struct wordCountPair {                                                                     
    char* word;                                                                         
    int count;
	queue_t* page_queue;                                                                    
} wordCountPair_t;

typedef struct idCountPair {                                                                     
    char* id;                                                                         
    int count;                                                               
} idCountPair_t;

char* NormalizeWord(char *word){
//if the word is has more than 2 chars and is alphabet
	if(strlen(word)>=3){
		for(int i=0; i<strlen(word); i++){
		//if the word contains any non-alpha char return null
			if(isalpha(word[i]) == 0){
				return NULL;
			}
			word[i] = tolower(word[i]);
			}
	}else{
		return NULL;
	}
	return word;
}

void free_queue_element(void* p){
	if(p == NULL){
            printf("p is Null\n");
    }
    idCountPair_t *qp = (idCountPair_t *) p;
    char* current_id = qp->id;
    if(current_id != NULL){
        free(current_id);
    }
}

void print_queue_element(void* p){
	if(p == NULL){
            printf("p is Null\n");
    }
    idCountPair_t *qp = (idCountPair_t *) p;
    char* current_id = qp->id;
	int current_count = qp->count;
    if(current_id != NULL){
            printf("id = %s\ncount=%d\n", current_id,current_count);
    }else if(current_id != NULL){
            printf("word\n");
    }else{
            printf("word error!\n");
	}
}

void removeWordAndQueue(void *p){
	if(p == NULL){
            printf("p is Null\n");
    }
    wordCountPair_t *qp = (wordCountPair_t *) p;
    char *current_word = qp->word;
	queue_t* current_queue = qp->page_queue;
	qapply(current_queue,free_queue_element);
	qclose(current_queue);
	free(current_word);
}

void sumwords(void *p){
	if(p == NULL){
            printf("p is Null\n");
    }
    wordCountPair_t *qp = (wordCountPair_t *) p;
	int current_count = qp->count;
    total += current_count;
}

bool hsearch_word(void *elementp, const void* searchkeyp){
	wordCountPair_t* element = (wordCountPair_t*)elementp;
    char *ep = element->word;
    char *skp = (char*)searchkeyp;
    return strcmp(ep, skp) == 0;
}

bool qsearch_word(void *elementp, const void* searchkeyp){
	char* element = (char*)elementp;
    char* skp = (char*)searchkeyp;
    return strcmp(element, skp) == 0;
}
void print_hash_element(void *p){
    if(p == NULL){
            printf("p is Null\n");
    }
    wordCountPair_t *qp = (wordCountPair_t *) p;
    char *current_word = qp->word;
	int current_count = qp->count;
	queue_t* current_queue = qp->page_queue;
    if(current_word != NULL){
            printf("word = %s\ncount=%d\n", current_word,current_count);
			idCountPair_t* abc = qget(current_queue);
			char* id = abc->id;
			printf("%s\n",id);
			//qapply(current_queue,print_queue_element);
    }else if(current_word== NULL){
            printf("word\n");
    }else{
            printf("word error!\n");
    }
}

void createWordCountHash(hashtable_t* word_hash,char* word){
	if (word!=NULL){
		if(hsearch(word_hash,hsearch_word,word,strlen(word))){
			wordCountPair_t* old_wordCountPair = hremove(word_hash,hsearch_word,word,strlen(word));
			int count = old_wordCountPair->count;
			count+=1;
			old_wordCountPair->count = count;
			hput(word_hash,old_wordCountPair,old_wordCountPair->word,strlen(word));
		}else{
			char *new_word = malloc(strlen(word)+1);
			int count = 1;
            strcpy(new_word, word);
			wordCountPair_t* new_wordCountPair = malloc(sizeof(wordCountPair_t));
			queue_t* queue = qopen();
			new_wordCountPair->word = new_word;
			new_wordCountPair->count = count;
			new_wordCountPair->page_queue = queue;
			hput(word_hash,new_wordCountPair,new_word,strlen(new_word));
		}
	}else{
		printf("NULL appears");
	}
}

void modifyQWordCountHash(hashtable_t* master_word_hash, char* id){
	int pageid = atoi(id);
	//printf("%d",pageid);
	webpage_t *page = pageload(pageid, "../pages/");
	char *word;
	int pos = 0;
	queue_t *word_queue = qopen();
	uint32_t hsize = 999;
    hashtable_t *word_hash = hopen(hsize);

	while( (pos = webpage_getNextWord(page, pos, &word)) > 0){ 
		// fprintf(fp, "%s\n", word);
		char *w = NormalizeWord(word); 
		if(w != NULL){
			if (qsearch(word_queue,qsearch_word,word)==NULL){
				char *new_word = malloc(strlen(word)+1);
            	strcpy(new_word, word);
				qput(word_queue,new_word);
			}
			createWordCountHash(master_word_hash,word);
			createWordCountHash(word_hash,word);
  		}
		free(word);
	}
	char* curr_word = qget(word_queue);
	while (curr_word!=NULL){
		wordCountPair_t* curr_wordCountPair = hremove(word_hash,hsearch_word,curr_word,strlen(curr_word));
		wordCountPair_t* master_word_hash_node = hremove(master_word_hash,hsearch_word,curr_word,strlen(curr_word));
		queue_t* curr_queue = master_word_hash_node->page_queue;
		idCountPair_t* curr_idCountPair = malloc(sizeof(idCountPair_t));
		curr_idCountPair->count = curr_wordCountPair->count;
		curr_idCountPair->id = id;
		qput(curr_queue,curr_idCountPair);
		hput(master_word_hash,master_word_hash_node,master_word_hash_node->word,strlen(master_word_hash_node->word));
		free(curr_wordCountPair->word);
		free(curr_wordCountPair);
		free(curr_word);
		curr_word = qget(word_queue);
	}
	qclose(word_queue);
	hclose(word_hash);
	webpage_delete(page);
}

int main(void){
	uint32_t hsize = 999;
    hashtable_t *master_word_hash = hopen(hsize);
	char* id = "1";
	modifyQWordCountHash(master_word_hash,id);
	happly(master_word_hash,print_hash_element);
	happly(master_word_hash,removeWordAndQueue);
	hclose(master_word_hash);
	return 0;
}


