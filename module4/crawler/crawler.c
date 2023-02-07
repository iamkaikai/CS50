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
		printf("URL = %s\n", url);
	}else{
		printf("queue is empty\n");
	}
	free(url);
}

bool search_url(void *elementp, const void* searchkeyp){
	char *ep = (char *)elementp;
	char *skp = (char *)searchkeyp;
	return strcmp(ep, skp) == 0;
}


void get_url(int depth, webpage_t *page, queue_t *url_queue){
	//get all the urls in the same layer
	int pos = 0;
	char *result;
		
	while((pos = webpage_getNextURL(page, pos, &result)) > 0){
				//check whether the URL is internal
				if(IsInternalURL(result)){
					printf("Found URL (internal): %s\n",result);
					//put URL in to the queue
					webpage_t *new_page;
					new_page = webpage_new(result, depth, NULL);
					qput(url_queue, new_page);
				}else{
					printf("Found URL (external): %s\n\n",result);
				}
				//free the URL for each iteration
				free(result);
	}
}

int main(void){

	printf("Hello\n");
	char seed[] = "https://thayer.github.io/engs50/";
	int depth = 0;
	webpage_t *page = webpage_new(seed, depth, NULL);;
	void (*fn1)(queue_t *) = print_element;
	//void (*fn2)(queue_t *) = put_url_in_hash;

	//if seed page fails
	if(page == NULL){
		printf("webpage_new error!\n");

	//if seed page initialization succeed, fetch the page
	}else{
		printf("webpage_new succeed!\n");
		
		if(webpage_fetch(page)){
			printf("webpage_fetch succeed!\n");

			//get all the URL in the seed page
			queue_t *url_queue = qopen();
			
			//getting url in designated page and put them in url_queue
			get_url(depth, page, url_queue);

			//open a hash to store url
			uint32_t hsize = 500;    //set the size of the hash table to 500
			hashtable_t *hash  = hopen(hsize);
			
			printf("---------- Queue of URLs ------------\n");
			qapply(url_queue, fn1);

			printf("\n---------- Hash of URLs ------------\n");
			depth +=1;
			void *qp = qget(url_queue);
			while(qp != NULL){

				webpage_t *wqp = (webpage_t *) qp;
				char *url = webpage_getURL(wqp);				
				long  url_len = strlen(url);
				printf("URL = %s\n", url);
 				get_url(depth, wqp, url_queue);
				if(hsearch(hash, search_url, url, url_len)==NULL){
					printf("Adding URL to hash table!\n");
 					hput(hash, url, url, url_len);
 				}else{	
					printf("URL existed!\n");
				}

				qp = qget(url_queue);
				free(url);
			}

			printf("\n---------- Queue updates ------------\n");
			qapply(url_queue, fn1);
			qclose(url_queue);
			
		//if fetch failed, exit	
		}else{
			exit(EXIT_FAILURE);
		}
	}


	webpage_delete(page);
	exit(EXIT_SUCCESS);
}


