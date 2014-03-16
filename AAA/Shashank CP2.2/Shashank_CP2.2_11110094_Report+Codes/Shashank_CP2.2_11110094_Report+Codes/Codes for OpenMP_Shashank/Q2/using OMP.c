//by Shashank Heda
/*Program to calculate value of pi*/

#include <stdio.h>  // Core input/output operations                         
#include <stdlib.h> // Conversions, random numbers, memory allocation, etc. 
#include <math.h>   // Common mathematical functions                        
#include <time.h>   // Converting between various date/time formats         
#include<omp.h>
#define PI      3.141592653589793238462643

int main () { 

  long int ndarts,i;
  int nthreads;
  double error,pi_current,pi_sum,sTime,eTime;         // Error in calculation of PI           

  scanf(" %d ",&nthreads);
  omp_set_num_threads(nthreads);
  scanf(" %ld",&ndarts);

  sTime = omp_get_wtime();
  int    llimit=-1,        // Lower limit for random numbers       
         ulimit=1,        // Upper limit for random numbers       
         n_circle=0;      // Number of darts that hit the circle  

	  double x,             // x coordinate, betwen -1 & +1         
                y,             // y coordinate, betwen -1 & +1         
                z;             // Sum of x^2 and y^2

#pragma omp parallel reduction(+ : n_circle)
{
	for (i = 1; i <= ndarts/nthreads; i++)
	{
		x = ((ulimit- llimit) * ((double) rand()/(double) RAND_MAX)) + llimit;
		y = ((ulimit- llimit) * ((double) rand()/(double) RAND_MAX)) + llimit;

		z=x*x+y*y;
		if (z <= 1.0) 
	       {
       	       n_circle++;
    		}
	}
printf("\n\n\t\t\tN circle is %d ",n_circle);
printf("\n\n\t\t\tLocal pi is %e",(4.0 * nthreads * (double)n_circle)/(double)ndarts);
}

printf("\n\n\t\t\t\t\t\tN circle is %d ",n_circle);

	pi_current = 4.0 * (double)n_circle/(double)ndarts;printf("\npi_current: %e\n",pi_current);
	pi_sum=pi_current;//nthreads;
 	eTime = omp_get_wtime();

	error = fabs((pi_sum - PI)/PI) * 100;

	     	//printf("Known value of  PI                                 : %11.20e\n",PI);
		printf("Calculated value of pi is                          : %11.20e\n",pi_sum);
		printf("Total Time Taken is                                : %e\n",eTime-sTime);
	       printf("No. of iterations is                               : %ld\n",nthreads * ndarts);      
	       printf("Percentage Error                                   : %11.10f\n", error); 
       
return 0;
}