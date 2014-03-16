//by Shashank Heda
// Serial code to compute Pi using Monte Carlo method

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include<time.h>
#define PI      3.141592653589793238462643
#define SEED 36584136

main(int argc, char* argv)
{
   int iter=0;
   double x,y;
   int i,count=0; /* # of points in the 1st quadrant of unit circle */
   double z,pi,timer,error;
   clock_t start_t, stop_t;

   printf("\n\tPlease enter the number of iterations used to estimate pi: ");
   scanf("%d",&iter);
   
   start_t = clock();


   /* initialize random numbers */
   srand(SEED);
   count=0;
   for ( i=0; i<iter; i++) {
      x = (double)rand()/RAND_MAX;
      y = (double)rand()/RAND_MAX;
      z = x*x+y*y;
      if (z<=1) count++;
      }
   pi=(double)count/iter*4;

   stop_t = clock();

   timer = (double)((double)(stop_t - start_t) / (double)CLOCKS_PER_SEC);

   error = fabs((pi - PI)/PI) * 100;

   printf("\n\tTotal Number of trials= %d , approximate value of pi is %11.20e",iter,pi);
   printf("\n\tKnown value of  PI                              : %11.20e",PI);
   printf("\n\tCPU time elapsed                        	:%lf seconds", timer);
   printf("\n\tPercentage Error                                : %11.10f\n", error);

}


		

		
