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
    if( !(q = (my_queue*)malloc(sizeof(q))) ){                                  
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
        free(temp);
    }
    nqp -> back = NULL;
    printf("front = %p\n", (void *)nqp->front);
    printf("back = %p\n", (void *)nqp->back);
    free(nqp);
}

int32_t qput(queue_t *qp, void *elementp){
/* put element at the end of the queue
returns 0 is successful; nonzero otherwise */

	  //makes new node new with elementp
    my_queue *nqp = (my_queue*)qp;
		//printf("nqp = %p\n", (void *)nqp);
    qnode_t *new = malloc(sizeof(qnode_t));
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


		if (nqp->back == new) {
        printf("Success: back = %p\n", (void *)nqp->back);   
        return 0;
    } else {
        printf("Error: back = %p\n", (void *)nqp->back);  
        return -1;
    }
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


/* concatenatenates elements of q2 into q1                              
 * q2 is dealocated, closed, and unusable upon completion               
 */                                                                     
void qconcat(queue_t *q1p, queue_t *q2p){

	//check whether queue1 and queue are not empty
	my_queue *nq1p = (my_queue*)q1p;
	my_queue *nq2p = (my_queue*)q2p;
			
	//if both queue are empty
	if(nq1p->front == NULL && \
		 nq1p->back == NULL && \
		 nq2p->front == NULL && \
		 nq2p->back == NULL){
		
		printf("both queue are empty. close queue2. keep queue1!\n");
	
	//if queue1 is empty, put queue2 in front
	}else if(nq1p->front == NULL && \
					 nq1p->back == NULL && \
					 nq2p->front != NULL && \
					 nq2p->back != NULL){
			
		nq1p->front = nq2p->front;
		nq2p->back = nq2p->back;
	
	//if queue1 is not empty, put queue2 in back
	}else if(nq1p->front != NULL && \
					 nq1p->back != NULL && \
					 nq2p->front != NULL && \
					 nq2p->back != NULL){

		nq1p->back->next = nq2p->front;
		nq1p->back = nq2p->back;

	//if queue1 is not empty but queue2 is empty, close queue2
	}else{
		printf("queue2 is empty. close queue2!\n");
	}
	//free queue2
	free(nq2p);
	nq2p->front = NULL;
	nq2p->back = NULL;
	q2p = NULL;
	
}
