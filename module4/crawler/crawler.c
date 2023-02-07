/* crawler.c --- 1;95;0c * 
 * 
 * Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
 * Created: Fri Feb  3 22:15:27 2023 (-0500)
 * Version: 1.0
 * 
 * Description: tiny search engine in module 4, grounp 2 in W23 CS50 
 * 
 */

# include <stdio.h>
# include <stdlib.h>   
# include <string.h>
# include <stdbool.h>
# include <webpage.h>
# include <queue.h>
# include <hash.h>

void print_element(void *p){
  webpage_t *qp = (webpage_t *) p;
	char *url = webpage_getURL(qp);
	if(url != NULL){
		printf("%p; URL = %s\n",p , url);
	}else{
		printf("queue is empty\n");
	}
	//	free(url);
}

bool hsearch_url(void *elementp, const void* searchkeyp){
	char *ep = (char *)elementp;
	char *skp = (char *)searchkeyp;
	return strcmp(ep, skp) == 0;
}

bool qsearch_url(void *elementp,const void* keyp){
	char *ep = (char *)webpage_getURL((webpage_t*)elementp);
	char *skp = (char *)webpage_getURL((webpage_t *)keyp);
	return strcmp(ep, skp) == 0;
}

bool url_search_and_hput(webpage_t *page, hashtable_t *hash){
	char *url = webpage_getURL(page);
	long url_len = strlen(url);
	if(hsearch(hash, hsearch_url, url, url_len) == NULL){
	  hput(hash, url, url, url_len);
		//printf("> added in hash!\n");
		return false;
	}else{
		//printf("> already existed in hash\n");
		return true;
	}
}

void get_url(webpage_t *cur_page, queue_t *url_queue, hashtable_t *hash){
	//get all the urls in the same layer
	int pos = 0;
	char *result;   //store urls crawled in current page
	int cur_depth = webpage_getDepth(cur_page);
	int next_depth = cur_depth+1;
	void (*fn1)(queue_t *) = print_element;
	bool (*fn2)(void *elementp, const void* searchkeyp) = qsearch_url;
	printf("current page = %p\n",(void*)cur_page);
        
	if(webpage_fetch(cur_page)){

		//check current page in hash
		url_search_and_hput(cur_page, hash);
					
		while((pos = webpage_getNextURL(cur_page, pos, &result)) > 0){
				
				if(IsInternalURL(result)){
					printf("Depth %d: Found URL (internal): %s\n",next_depth, result);
					//put URL in to the queue
					webpage_t *new_page = webpage_new(result, next_depth, NULL);
					if( qsearch(url_queue, fn2, new_page) == false &&\
							url_search_and_hput(new_page, hash) == false){
						//printf("put in queue!\n");
						qput(url_queue, new_page);
					}else{
						//			printf("> already in queue!\n");
					}
					//check whether current page is in hash table, if not put it in
										
				}else{
					printf("Depth %d: Found URL (external): %s\n\n",next_depth, result);
				}

				free(result);
		}
	}else{
		exit(EXIT_FAILURE);
	}

	printf("---------- Queue of URLs ------------\n");
	qapply(url_queue, fn1);
	printf("-------------------------------------\n\n");
}

int main(void){

	//setup up basic variables
	char seed[] = "https://thayer.github.io/engs50/";
	int depth = 0;
	uint32_t hsize = 500;
	hashtable_t *url_hash  = hopen(hsize);
	queue_t *url_queue = qopen();
	//int count = 0;
	//int count_limit = 200;

	//initiate the seed page
	webpage_t *seed_page = webpage_new(seed, depth, NULL);;
	get_url(seed_page, url_queue, url_hash);   		

	//iterate through all the pages in the queue until it's empty
	void *qp = qget(url_queue);
	while(qp != NULL){
		get_url(qp, url_queue, url_hash);
		qp = qget(url_queue);
	}
	
	//qclose(url_queue);
	//hclose(url_hash);
	webpage_delete(seed_page);
	exit(EXIT_SUCCESS);
}


