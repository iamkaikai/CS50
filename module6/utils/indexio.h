#pragma once

/* indexio.h --- 
 *  
 * Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
 * Created: Wed Feb 15 14:59:27 2023 (-0500)
 * Version: 1.0
 * 
 * Description: load a file that is already indexed, 
 * or save a index hashtable to a designated location 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>


typedef struct wordCountPair {
  char *word;                                                                         
  int count;
	queue_t *page_queue;                                                                    
}wordCountPair_t;

typedef struct idCountPair {
	char *id;                                                                         
  int count;                                                               
}idCountPair_t;

/*save the index to the destinated location "file_dir"*/
void indexsave(hashtable_t *hash, char *fileDir, char *fileName);

/*load the index file from the destinated location "file_dir"*/
int indexload(char *fileDir, char *fileName, hashtable_t *master_hash);
