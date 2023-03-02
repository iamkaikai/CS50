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

//define constructor
#define MAXREG 10     
typedef struct car {
  struct car *next;
  char plate[MAXREG];
  double price;
  int year;
} car_t;

void print_car_plate(void* elementp){
	car_t *car = (car_t*)elementp;
  printf("car plate = %s\n", car->plate);                                    
}  

car_t *make_car(char *plateP, double price, int year){
	car_t *car;
  if( !(car = (car_t*)malloc(sizeof(car_t))) ){
		printf("Error:malloc failed allocating car!!!!\n");
		return NULL;
	}
	car -> next = NULL;
  strcpy(car->plate, plateP);
  car -> price = price;
  car -> year = year;
	return car;
}
bool lqsearch_fn(void* elementp,const void *skeyp){
    car_t *car = (car_t*)elementp;
    char* p = (char*) skeyp;
    return strcmp(car->plate,p)==0;
}

lqueue_t *lqueue;
lhashtable_t *lhash;
int count1 = 0;
int count2 = 0;

void *mainq1(void *p){
    car_t *car1 = make_car("ABCD1234", 1000, 2023);
    car_t *car2 = make_car("ABCD1235", 2000, 2021);
    lqput(lqueue, car1);
	lqput(lqueue, car2);
    printf("test for lsearch\n");
    car_t *car = lqsearch(lqueue,lqsearch_fn,"ABCD1234");
    print_car_plate(car);
    printf("\n");
    pthread_exit(NULL);

}

void *mainq2(void *p){
    car_t *car3 = make_car("ABCD1236", 3000, 2020);
    car_t *car4 = make_car("ABCD1237", 4000, 2030);
    lqput(lqueue, car3);
	lqput(lqueue, car4);
    printf("test for lqget \n");
    car_t *car = lqget(lqueue);
    print_car_plate(car);
    free(car);
    printf("\n");
    pthread_exit(NULL);
}

void *mainh1(void *p){
    car_t *car1 = make_car("ABCD1234", 1000, 2023);
    car_t *car2 = make_car("ABCD1235", 2000, 2021);
    lhput(lhash, car1,"ABCD1234",8);
	lhput(lhash, car2,"ABCD1235",8);
    printf("test for lsearch\n");
    car_t *car = lhsearch(lhash,lqsearch_fn,"ABCD1234",8);
    print_car_plate(car);
    printf("\n");
    pthread_exit(NULL);
}

void *mainh2(void *p){
    car_t *car3 = make_car("ABCD1236", 3000, 2020);
    car_t *car4 = make_car("ABCD1237", 4000, 2030);
    lhput(lhash, car3,"ABCD1236",8);
	lhput(lhash, car4,"ABCD1237",8);
    //lhapply(lhash, print_car_plate);
    printf("test for lhremove \n");
    car_t *car = lhremove(lhash,lqsearch_fn,"ABCD1236",8);
    print_car_plate(car);
    free(car);
    printf("\n");
    pthread_exit(NULL);
}

int test1(void){

    pthread_t tid1, tid2;
    lqueue = lqopen();
    if(pthread_create(&tid1, NULL, mainq1, NULL)!=0){
        printf("Failed to create thread 1\n");
        exit(EXIT_FAILURE);                                                           
    }
    if(pthread_create(&tid2, NULL, mainq2, NULL)!=0){
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
    lqapply(lqueue, print_car_plate);
    printf("\n");
    lqclose(lqueue);
    return 0;
}

int test2(void){

    pthread_t tid1, tid2;
    lhash = lhopen(20);
    if(pthread_create(&tid1, NULL, mainh1, NULL)!=0){
        printf("Failed to create thread 1\n");
        exit(EXIT_FAILURE);                                                           
    }
    if(pthread_create(&tid2, NULL, mainh2, NULL)!=0){
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
    printf("lhash: ");
    lhapply(lhash, print_car_plate);
    printf("\n");
    lhclose(lhash);
    return 0;
}

int main(void){
    //test for lqueue
    //return test1();
    //test for lhash
    return test2();
}
