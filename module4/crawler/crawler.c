/* crawler.c --- 1;95;0c * 
* 
* Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
* Created: Fri Feb     3 22:15:27 2023 (-0500)
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
    if(p == NULL){
            printf("p is Null\n");
    }
    webpage_t *qp = (webpage_t *) p;
    char *url = webpage_getURL(qp);
    if(url != NULL){
            printf("%p; URL = %s\n",p , url);
    }else if(url == NULL){
            printf("queue is empty\n");
    }else{
            printf("queue error!\n");
    }
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
    fprintf(fp, "%s\n%s\n%s\n%s\n", url,depth,html_len, html);
    fclose(fp);
    return 0;
}
bool hsearch_url(void *elementp, const void* searchkeyp){
    char *ep = (char *)elementp;
    char *skp = (char *)searchkeyp;
    return strcmp(ep, skp) == 0;
}

bool qsearch_url(void *elementp,const void* keyp){
    char *ep = webpage_getURL((webpage_t *)elementp);
    // printf("%s\n",ep);
    char *skp = webpage_getURL((webpage_t *)keyp);
    return strcmp(ep, skp) == 0;
}

bool url_search_and_hput(webpage_t *page, hashtable_t *hash){
    char *url = webpage_getURL(page);
    char *h_url = malloc(strlen(url) + 1);
    strcpy(h_url, url);
    int h_url_len = strlen(h_url);
    bool result;
    if(hsearch(hash, hsearch_url, h_url, h_url_len) == NULL){
        hput(hash, h_url, h_url, h_url_len);
        result = false;
    }else{
        result = true;
    }
    return result;
}


int get_url(webpage_t *cur_page, int max, queue_t *url_queue, hashtable_t *hash, int counter, char *dirname){
    //get all the urls in the same layer
    int pos = 0;
    char *result;      //store urls crawled in current page
    int cur_depth = webpage_getDepth(cur_page);
    int next_depth = cur_depth+1;
    void (*fn1)(queue_t *) = print_element;
    bool (*fn2)(void *elementp, const void* searchkeyp) = qsearch_url;
    bool (*fn3)(void *elementp, const void* searchkeyp) = hsearch_url;
    bool fetch = webpage_fetch(cur_page);
    //webpage_t *new_page = NULL;
        
    if(fetch){
        printf("\ncurrent url = %s\n", webpage_getURL(cur_page));
        url_search_and_hput(cur_page, hash);
        pagesave(cur_page, counter, dirname);
        printf("%p; file %d: %s saved!!!!!\n\n", (void*)cur_page, counter, webpage_getURL(cur_page));

        while((pos = webpage_getNextURL(cur_page, pos, &result)) > 0 ){          
            // printf("get NextURL\n");
					char *new_url = malloc(strlen(result)+1);
					strcpy(new_url, result);
					webpage_t *new_page = webpage_new(new_url, next_depth, NULL);
            if(IsInternalURL(result) && next_depth <= max){
                // printf("Depth %d: Found URL (internal): %s\n",next_depth, result);
                //new_page = webpage_new(result, next_depth, NULL);
                if( qsearch(url_queue, fn2, new_page) == false && \
                    hsearch(hash, fn3, result, strlen(result)) == false)
                {
                    qput(url_queue, new_page);
                }else{
									webpage_delete(new_page);
								}
            }else{
							// printf("Depth %d: Found URL (external): %s\n\n",next_depth, result);
							webpage_delete(new_page);
            }
            // printf("pos = %d\n",pos);
            free(result);
						free(new_url);
            //webpage_delete(new_page);
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

int main(int argc, char *argv[]){
          
        if(argc < 4){
            printf("usage: -seedurl -pagedir -maxdepth\n");
            exit(EXIT_FAILURE);
        }else if(argc == 5){
					argv[1] = argv[2];
					argv[2] = argv[3];
					argv[3] = argv[4];
					
				}

        char *seed = argv[1];
        // char *seed = "https://thayer.github.io/engs50/";
        int max_depth = atoi(argv[3]);
        int crawl_depth = 0;
        uint32_t hsize = 999;
        hashtable_t *url_hash     = hopen(hsize);
        queue_t *url_queue = qopen();
        int counter = 0;
        char *dirname = argv[2];
        
        //initiate the seed page
        webpage_t *seed_page = webpage_new(seed, 0, NULL);                     
        qput(url_queue, seed_page);

        //iterate through all the pages in the queue until it's empty
        void *qp = qget(url_queue);
        while(qp != NULL){
					//while(qp != NULL && crawl_depth <= max_depth){
            crawl_depth = get_url(qp, max_depth, url_queue, url_hash, counter, dirname);
            qp = qget(url_queue);
            // pagesave(qp, counter, dirname);
            counter +=1;
            if(qp == NULL){
                printf("queue is empty!!!\n\n");
            }
        }
          
        qclose(url_queue);
        hclose(url_hash);
        exit(EXIT_SUCCESS);
}
