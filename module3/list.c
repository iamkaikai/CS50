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
# include <list.h>

#define MAXREG 10

/*the representation of the car */
/*struct car {
  struct car *next;
  char plate[MAXREG];
  double price;
  int year;
}car_t;
*/

//list of cars
static car_t *front=NULL;

/*
* put(); place a car at the beginning of the list
* returns 0 if successful; nonzero otherwise
* first make *front = car
* if the *front = NULL, then make the next pointer = NULL
* if not, then make the next pointer = front car's original pointer
*/

int32_t lput(car_t *cp){
  //declare new pointer
  car_t *np;

  // if the list is empty then make front the new car and point it to null
  if (front == NULL){
    front=cp;
    cp->next=NULL;
  }
  // if the list is not empty, make front = to the new car and make new car point to origial 1st car
  else{
    np=front;
    front=cp;
    cp->next=np;
  }
  return 0;
}


/*
* get(); remove and return the first car in the list;
* return NULL if the list is empty
* if front=NULL then return NULL
* if front != NULL then return front
*/

car_t *lget(){
  //if the list isn't empty
  if (front != NULL){
    car_t *got;         //declare new pointer
    got=front;          //copy got to front
    front=front->next;  //remove the original front car
    return got;
  }
  else{
    return NULL;
  }
}

/* apply a function to every car in the list */                                 
void lapply(void (*fn)(car_t *cp)){
	car_t *p;
	for(p=front; p!=NULL; p=p->next){
		fn(p);
    }
}


/* remove(): find, remove, and return any car with                   
 * the designated plate; return NULL if not present
 */                                                                   
car_t *lremove(char *platep){
	car_t *p,*f;
	//if the list is not empty
	if (front != NULL){
		//if the first car need to be removed
		if (strcmp(front->plate,platep)==0){
			p=front;
 			front=front->next;
			return p;
		}
		
		//if the middle or the last car need to be removed
		for(p=front->next; p!=NULL; p=p->next){
				if(strcmp(p->plate,platep)==0){
				f->next = p->next;
				return p;                                           
			}                                                        
			f=p;
		}
		return NULL;
  }
	//if the list is empty
	else{
		return NULL;
	}
}
