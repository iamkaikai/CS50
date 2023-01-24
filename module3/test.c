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

int main(void){
	
	car_t *car1 = make_car("ABC1234", 3000.0, 2003);
  free(car1);
	//exit(EXIT_SUCCESS);
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
