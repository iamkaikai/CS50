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

int total = 0;
typedef struct wordCountPair {                                                                     
    char* word;                                                                         
    int count;
	queue_t* webs;                                                                    
} wordCountPair_t;

void print_hash_element(void *p){
    if(p == NULL){
            printf("p is Null\n");
    }
    wordCountPair_t *qp = (wordCountPair_t *) p;
    char *current_word = qp->word;
	int current_count = qp->count;
    if(current_word != NULL){
            printf("word = %s\ncount=%d\n", current_word,current_count);
    }else if(current_word== NULL){
            printf("word\n");
    }else{
            printf("word error!\n");
    }
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
    char *skp = searchkeyp;
    return strcmp(ep, skp) == 0;
}

void createWordCountHash(hashtable_t* word_hash,char* word){
	if (word!=NULL){
		if(hsearch(word_hash,hsearch_word,word,strlen(word))){
			wordCountPair_t* old_wordCountPair = hremove(word_hash,hsearch_word,word,strlen(word));
			int count = old_wordCountPair->count;
			count+=1;
			old_wordCountPair->count = count;
			hput(word_hash,old_wordCountPair,old_wordCountPair->word,strlen(word));
			//free(word);
		}else{
			char *new_word = malloc(strlen(word)+1);
			int count = 1;
            strcpy(new_word, word);
			wordCountPair_t* new_wordCountPair = malloc(sizeof(wordCountPair_t));
			new_wordCountPair->word = new_word;
			new_wordCountPair->count = count;
			hput(word_hash,new_wordCountPair,new_word,strlen(new_word));
			//free(word);
		}
	}else{
		printf("NULL appears");
	}
}

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

int step2(void){
 
 webpage_t *page = pageload(1, "../pages/");
 char *word;
 int pos = 0;

 FILE *fp;
 fp = fopen("./1", "w");
 while( (pos = webpage_getNextWord(page, pos, &word)) > 0){ 

  // fprintf(fp, "%s\n", word);
 
  char *w = NormalizeWord(word); 
  if(w != NULL){
   fprintf(fp, "%s\n", word);
  }
  free(word);
 }
 fclose(fp);
 
 webpage_delete(page);
 return 0;
}

int main(void){
 
	webpage_t *page = pageload(1, "../pages/");
	char *word;
	int pos = 0;

	uint32_t hsize = 999;
    hashtable_t *word_hash = hopen(hsize);

	while( (pos = webpage_getNextWord(page, pos, &word)) > 0){ 
		// fprintf(fp, "%s\n", word);
		char *w = NormalizeWord(word); 
		if(w != NULL){
			createWordCountHash(word_hash,word);
			//free(w);
  		}
		free(word);
	}
	
	happly(word_hash,sumwords);

	happly(word_hash,print_hash_element);
	hclose(word_hash);
	printf("total words = %d\n",total);
	webpage_delete(page);
	return 0;
}


