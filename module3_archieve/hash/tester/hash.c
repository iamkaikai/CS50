/* 
 * hash.c -- implements a generic hash table as an indexed set of queues.
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <hash.h>
#include <queue.h>
/* 
 * SuperFastHash() -- produces a number between 0 and the tablesize-1.
 * 
 * The following (rather complicated) code, has been taken from Paul
 * Hsieh's website under the terms of the BSD license. It's a hash
 * function used all over the place nowadays, including Google Sparse
 * Hash.
 */
#define get16bits(d) (*((const uint16_t *) (d)))

typedef struct my_hashtable{
    uint32_t hsize;
    queue_t **qp;
}my_hashtable_t;

static uint32_t SuperFastHash (const char *data, int len, uint32_t tablesize) {
  uint32_t hash = len, tmp;
  int rem;
  
  if (len <= 0 || data == NULL)
		return 0;
  rem = len & 3;
  len >>= 2;
	
  /* Main loop */
  for (;len > 0; len--) {
    hash  += get16bits (data);
    tmp    = (get16bits (data+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    data  += 2*sizeof (uint16_t);
    hash  += hash >> 11;
  }
	
  /* Handle end cases */
  switch (rem) {
  case 3: hash += get16bits (data);
    hash ^= hash << 16;
    hash ^= data[sizeof (uint16_t)] << 18;
    hash += hash >> 11;
    break;
  case 2: hash += get16bits (data);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1: hash += *data;
    hash ^= hash << 10;
    hash += hash >> 1;
  }
  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;
  return hash % tablesize;
}

/* hopen -- opens a hash table with initial size hsize */
hashtable_t *hopen(uint32_t hsize){
  my_hashtable_t *ht;
  if (!(ht = malloc(sizeof(my_hashtable_t)))){
      printf("Error: malloc failed allocating hash table!!\n");
      return NULL;
  }
	if (!(ht->qp = malloc(sizeof(queue_t*)*hsize))){
		printf("Error: malloc failed allocating hash table!!\n");
		return NULL;
	}
  ht->hsize = hsize;
  for (int i=0; i<ht->hsize;i++){
     ht->qp[i]=NULL;
  }
  printf("hash opened!\n");
  return (hashtable_t*)ht;
}




/* hclose -- closes a hash table */
void hclose(hashtable_t *htp){
	if(htp == NULL){
		printf("argument invalid!");
		exit(1);
	}
	my_hashtable_t *ht = (my_hashtable_t*)htp;
	for (int i=0; i<ht->hsize;i++){
		if (ht->qp[i] != NULL){
			printf("%p",ht->qp[i]);
			qclose(ht->qp[i]);
		}
	}
	free(ht);
	printf("closed\n");
}



/* hput -- puts an entry into a hash table under designated key                               
 * returns 0 for success; non-zero otherwise                                                  
 */

int32_t hput(hashtable_t *htp, void *ep, const char *key, int keylen){
    if(htp == NULL || ep == NULL || key == NULL || keylen < 0){
        printf("argument invalid!");
        return -1;
    }else{
        my_hashtable_t *ht = (my_hashtable_t*)htp;
        uint32_t idx = SuperFastHash(key, keylen, ht->hsize);  //get key for hash
        if ((ht->qp[idx]) != NULL){
            qput(ht->qp[idx],ep);
            return 0;
        }else{
            queue_t *nqp = qopen();
            qput(nqp,ep);
            ht->qp[idx]=nqp;
            return 0;
        }
    }
}




/* happly -- applies a function to every entry in hash table */
void happly(hashtable_t *htp, void (*fn)(void* ep)){
    if(htp == NULL){
        printf("argument invalid!");
        exit(1);
    }else {
        my_hashtable_t *ht = (my_hashtable_t *) htp;
        for (int i=0; i<ht->hsize;i++){
            if ((ht->qp[i]) != NULL){
                qapply(ht->qp[i],fn);
            }
        }
    }
}



/* hsearch -- searchs for an entry under a designated key using a                             
 * designated search fn -- returns a pointer to the entry or NULL if                          
 * not found                                                                                  
 */
void *hsearch(hashtable_t *htp,
							bool (*searchfn)(void* elementp, const void* searchkeyp),
				      const char *key,
				      int32_t keylen){
    if(htp == NULL){
        printf("argument invalid!");
        exit(1);
    }else {
        my_hashtable_t *ht = (my_hashtable_t *) htp;
        uint32_t idx = SuperFastHash(key, keylen, ht->hsize);  //get key for hash
        qsearch(ht->qp[idx],searchfn,key);
    }
}



/* hremove -- removes and returns an entry under a designated key                             
 * using a designated search fn -- returns a pointer to the entry or            
 * NULL if not found               
 */

void *hremove(hashtable_t *htp,
							
        bool (*searchfn)(void* elementp, const void* searchkeyp),
							
        const char *key,

        int32_t keylen){
    if(htp == NULL){
        printf("argument invalid!");
        exit(1);
    }else {
        my_hashtable_t *ht = (my_hashtable_t *) htp;
        uint32_t idx = SuperFastHash(key, keylen, ht->hsize);  //get key for hash
        qremove(ht->qp[idx],searchfn,key);
    }
}
