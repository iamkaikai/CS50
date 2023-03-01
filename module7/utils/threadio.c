/* threadio.c --- 
 * 
 * 
 * Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
 * Created: Tue Feb 28 11:32:24 2023 (-0500)
 * Version: 1.0 
 * 
 * Description: 
 * 
 */
#include <stdlib.h>
#include <queue.h>
#include <hash.h>
#include <pthread.h>
/* the queue representation is hidden from users of the module */
typedef struct lqueue{
    queue_t* queue;
    pthread_mutex_t m;
}lqueue_t;

/* create an empty queue */
lqueue_t* lqopen(void){
    queue_t* queue = qopen();
    lqueue_t* localQueue = malloc(sizeof(lqueue_t));
	pthread_mutex_t queueM;
	pthread_mutex_init(&queueM,NULL);
	localQueue->queue=queue;
	localQueue->m=queueM;
	return localQueue;
}        

/* deallocate a queue, frees everything in it */
void lqclose(lqueue_t *qp){
	qclose(qp->queue);
	pthread_mutex_t queueM = qp->m;
	pthread_mutex_destroy(&queueM);
	free(qp);
}

/* put element at the end of the queue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t lqput(lqueue_t *qp, void *elementp){
	pthread_mutex_lock(&(qp->m));
	int32_t a = qput(qp->queue,elementp);
	pthread_mutex_unlock(&(qp->m));
	return a;
}

/* get the first first element from queue, removing it from the queue */
void* lqget(lqueue_t *qp){
	pthread_mutex_lock(&(qp->m));
	void* localElement = qget(qp->queue);
	pthread_mutex_unlock(&(qp->m));
	return localElement;
}

/* apply a function to every element of the queue */
void lqapply(lqueue_t *qp, void (*fn)(void* elementp)){
	pthread_mutex_lock(&(qp->m));
	qapply(qp->queue,fn);
	pthread_mutex_unlock(&(qp->m));
}

/* search a queue using a supplied boolean function
 * skeyp -- a key to search for
 * searchfn -- a function applied to every element of the queue
 *          -- elementp - a pointer to an element
 *          -- keyp - the key being searched for (i.e. will be 
 *             set to skey at each step of the search
 *          -- returns TRUE or FALSE as defined in bool.h
 * returns a pointer to an element, or NULL if not found
 */
void* lqsearch(lqueue_t *qp, 
				bool (*searchfn)(void* elementp,const void* keyp),
				const void* skeyp){
	pthread_mutex_lock(&(qp->m));
	void* localElement = qsearch(qp->queue,searchfn,skeyp);
	pthread_mutex_unlock(&(qp->m));
	return localElement;
}

/* search a queue using a supplied boolean function (as in qsearch),
 * removes the element from the queue and returns a pointer to it or
 * NULL if not found
 */
void* lqremove(lqueue_t *qp,
				bool (*searchfn)(void* elementp,const void* keyp),
				const void* skeyp){
	pthread_mutex_lock(&(qp->m));
	void* localElement = qremove(qp->queue,searchfn,skeyp);
	pthread_mutex_unlock(&(qp->m));
	return localElement;
}

/* representation of a hashtable hidden */
typedef struct lhashtable{
	hashtable_t* hash;
	pthread_mutex_t m;
}lhashtable_t;	

/* hopen -- opens a hash table with initial size hsize */
lhashtable_t *lhopen(uint32_t hsize){
	queue_t* hash = hopen(hsize);
    lhashtable_t* localHash = malloc(sizeof(lhashtable_t));
	pthread_mutex_t hashM;
	pthread_mutex_init(&hashM,NULL);
	localHash->hash=hash;
	localHash->m=hashM;
	return localHash;
}

/* hclose -- closes a hash table */
void lhclose(lhashtable_t *htp){
	qclose(htp->hash);
	pthread_mutex_destroy(&(htp->m));
	free(htp);
}

/* hput -- puts an entry into a hash table under designated key 
 * returns 0 for success; non-zero otherwise
 */
int32_t lhput(lhashtable_t *htp, void *ep, const char *key, int keylen){
	pthread_mutex_lock(&(htp->m));
	int32_t a=hput(htp->hash,ep,key,keylen);
	pthread_mutex_unlock(&(htp->m));
	return a;
}

/* happly -- applies a function to every entry in hash table */
void lhapply(lhashtable_t *htp, void (*fn)(void* ep)){
	pthread_mutex_lock(&(htp->m));
	happly(htp->hash,fn);
	pthread_mutex_unlock(&(htp->m));
}

/* hsearch -- searchs for an entry under a designated key using a
 * designated search fn -- returns a pointer to the entry or NULL if
 * not found
 */
void *lhsearch(lhashtable_t *htp, 
	      bool (*searchfn)(void* elementp, const void* searchkeyp), 
	      const char *key, 
	      int32_t keylen){
	pthread_mutex_lock(&(htp->m));
	void* localElement = hsearch(htp->hash,searchfn,key,keylen);
	pthread_mutex_unlock(&(htp->m));
	return localElement;
}

/* hremove -- removes and returns an entry under a designated key
 * using a designated search fn -- returns a pointer to the entry or
 * NULL if not found
 */
void *lhremove(lhashtable_t *htp, 
	      bool (*searchfn)(void* elementp, const void* searchkeyp), 
	      const char *key, 
	      int32_t keylen){
	pthread_mutex_lock(&(htp->m));
	void* localElement=lhremove(htp->hash,searchfn,key,keylen);
	pthread_mutex_unlock(&(htp->m));
	return localElement;
}


