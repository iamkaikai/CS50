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
    char *word;                                                                         
    int count;
	queue_t *page_queue;                                                                    
} wordCountPair_t;

typedef struct idCountPair {                                                                     
    char *id;                                                                         
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

void print_queue_element(void* p){
    idCountPair_t *qp = (idCountPair_t *) p;
    char* current_id = qp->id;
	int current_count = qp->count;
    if(current_id != NULL){
        printf("id = %s\ncount=%d\n", current_id, current_count);
    }else{
        printf("word error!\n");
	}
}

void removeWordAndQueue(void *p){
    wordCountPair_t *qp = (wordCountPair_t *) p;
    char *current_word = qp->word;
	queue_t *current_queue = qp->page_queue;
	qclose(current_queue);
	free(current_word);
}

void sumwords(void *p){
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
    wordCountPair_t *qp = (wordCountPair_t *) p;
    char *current_word = qp->word;
		int current_count = qp->count;
		queue_t* current_queue = qp->page_queue;
		if(current_word != NULL){
			printf("word = %s %d\n", current_word,current_count);
			qapply(current_queue,print_queue_element);
		}else if(current_word== NULL){
			printf("word\n");
    }else{
			printf("word error!\n");
    }
}

void createWordCountHash(hashtable_t* word_hash,char* word){
	if (word!=NULL){
		//if the word already exist in the hash table
		if(hsearch(word_hash,hsearch_word,word,strlen(word))){
			wordCountPair_t *old_wordCountPair = hremove(word_hash,hsearch_word,word,strlen(word));
			old_wordCountPair->count += 1;  
			hput(word_hash,old_wordCountPair,old_wordCountPair->word,strlen(word));
		
		//if the word does not exist in the hash table
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
		printf("NULL appears\n");
	}
}

void modifyQWordCountHash(hashtable_t* master_word_hash, char* id){
	int pageid = atoi(id);	
	webpage_t *page = pageload(pageid, "../pages/");
	char *word;
	int pos = 0;
	queue_t *word_queue = qopen();
	uint32_t hsize = 999;
    hashtable_t *local_word_hash = hopen(hsize);

	//get all the words from a page and nomalize it
	while( (pos = webpage_getNextWord(page, pos, &word)) > 0){ 
		char *w = NormalizeWord(word); 
		if(w != NULL){
			if (qsearch(word_queue,qsearch_word,word)==NULL){
				char *new_word = malloc(strlen(word)+1);
            	strcpy(new_word, word);
				qput(word_queue,new_word);
			}
			//master hash table for word count
			createWordCountHash(master_word_hash,word);
			//local hash table for single page word count
			createWordCountHash(local_word_hash,word);
  		}
		free(word);
	}

	char *curr_word = qget(word_queue);
	while(curr_word!=NULL){
		
		wordCountPair_t *local_wordCountPair = hremove(local_word_hash,hsearch_word,curr_word,strlen(curr_word));
		wordCountPair_t *global_word_hash_node = hremove(master_word_hash,hsearch_word,curr_word,strlen(curr_word));
		
		idCountPair_t *new_idCountPair = malloc(sizeof(idCountPair_t));
		new_idCountPair->count = local_wordCountPair->count;
		new_idCountPair->id = id;

		qput(global_word_hash_node->page_queue, new_idCountPair);
		hput(master_word_hash,global_word_hash_node,global_word_hash_node->word,strlen(global_word_hash_node->word));
		free(local_wordCountPair->word);
		qclose(local_wordCountPair->page_queue);
		free(local_wordCountPair);
		free(curr_word);
		curr_word = qget(word_queue);
	}

	qclose(word_queue);
	happly(local_word_hash,removeWordAndQueue);
	hclose(local_word_hash);
	webpage_delete(page);
}

int main(void){
	uint32_t hsize = 999;
	hashtable_t *master_word_hash = hopen(hsize);
	char *id = "1";
	modifyQWordCountHash(master_word_hash,id);
	happly(master_word_hash,print_hash_element);
	happly(master_word_hash,removeWordAndQueue);
	hclose(master_word_hash);
	return 0;
}


