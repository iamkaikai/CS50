/* indexio.c --- 
 * 
 * 
 * Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
 * Created: Tue Feb 14 11:45:15 2023 (-0500)
 * Version: 1.0
 * 
 * Description: 
 * 
 */
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <string.h> 
# include <hash.h>
# include <queue.h>
# include <indexio.h>

FILE *fp;

void save_queue_element(void *p){
    idCountPair_t *qp = (idCountPair_t *) p;
    char* current_id = qp->id;
	int current_count = qp->count;
    if(current_id != NULL){
			fprintf(fp, " %s %d", current_id, current_count);
    }
}

void save_hash_element(void *p){
    wordCountPair_t *qp = (wordCountPair_t *) p;
    char *current_word = qp->word;
		//int current_count = qp->count;
		queue_t* current_queue = qp->page_queue;
		if(current_word != NULL){
			fprintf(fp, "%s", current_word);
			qapply(current_queue,save_queue_element);
			fprintf(fp, "\n");
		}
}

void indexsave(hashtable_t *master_hash, char *fileDir, char *fileName){
	char file_location[256];
  	sprintf(file_location, "%s%s", fileDir, fileName);
	fp = fopen(file_location, "w");
	if(fp == NULL){
		printf("unable to create file!!\n");
	}else{
		happly(master_hash, save_hash_element);
	}
	fclose(fp);
}

int indexload(char *fileDir, char *fileName, hashtable_t *master_hash){
	char file_location[256];
	sprintf(file_location, "%s%s", fileDir, fileName);
	char line[1024];
	fp = fopen(file_location, "r");

	if(fp == NULL){
		printf("unable to locate index directory or index file!!\n");
		return -1;
	}else{
		//read each line and create a wordCountPair
		while(fgets(line, sizeof(line), fp)){
            
			int total_count = 0;
			wordCountPair_t *new_wordCountPair = malloc(sizeof(wordCountPair_t));
			queue_t* queue = qopen();
			char *word = malloc(sizeof(char)*128);	// word in wordCountPair_t
			char *token = strtok(line, " ");		//get the first word
			strcpy(word, token); 					
			new_wordCountPair->word = word;
			
			while( token != NULL ){
	
				//create idCountPair_t to store value in the queue of wordCountPair
				//char *id = malloc(sizeof(char)*4);  	// int total_word;
				idCountPair_t *new_idCountPair = malloc(sizeof(idCountPair_t));
				token = strtok(NULL, " \n");
				
				//the strtok() will return a NULL pointer at the end of each line
				//need to check again to make sure we don't store NULL as the value
				if(token != NULL){
					char *id = malloc(sizeof(char)*4);
					strcpy(id, token);  
					// printf("id get = %s\n",id);          
					new_idCountPair->id = id;	

					token = strtok(NULL, " \n");
					char *id_count = malloc(sizeof(char)*4+1);
					strcpy(id_count, token);
					int count = strtol(id_count, NULL, 10);
					new_idCountPair->count = count;
					qput(queue,new_idCountPair);
					total_count += count;
					free(id_count);
				}else{
					free(new_idCountPair);
					//free(id);
				}
			}
			
			new_wordCountPair->page_queue = queue;
			new_wordCountPair->count = total_count;
			hput(master_hash,new_wordCountPair,word,strlen(word));
        };
	}
	fclose(fp);	
	return 0;
}
