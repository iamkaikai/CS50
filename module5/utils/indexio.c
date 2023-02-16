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

hashtable_t indexload(char *fileDir, char *fileName, hashtable_t *master_hash){

	char file_location[256];
	sprintf(file_location, "%s%s", fileDir, fileName);
	char line[1024];

	fp = fopen(file_location, "r");
	if(fp == NULL){
		printf("unable to create file!!\n");
	}else{
		//read each line and create a wordCountPair
		while(fgets(line, sizeof(line), fp)){
            
			//initialization
			wordCountPair_t *new_wordCountPair = malloc(sizeof(wordCountPair_t));
			queue_t* queue = qopen();
			
			//variable initialization
			char *word = malloc(sizeof(char)*128);
			int total_word;
			char *id = malloc(sizeof(char)*4);
  			int id_count;

			//split each word by space, get the first word
			char *token = strtok(line, " ");
			strcpy(word, token);
			int cur_pos = 1;
			int first = 1; 	
			new_wordCountPair->word = word;
			
			//create one/multiple idCountPair_t to store value in the queue of wordCountPair
			idCountPair_t *new_idCountPair = malloc(sizeof(idCountPair_t));
			while( token != NULL){
				
				//if there are more than one idCountPair_t, create a new one
				first = 0;
				if(first == 0){

				}
				//differentiate the 'ID' and 'count' base on the sequence
				cur_pos +=1;
				token = strtok(NULL, " ");
				
				//before reaching the end of the string
				if(token != NULL){	
					if(cur_pos %2 == 0){
						strcpy(id, token);
						printf("%s\n", id);
						new_idCountPair->id = id;
						
					}else{
						char *id_count_temp = malloc(sizeof(char)*4);
						strcpy(id_count_temp, token);
						int val = strtol(id_count_temp, NULL, 10);
						id_count = val;
						new_idCountPair->count = id_count;
						total_word += id_count;
						qput(new_wordCountPair->page_queue,new_idCountPair);
					
						free(id_count_temp);
						
					}
				}

			}
			
			new_wordCountPair->count = count;
			new_wordCountPair->page_queue = queue;
			hput(word_hash,new_wordCountPair,new_word,strlen(new_word));
        }
		// fscanf(fp, "%s\n", line);
	}
	fclose(fp);

	// free(word);
	// free(id);
	
}
