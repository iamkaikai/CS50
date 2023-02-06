
/* crawler.c --- 
1;95;0c * 
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
# include <webpage.h>
# include <queue.h>
# include <hash.h>

void print_element(void *p){
  webpage_t *qp = (webpage_t *) p;
	char *url = webpage_getURL(qp);
	printf("URL = %s\n", url);
	//free(url);
}

void put_url_in_hash(void *p){
  webpage_t *qp = (webpage_t *) p;
	char *url = webpage_getURL(qp);
	long  url_len = strlen(url);
	printf("URL = %s\n", url);   
	printf("URL length = %ld\n",url_len);  
	//hput(hash, , url, url_len);
	//free(url2);
}

int main(void){

	printf("Hello\n");
	char seed[] = "https://thayer.github.io/engs50/";
	int depth = 0;
	webpage_t *page = webpage_new(seed, depth, NULL);;
	void (*fn1)(queue_t *) = print_element;
	void (*fn2)(queue_t *) = put_url_in_hash;

	//if seed page fails
	if(page == NULL){
		printf("webpage_new error!\n");

	//if seed page initialization succeed, fetch the page
	}else{
		printf("webpage_new succeed!\n");
		
		if(webpage_fetch(page)){
			printf("webpage_fetch succeed!\n");

			//get all the URL in the seed page
			int pos = 0;
			char *result;
			queue_t *url_queue = qopen();
			depth +=1;
			uint32_t hsize = 1000;
			hashtable_t *hash = hopen(hsize);
			
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
			printf("---------- Queue of URLs ------------\n");
			qapply(url_queue, fn1);
			printf("---------- Hash of URLs ------------\n");
			qapply(url_queue, fn2);
			qclose(url_queue);
			
		//if fetch failed, exit	
		}else{
			exit(EXIT_FAILURE);
		}
	}


	webpage_delete(page);
	exit(EXIT_SUCCESS);
}
