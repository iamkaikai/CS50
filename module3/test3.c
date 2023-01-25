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
# include "list.h"


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

void printCar(car_t *cp){
	if (cp!=NULL){
		printf("plate: %s, price: %f, year: %d \n", cp->plate,cp->price,cp->year);
	}
}

int main(void){
	car_t *car1 = make_car("ABC1234", 3000.0, 2003);
	car_t *car2 = make_car("ABC2", 3000.0, 2003);
	car_t *car3 = make_car("ABC3", 3000.0, 2003);
	car_t *car4 = make_car("ABC4", 3000.0, 2003);
	
	lput(car1);
	lput(car2);
	lput(car3);
	lput(car4);

	lapply(&printCar);
	
	printf("Test for remove \n");
	car_t *abc;
	abc=lremove("ABC1234");
	if (abc!=NULL){
		printf("%s \n", abc->plate);
	}
	else {
		printf("NULL");
	}
	lapply(&printCar);
	printf("Test for get\n");
	lget();
	lapply(&printCar);
	free(car1);
  free(car2);
  free(car3);
	free(car4);
	exit(EXIT_SUCCESS);
	
}


