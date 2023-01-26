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
# include "queue.h"

typedef struct qnode {
    void *element;
    struct qnode *next;
} qnode_t;

/* the queue representation is hidden from users of the module */
typedef struct queue {
    qnode_t *front;
    qnode_t *end;
} queue_t;

/* create an empty queue */
queue_t* qopen(void){
    queue_t *q;

    if( !(q = (queue_t*)malloc(sizeof(q))) ){
        printf("Error:malloc failed allocating car!!!!\n");
        return NULL;
    }

    q -> front = NULL;
    strcpy(car->plate, plateP);
    car -> price = price;
    car -> year = year;

    return car;
}

/* deallocate a queue, frees everything in it */
void qclose(queue_t *qp);
