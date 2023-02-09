/* test3.c --- 
 * 
 * 
 * Author: Hongke Lu
 * Created: Sun Jan 29 16:44:26 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include "queue.h"
# include <stdbool.h>

typedef struct person {
    char name[10];
    int age;
    double rate;
} person_t;

person_t *make_person(char *namep,int age,double rate)  {
    person_t *pp;
    if(!(pp = (person_t*)malloc(sizeof(person_t)))) {
        printf("[Error: malloc failed allocating person]\n");
        return NULL;
        }
    strcpy(pp->name,namep);
    pp->age=age;
    pp->rate=rate;
    return pp;
}

void print_person_name(void* elementp){
    person_t *personp = (person_t*)elementp;
    printf("person name = %s\n", personp->name);
}

bool searchFunction(void* elementp, const void* skeyp){
    person_t *personp = (person_t*) elementp;

    printf("we want to search\n");
    if (//strcmp(personp->name,skeyp)==0
        elementp == skeyp){
        return true;
    }else{return false;}
}

int main(void) {
    person_t *pp1, *pp2, *pp3, *testp;
    queue_t *qp;

    qp = qopen();
    pp1 = make_person("Fred",20,20.0);
    qput(qp,(void*)pp1);
    pp2 = make_person("abc",20,20.0);
    qput(qp,(void*)pp2);
    pp3 = make_person("bcd",20,20.0);
    qput(qp,(void*)pp3);
    qapply(qp,print_person_name);
    printf("--------\n");
    //char *name[10];
    //strcpy(*name,"Fred");
    testp = qsearch(qp,  &searchFunction, pp1);
    print_person_name(testp);
    //testp = qremove(qp,searchFunction,name);
    //print_person_name(testp);
    //qapply(qp,print_person_name);
    qclose(qp);
}
