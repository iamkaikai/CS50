/* list.c --- 
 * 
 * 
 * Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
 * Created: Mon Jan 23 14:24:41 2023 (-0500)
 * Version: 
 * 
 * Description: Implementation of the linked list
 * 
 */

# include <stdio.h>                                                             
# include <stdlib.h>                                                            
# include <stdint.h>                                                            
# include <string.h>                                                            
# include <queue.h>                                                             
                                                                                
typedef struct qnode {                                                          
    void *element;                                                              
    struct qnode *next;                                                         
} qnode_t;                                                                      
                                                                                

/* the queue representation is hidden from users of the module */               
typedef struct my_queue {                                                       

  qnode_t *front;                                                             
  qnode_t *back;
	
} my_queue;                                                                     


/* create an empty queue */                                                     
queue_t* qopen(void){                                                           
    my_queue *q;                                                               
    if( !(q = (my_queue*)malloc(sizeof(my_queue))) ){                                  
        printf("Error:malloc failed allocating queue!!!!\n");                   
        return NULL;                                                            
    }
		q -> front = NULL;                                                          
    q -> back = NULL;
		//printf("back = %p\n", (void *)q->back);
		return q;              
}

/* deallocate a queue, frees everything in it */
void qclose(queue_t *qp){
    my_queue *nqp;
    nqp = ((my_queue*) qp);
    printf("before front = %p\n", (void *)nqp->front);
    printf("before back = %p\n", (void *)nqp->back);
    qnode_t *temp;
    while(nqp ->front != NULL){
        temp =nqp -> front;
        nqp -> front = nqp -> front -> next;
				free(temp->element);
				free(temp);
    }
    nqp -> back = NULL;
    printf("front = %p\n", (void *)nqp->front);
    printf("back = %p\n", (void *)nqp->back);
    free(nqp);
}

/* put element at the end of the queue                                                               
returns 0 is successful; nonzero otherwise */                                                        
int32_t qput(queue_t *qp, void *elementp){                                                           

	  if(qp == NULL){
			printf("Error: Queue is not initialized\n");
			return -1;
    }else if(elementp == NULL){
			printf("Error: put is empty\n");
			return -1;
	  }

	//makes new node new with elementp                                                               
    my_queue *nqp = (my_queue*)qp;                                                                   
    qnode_t *new;
		
		if( !(new = (qnode_t*)malloc(sizeof(qnode_t))) ){
        printf("Error:malloc failed allocating queue!!!!\n");
        exit(EXIT_FAILURE);
    }
		
    new->element = elementp;                                                                         
    new->next = NULL;
		
    //if queue is empty, make new become head                                                        
    if ((nqp->front == NULL) && (nqp->back == NULL)) {                                               
			nqp->front = new;                                                                            
			nqp->back = new;                                                                             

		//if not empty, add new to the end of queue                                                    
    } else {
			nqp->back->next = new;
      nqp->back = new;
    }
		return 0;
}


/* get the first first element from queue, removing it from the queue */                             
void* qget(queue_t *qp) {

	  printf("get beging!!\n");
    my_queue *nqp = (my_queue*)qp;
		void *elementP;       
		
		//if queue is empty, return -1                                                                   
    if((nqp->front == NULL) && (nqp->back == NULL)) {                                                
        printf("Error: the queue is empty. \n");                                                     
        return NULL;
				
    //make the second node head and delete previous head                                             
    }else{
			
			//			qnode_t *front = nqp->front;
			
			if(nqp->front->next == NULL){
				elementP = nqp->front->element;
				nqp->front = NULL;
				nqp->back = NULL;
			}else{
				elementP = nqp->front->element;
				qnode_t *qnode = nqp->front;
				nqp->front = nqp->front->next;
				free(qnode);
			}
    }
		
		return (void *)elementP;
}


/*apply a function to every element of the queue*/
void qapply(queue_t *qp, void (*fn)(void* elementp)){

	my_queue *nqp = (my_queue*)qp;
	if((nqp->front == NULL) && (nqp->back == NULL)){
			printf("The queue is empty. Nothing to apply!!\n");
	}else{
		//if there's only one object in the queue
		if(nqp->front->next == NULL){
			fn(nqp->front->element);
		//if there are more than one objects in the queue
		}else{
			qnode_t *qcurrent = nqp->front;
			qnode_t *qnext = nqp->front->next;
			while(qnext != NULL){
				fn(qcurrent->element);
				qcurrent = qnext;
				qnext = qcurrent->next;
				//execute the back object in the queue
				if(qnext == NULL){
					fn(qcurrent->element);
				}
			}
		}		
	}
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
void* qsearch(queue_t *qp,
              bool (*searchfn)(void* elementp,const void* keyp),
              const void* skeyp){

    my_queue *nqp = (my_queue*)qp;
    if((nqp->front == NULL) || (nqp->back == NULL)){
        printf("Error: the queue is either empty or something wrong!!\n");
        return NULL;
    }else{
        //if there's only one object in the queue
        if(nqp->front->next == NULL){
            if (searchfn(nqp->front->element,skeyp)){
                return nqp->front->element;
            }else{
                return NULL;
            }

            //if there are more than one objects in the queue
        }else{
            qnode_t *qcurrent = nqp->front;
            qnode_t *qnext = nqp->front->next;
            while(qnext != NULL){
                if (searchfn(qcurrent->element,skeyp)){
                    return qcurrent->element;
                }

                qcurrent = qnext;
                qnext = qcurrent->next;

                //execute the back object in the queue
                if(qnext == NULL){
                    if(searchfn(qcurrent->element,skeyp)){
                        return qcurrent->element;
                    }
                }
            }
            return NULL;
        }
    }
}

/* search a queue using a supplied boolean function (as in qsearch),
 * removes the element from the queue and returns a pointer to it or
 * NULL if not found
 */
void* qremove(queue_t *qp,
              bool (*searchfn)(void* elementp,const void* keyp),
              const void* skeyp){
    my_queue *nqp;
    nqp = ((my_queue*) qp);
    if((nqp->front == NULL) || (nqp->back == NULL)) {
        printf("Error: the queue is either empty or something wrong!!\n");
        return NULL;
    }else{
        qnode_t *temp;
        temp = nqp -> front;
        //if there is only one element in the queue
        if (nqp ->front == nqp->back){
            if (searchfn(temp->element,skeyp)){
                nqp ->front = NULL;
                nqp ->back = NULL;
								void* elementp = temp->element;
								free(temp);
                return elementp;
            }else{
                return NULL;
            }
        } else {
            //if the front is the same as keyp
            if (searchfn(temp->element,skeyp)){
                nqp->front = nqp->front->next;
                return temp->element;
            }else{
                qnode_t *previous;
                while(temp != nqp->back){
                    if (searchfn(temp->element,skeyp)){
                        previous ->next = temp->next;
												void* elementp = temp->element;                                                                                    
												free(temp);                                                                                                        
												return elementp; 
                    }else{
                    //move to next qnode
                        previous = temp;
                        temp = temp -> next;
                    }
                }
                //then work on the last element or temp == nqp->back
                if (temp == nqp->back){
                    if (searchfn(temp->element,skeyp)){
                        previous->next = NULL;
                        nqp ->back =previous;
												void* elementp = temp->element;                                                                                    
												free(temp);                                                                                                        
												return elementp; 
                    }
                    else{
                        return NULL;
                    }
                }
            }
        }
    }
		return NULL;
}

/* concatenatenates elements of q2 into q1                              
 * q2 is dealocated, closed, and unusable upon completion               
 */                                                                     
void qconcat(queue_t *q1p, queue_t *q2p){

	//check whether queue1 and queue are not empty
	my_queue *nq1p = (my_queue*)q1p;
	my_queue *nq2p = (my_queue*)q2p;

	printf("start........\n");
	//if both queue are empty
	if(nq1p->front == NULL && \
		 nq1p->back == NULL && \
		 nq2p->front == NULL && \
		 nq2p->back == NULL){

		qclose(q2p);
		
	//if queue1 is empty, put queue2 in front
	}else if(nq1p->front == NULL && \
					 nq1p->back == NULL && \
					 nq2p->front != NULL && \
					 nq2p->back != NULL){
		
		nq1p->front = nq2p->front;
		nq1p->back = nq2p->back;
	
	//if queue1 is not empty, put queue2 in back
	}else if(nq1p->front != NULL && \
					 nq1p->back != NULL && \
					 nq2p->front != NULL && \
					 nq2p->back != NULL){
		
		nq1p->back->next = nq2p->front;
		nq1p->back = nq2p->back;

	//if queue1 is not empty but queue2 is empty, close queue2
	}else if(nq1p->front != NULL && \
					 nq1p->back != NULL && \
					 nq2p->front == NULL && \
					 nq2p->back == NULL){
		
		qclose(q2p);
		
	}else{
		printf("Something wrong with front or back!\n");
		exit(EXIT_FAILURE);
	}
	
	//free queue2
	nq2p->front = NULL;
	nq2p->back = NULL;
	free(nq2p);
	q2p = NULL;

}
