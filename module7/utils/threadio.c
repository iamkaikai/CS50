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
#include <queue.h>
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
}        

/* deallocate a queue, frees everything in it */
void lqclose(lqueue_t *qp){
	qclose(qp->queue);
	pthread_mutex_destroy(&(qp->m));
	free(qp);
}

/* put element at the end of the queue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t lqput(lqueue_t *qp, void *elementp){
	pthread_mutex_lock(&(qp->m));
	qput(qp->queue,elementp);
	pthread_mutex_unlock(&(qp->m));
}

/* get the first first element from queue, removing it from the queue */
void* lqget(lqueue_t *qp){
	pthread_mutex_lock(&(qp->m));
	qget(qp->queue);
	pthread_mutex_unlock(&(qp->m));
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
	qsearch(qp->queue,searchfn,skeyp);
	pthread_mutex_unlock(&(qp->m));
}

/* search a queue using a supplied boolean function (as in qsearch),
 * removes the element from the queue and returns a pointer to it or
 * NULL if not found
 */
void* lqremove(lqueue_t *qp,
				bool (*searchfn)(void* elementp,const void* keyp),
				const void* skeyp){
	pthread_mutex_lock(&(qp->m));
	qremove(qp->queue,searchfn,skeyp);
	pthread_mutex_unlock(&(qp->m));
}


