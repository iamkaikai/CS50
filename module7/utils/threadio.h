#pragma once
/* threadio.h --- 
 * 
 * 
 * Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
 * Created: Tue Feb 28 11:33:17 2023 (-0500)
 * Version: 1.0
 * 
 * Description: threadio.h for lqueue and lhash
 * 
 */

#include <stdint.h>
#include <stdbool.h>

/* the queue representation is hidden from users of the module */
typedef void lqueue_t;		

/* create an empty queue */
lqueue_t* lqopen(void);        

/* deallocate a queue, frees everything in it */
void lqclose(lqueue_t *qp);   

/* put element at the end of the queue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t lqput(lqueue_t *qp, void *elementp); 

/* get the first first element from queue, removing it from the queue */
void* lqget(lqueue_t *qp);

/* apply a function to every element of the queue */
void lqapply(lqueue_t *qp, void (*fn)(void* elementp));

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
				const void* skeyp);

/* search a queue using a supplied boolean function (as in qsearch),
 * removes the element from the queue and returns a pointer to it or
 * NULL if not found
 */
void* lqremove(lqueue_t *qp,
				bool (*searchfn)(void* elementp,const void* keyp),
				const void* skeyp);


typedef void lhashtable_t;	/* representation of a hashtable hidden */

/* hopen -- opens a hash table with initial size hsize */
lhashtable_t *lhopen(uint32_t hsize);

/* hclose -- closes a hash table */
void lhclose(lhashtable_t *htp);

/* hput -- puts an entry into a hash table under designated key 
 * returns 0 for success; non-zero otherwise
 */
int32_t lhput(lhashtable_t *htp, void *ep, const char *key, int keylen);

/* happly -- applies a function to every entry in hash table */
void lhapply(lhashtable_t *htp, void (*fn)(void* ep));

/* hsearch -- searchs for an entry under a designated key using a
 * designated search fn -- returns a pointer to the entry or NULL if
 * not found
 */
void *lhsearch(lhashtable_t *htp, 
	      bool (*searchfn)(void* elementp, const void* searchkeyp), 
	      const char *key, 
	      int32_t keylen);

/* hremove -- removes and returns an entry under a designated key
 * using a designated search fn -- returns a pointer to the entry or
 * NULL if not found
 */
void *lhremove(lhashtable_t *htp, 
	      bool (*searchfn)(void* elementp, const void* searchkeyp), 
	      const char *key, 
	      int32_t keylen);
