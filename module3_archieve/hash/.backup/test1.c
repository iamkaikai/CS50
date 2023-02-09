/* test.c --- 
 * 
 * 
 * Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
 * Created: Tue Jan 24 08:33:13 2023 (-0500)
 * Version: 1.0 
 * 
 * Description: test file for hash
 * 
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <hash.h>

//define constructor
#define TABLESIZE 10     
#define MAXREG 10     


typedef struct car {
  struct car *next;
  char plate[MAXREG];
  double price;
  int year;
} car_t;

#define MAXADD 20     
#define MAXOWN 10     
typedef struct house {
  struct house *next;
  char owner[MAXOWN];
	char address[MAXADD];
} house_t;



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

void car_price_double(void* elementp){
	car_t *car = (car_t*)elementp;
	car->price *= 2;
	printf("car price*2 = %lf\n", car->price);                                    
}                                                                           

void print_house(void* elementp){
	house_t *house = (house_t*)elementp;
  printf("house owner = %s; address = '%s'\n", house->owner, house->address);
}                                                                           

void print_next(void* elementp){
	car_t *car = (car_t*)elementp;
  printf("address = %p\n", (void *)car->next);
}                                                                           


//make functions
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

house_t *make_house(char *ownerP, char *addrP){
	house_t *house;
  if( !(house = (house_t*)malloc(sizeof(house_t))) ){
		printf("Error:malloc failed allocating house!!!!\n");
		return NULL;
	}
	house -> next = NULL;
  strcpy(house->owner, ownerP);
  strcpy(house->address, addrP);
  return house;
}

//test in main
int main(void){
	
	hashtable_t *hp1 = hopen(10);
	//	queue_t *qp2 = qopen();

	car_t *car1 = make_car("ABCD1234", 1000, 2023);
	car_t *car2 = make_car("ABCD1235", 2000, 2021);
	car_t *car3 = make_car("ABCD1236", 3000, 2020);
	//car_t *car4 = make_car("ABCD1237", 4000, 2030);
	//car_t *car5 = make_car("ABCD1238", 5000, 2040);
	//house_t *house1 = make_house("Lius", "6 juniper cr");
	//house_t *house2 = make_house("Erin", "Dartmouth college");

	//void (*fn_p1)(void *cp) = print_car_plate;
	//void (*fn_p2)(void *cp) = print_car_price;
	//void (*fn_p3)(void *cp) = print_car_year;
	//void (*fn_p4)(void *cp) = car_price_double;
	//void (*fn_p5)(void *cp) = print_house;
	//void (*fn_p6)(void *cp) = print_next;

	hput(hp1, car1, (char*)car1->plate, 8);
	hput(hp1, car1, (char*)car1->plate, 8);
	hput(hp1, car2, (char*)car2->plate, 8);
	hput(hp1, car3, (char*)car3->plate, 8);
	//	qput(qp2, house1);
	//qput(qp2, house2);
	//void *qp3 = qget(qp1);
  //void *qp4 = qget(qp1);
  //print_car_plate(qp3);

	//free(qp3);
	//free(qp4);

	//printf("-------------------\n");
	//qapply(qp1, fn_p1);
	//qapply(qp1, fn_p2);
	//qapply(qp1, fn_p3);
	//qapply(qp1, fn_p4);
	//	qapply(qp2, fn_p1);

	//qconcat(qp1,qp2);
	//qapply(qp1, fn_p1);
	//qapply(qp2, fn_p1);

	//qclose(qp1);
	hclose(hp1);
	exit(EXIT_SUCCESS);
	
}


