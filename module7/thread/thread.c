/* thread.c --- 
 * 
 * 
 * Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
 * Created: Tue Feb 28 12:03:13 2023 (-0500)
 * Version: 1.0
 * 
 * Description: 
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
# include <ctype.h>
# include <indexio.h>
# include <dirent.h>
# include <threadio.h>
# include <pthread.h>

char *message1, *message2;
lqueue_t *lqueue;
int count1 = 0;
int count2 = 0;

void queue_print(void *p){
    char *s = (char *)p;
    printf("%s ",s);
}

void *put_element1(void *qp){
    while(count1 < 100){
        lqput(lqueue, message1);
        count1 +=1;
    }
    printf("\n");
    pthread_exit(NULL);

}

void *put_element2(void *p){
   while(count2 < 100){
        lqput(lqueue, message2);
        count2 +=1;
    }
    printf("\n");
    pthread_exit(NULL);
}

int main(void){

    pthread_t tid1, tid2;
    lqueue = lqopen();
    message1 = "1";
    message2 = "2";
    
    
    if(pthread_create(&tid1, NULL, put_element1, (void*) message1)!=0){
        printf("Failed to create thread 1\n");
        exit(EXIT_FAILURE);                                                           
    }
    if(pthread_create(&tid2, NULL, put_element2, (void*) message2)!=0){
        printf("Failed to create thread 2\n");
        exit(EXIT_FAILURE);                                                           
    }    
        
    if(pthread_join(tid1,NULL)!=0){
        printf("Failed to join thread 1\n");
        exit(EXIT_FAILURE);                         
    }
    if(pthread_join(tid2,NULL)!=0){
        printf("Failed to join thread 2\n");
        exit(EXIT_FAILURE);                         
    }
    printf("queue: ");
    lqapply(lqueue, queue_print);
    printf("\n");
    // lqclose(lqueue);
    return 0;
}