/* integrate.c --- 
 * 
 * 
 * Author: Yenkai Huang
 * Created: Wed Jan 11 14:30:57 2023 (-0500)
 * Version: 1.2
 * 
 * Description: a little function of integration 
 * 
 */

# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <stdbool.h>
# include <math.h>
# include <stdint.h>
# include <string.h>


int32_t integrate(double (*f)(double x),double a, double b, double p, double *rp, uint32_t *sp){
	printf("\n");
	int i;
	double result = 0;
	double h;

	//check if b>a
	if(b > a){
		printf("b > a checked!!!\n");

		if(p < 1 && p>0){

			printf("precision = %lf\n",p);
			
			double diff = 1.0;
			double prev_result = 0.0;
			// reset the value of global variable 
			*rp = 0;
			*sp = 1;
			while(diff>p){
				result = 0;
				h = (b-a)/(double)*sp;

				for(i=0;i<*sp;i++){
				  result += (( f(a+i*h) + f(a+(i+1)*h) )*h/2.0);
				}

				diff = fabs(result - prev_result);
				prev_result = result;
				*sp +=1;

			}
			*rp = result;
			printf("\ninterval:[%lf-%lf], p:%lf, result=%lf\n\n",a,b,p,*rp);
		}else{
			printf("value of -p must between 0 and 1!!\n");
			exit(EXIT_FAILURE);
		}

	}else{
		printf("b must be greater than a!!\n");
		exit(EXIT_FAILURE);
	}

	printf("\n");
	exit(EXIT_SUCCESS);

}
