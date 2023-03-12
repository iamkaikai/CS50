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
# include <stdbool.h>
# include <math.h>

double f(double x){
	double y;
	y=2*pow(x,2)+9*x+4;
	return y;
}

int main(int argc, char *argv[]){
	
	double a;
  double b;
	int n;                                                                          
	int i;
  double result = 0;
	double area;
	
	a=3;
	b=12;
	n=100;
	
	double h = (b-a)/(double)n;

		
	for(i=0;i<n;i++){
		
		result += (( f(a+i*h) + f(a+(i+1)*h) )*h/2);
		
	}
	
	printf("\ninterval:[%lf-%lf], n:%d, result=%lf\n\n",a,b,n,result);
	exit(EXIT_SUCCESS);
}
