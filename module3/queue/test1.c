/* test.c --- 
 * 
 * 
 * Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
 * Created: Tue Jan 24 08:33:13 2023 (-0500)
 * Version: 1.0 
 * 
 * Description: test file for car
 * 
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <queue.h>


#define MAXREG 10     
typedef struct car {
  struct car *next;
  char plate[MAXREG];
  double price;
  int year;
} car_t;


//functions for lapply()                                                    
void print_car_year(void* elementp){
	car_t *car = (car_t*)elementp;
  printf("car current = %d\n", car->year);                                  
}                                                                           
                                                                           
void print_car_price(void* elementp){
	car_t *car = (car_t*)elementp;
  printf("car price = %lf\n", car->price);                                   
}                                                                           
                                                                            
void print_car_plate(void* elementp){
	car_t *car = (car_t*)elementp;
  printf("car plate = %s\n", car->plate);                                    
}                                                                           

//make car constructor
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

int main(void){

	printf("main\n");	
	
	queue_t *qp1 = qopen();
	//queue_t *qp2 = qopen();

	car_t *car1 = make_car("ABCD1234", 1000, 2023);
	car_t *car2 = make_car("ABCD1235", 2000, 2021);
	car_t *car3 = make_car("ABCD1236", 3000, 2020);

	void (*fn_p1)(void *cp) = print_car_plate;
	void (*fn_p2)(void *cp) = print_car_price;
	void (*fn_p3)(void *cp) = print_car_year;

	
	qput(qp1, car1);
	qput(qp1, car2);
	qput(qp1, car3);

	qapply(qp1, fn_p1);
	qapply(qp1, fn_p2);
	qapply(qp1, fn_p3);
	

	exit(EXIT_SUCCESS);
	
}


