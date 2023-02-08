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

/*int32_t pagesave(webpage_t *cur_page, int id, char *dirname){
	fetch
	}*/

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

int get_url(webpage_t *cur_page, int max, queue_t *url_queue, hashtable_t *hash){
	//get all the urls in the same layer
	int pos = 0;
	char *result;   //store urls crawled in current page
	int cur_depth = webpage_getDepth(cur_page);
	int next_depth = cur_depth+1;
	// webpage_t *new_page;
	void (*fn1)(queue_t *) = print_element;
	bool (*fn2)(void *elementp, const void* searchkeyp) = qsearch_url;
	bool fetch = webpage_fetch(cur_page);
		
	printf("current page = %p\n",(void*)cur_page);
        
	if(fetch){
		//check current page in hash
		url_search_and_hput(cur_page, hash);	
		while((pos = webpage_getNextURL(cur_page, pos, &result)) > 0 && next_depth <= max){	
			///webpage_t *new_page;
			if(IsInternalURL(result)){
				printf("Depth %d: Found URL (internal): %s\n",next_depth, result);
				//put URL in to the queue
				webpage_t *new_page = webpage_new(result, next_depth, NULL);   //how to free this?
				if( qsearch(url_queue, fn2, new_page) == false &&\
						url_search_and_hput(new_page, hash) == false)
				{
					qput(url_queue, new_page);
				}	
			}else{
				printf("Depth %d: Found URL (external): %s\n\n",next_depth, result);
			}
			// printf("pos = %d\n",pos);
			free(result);
			// webpage_delete(new_page);
		}
	}else{
		exit(EXIT_FAILURE);
	}

	printf("---------- Queue of URLs ------------\n");
	qapply(url_queue, fn1);
	printf("-------------------------------------\n\n");
	//webpage_delete(new_page);
	webpage_delete(cur_page);
	return cur_depth;
}

int32_t pagesave(webpage_t *pagep, int id, char *dirname) {
	FILE *fp;
	char path[64];
	char name[64];
	char depth[64];
	char html_len[64];
	char *url = webpage_getURL(pagep);
	char *html = webpage_getHTML(pagep);

	strcpy(path, dirname);	
	sprintf(name, "%d", id);
	sprintf(depth, "%d", webpage_getDepth(pagep));
	sprintf(html_len, "%d", webpage_getHTMLlen(pagep));
	
	fp = fopen(strcat(path,name), "w+");

	fprintf(fp, "%s\n%s\n%s\n%s\n", url, depth, html_len, html);
	
	fclose(fp);
	
	return 0;
}


int main(void){

	//setup up basic variables
	char seed[] = "https://thayer.github.io/engs50/";
	int max_depth = 1;
	int crawl_depth = 0;
	uint32_t hsize = 999;
	hashtable_t *url_hash  = hopen(hsize);
	queue_t *url_queue = qopen();

	//initiate the seed page
	webpage_t *seed_page = webpage_new(seed, 0, NULL);
	crawl_depth = get_url(seed_page, max_depth, url_queue, url_hash);   		
	
	//iterate through all the pages in the queue until it's empty
	void *qp = qget(url_queue);
	while(qp != NULL && crawl_depth <= max_depth){
		crawl_depth = get_url(qp, max_depth, url_queue, url_hash);
		qp = qget(url_queue);
	}
	
	//qclose(url_queue);
	//hclose(url_hash);
	exit(EXIT_SUCCESS);
}


