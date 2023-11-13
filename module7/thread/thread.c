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
# include <pageio.h>
# include <threadio.h>
# include <pthread.h>
# include <time.h>

lhashtable_t *url_hash;
lqueue_t *url_queue;

int counter = 1;
char *dirname;
int max_depth;


typedef struct lcount{
    int counter;
    pthread_mutex_t m;
}lcount_t;

lcount_t* createCount(){
    lcount_t* localcount = malloc(sizeof(lcount_t));
    pthread_mutex_t counterM;
	pthread_mutex_init(&counterM,NULL);
    localcount->counter = 1;
    localcount->m=counterM;
    return localcount;
}

lcount_t *count;

void countAdd(lcount_t* count){
    pthread_mutex_lock(&(count->m));
    count->counter = count->counter+1;
	pthread_mutex_unlock(&(count->m));
}

void countMinus(lcount_t* count){
    pthread_mutex_lock(&(count->m));
    count->counter = count->counter-1;
	pthread_mutex_unlock(&(count->m));
}

void destoryCount(lcount_t* count){
	pthread_mutex_t m = count->m;
	pthread_mutex_destroy(&m);
	free(count);
}


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
    if(lhsearch(hash, hsearch_url, h_url, h_url_len) == NULL){
        lhput(hash, h_url, h_url, h_url_len);
        result = false;
    }else{
        result = true;
    }
    return result;
}

int get_url(webpage_t *cur_page, int max, lqueue_t *url_queue, lhashtable_t *hash, int counter, char *dirname){
    //get all the urls in the same layer
    int pos = 0;
    char *result;      //store urls crawled in current page
    int cur_depth = webpage_getDepth(cur_page);
    int next_depth = cur_depth+1;
    void (*fn1)(queue_t *) = print_element;
    bool (*fn2)(void *elementp, const void* searchkeyp) = qsearch_url;
    bool (*fn3)(void *elementp, const void* searchkeyp) = hsearch_url;
    bool fetch = webpage_fetch(cur_page);

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
                //new_page = webpage_new(result, next_depth, NULL);
                if( lqsearch(url_queue, fn2, new_page) == false && \
                    lhsearch(hash, fn3, result, strlen(result)) == false)
                {
                    lqput(url_queue, new_page);
                }else{
					webpage_delete(new_page);
				}
            }else{
                    webpage_delete(new_page);
            }

            free(result);
            free(new_url);
        }
    }else{
        countMinus(count);
        //counter -=1;
    }
        
    printf("---------- Queue of URLs ------------\n");
    lqapply(url_queue, fn1);
    printf("-------------------------------------\n\n");

    webpage_delete(cur_page);
    return cur_depth;
}

void* thread(void *p){
    //iterate through all the pages in the queue until it's empty
        void *qp = lqget(url_queue);
        printf("thread created\n");
        while(qp != NULL){
            get_url(qp, max_depth, url_queue, url_hash, count->counter, dirname);
            qp = lqget(url_queue);
            countAdd(count);
            //counter +=1;
            if(qp == NULL){
                printf("queue is empty!!!\n\n");
            }
        }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    clock_t start_t, end_t;
    double total_t;
    start_t = clock();
    if(argc < 5){
        printf("usage: -seedurl -pagedir -maxdepth -numpthread\n");
        exit(EXIT_FAILURE);
    }else if(argc == 6){
        argv[1] = argv[2];
        argv[2] = argv[3];
        argv[3] = argv[4];
        argv[4] = argv[5];
                
    }
    char *seed = argv[1];
    max_depth = atoi(argv[3]);
    // int crawl_depth = 0;
    uint32_t hsize = 999;
    url_hash = lhopen(hsize);
    url_queue = lqopen();
    count = createCount();

    dirname = argv[2];
    int NUM_THREADS = atoi(argv[4]);
    //initiate the seed page
    webpage_t *seed_page = webpage_new(seed, 0, NULL);                     
    lqput(url_queue, seed_page);

    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    int i;

    // Create threads
    for (i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        if (pthread_create(&threads[i], NULL, thread, &thread_args[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }

    // Wait for threads to finish
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Threads finished.\n");


    lqclose(url_queue);
    lhclose(url_hash);
    destoryCount(count);

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time taken by CPU: %f\n", total_t  );
    exit(EXIT_SUCCESS);
}
