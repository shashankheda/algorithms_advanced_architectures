//by Shashank Heda
//Montecarlo using Collective Operations (Reduce)
#include <stdio.h>  // Core input/output operations                         
#include <stdlib.h> // Conversions, random numbers, memory allocation, etc. 
#include <math.h>   // Common mathematical functions                        
#include <time.h>   // Converting between various date/time formats         
#include <mpi.h>    // MPI functionality                                    
#include "mpe.h"
#define PI      3.141592653589793238462643

int main (int argc, char *argv[]) {
 
  
  int    proc_id,       // Process ID                           
         np,     	   // Number of processors                 
         llimit,        // Lower limit for random numbers       
         ulimit,        // Upper limit for random numbers       
         n_circle,      // Number of darts that hit the circle  
         i;             // Dummy/Running index                  
  long int ndarts=10000000/16;
 
  double pi_current,    // PI calculated by each WORKER         
         pi_sum,        // Sum of PI values from each WORKER    
         x,             // x coordinate, betwen -1 & +1         
         y,             // y coordinate, betwen -1 & +1         
         z,             // Sum of x^2 and y^2                   
         error,         // Error in calculation of PI           
         time1,         // Wall clock - start time              
         time2,         // Wall clock - end time
	  time3,	   // Wall clock - start communication
	  time4;	   // Wall clock - end communication

	struct timeval stime;

	llimit   = -1;
	ulimit   = 1;
	n_circle = 0;

	MPI_Init(&argc, &argv);
 
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	time1 = MPI_Wtime();

	gettimeofday(&stime, NULL);
	srand(stime.tv_usec * stime.tv_usec * stime.tv_usec * stime.tv_usec);


	for (i = 1; i <= ndarts; i++)
	{
		x = ((ulimit- llimit) * ((double) rand()/(double) RAND_MAX)) + llimit;
		y = ((ulimit- llimit) * ((double) rand()/(double) RAND_MAX)) + llimit;

		z=x*x+y*y;
		if (z <= 1.0)
		{
			n_circle++;
		}
	}

	pi_current = 4.0 * (double)n_circle/(double)ndarts;

	if(proc_id==0)
	pi_sum=pi_current;
	MPI_Barrier(MPI_COMM_WORLD);
	time3 = MPI_Wtime();

	MPI_Reduce(&pi_current, &pi_sum, 1, MPI_DOUBLE, MPI_SUM, 0 , MPI_COMM_WORLD);

	time4 = MPI_Wtime();

	pi_sum=pi_sum/np;


	time2 = MPI_Wtime();

	error = fabs((pi_sum - PI)/PI) * 100;

	if(proc_id==0)
      	{
       	printf("Known value of  PI                                 : %11.20e\n",PI);
		printf("Calculated value of pi is                          : %11.20e\n",pi_sum);
		printf("Number of processes is                             : %d\n",np);
	       printf("No. of iterations is                               : %ld\n",ndarts);      
       	printf("Time for the computation process is                : %lf\n",time2-time1);
       	printf("Time for the communication process is              : %lf\n",time4-time3);
	       printf("Percentage Error                                   : %11.10f\n", error);
       }

MPI_Finalize();
return 0;
}