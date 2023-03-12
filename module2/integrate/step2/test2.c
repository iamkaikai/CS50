/* test1.c --- 
 * 
 * 
 * Author: Yenkai Huang
 * Created: Mon Jan 16 16:12:26 2023 (-0500)
 * Version: 1.0
 * 
 * Description: 
 * 
 */

# include <stdio.h>                                                              
# include <stdlib.h>                                                                      
# include <math.h>                                                                        
# include <stdint.h>                                                                      
# include <string.h> 
# include "integrate.h"

double f(double x){                                                                       
  double y;                                                                                  
  y=2*(x*x)+9*x+4;                                                                         
  return y;                                                 
} 

int main(void){                                                                              
	printf("test2 starts!!!!!!!!!!!!!!!!!\n");
  double a, b, p, *rp, result;
	uint32_t *sp,n;
	double (*fn)(double x);
	a = 1;
	b = 3;
	p = 1;        //this should return error

	result = 0;
	n = 1;
	rp = &result;
	sp = &n;
	fn = f;

	integrate(fn, a, b, p, rp, sp);
	printf("test2 ends!!!!!!!!!!!!!!!!!!!!!!!1\n");
	
	return 0;                                                                                  
}        
