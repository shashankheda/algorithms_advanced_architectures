//by Shashank Heda
//Calculation of an integral using Trapezoidal Rule using Parallel Programming


#include<stdio.h>
#include<math.h>						// To incorporate usage of functions like sin, log etc.
#include<stdlib.h>						// For the function exit(1)
#include "mpi.h"						// To incorporate MPI Library
#include "mpe.h"

#define f(x) 4/(1+x*x)					// This function is used because it is known that integration of this function from 0 to 1 gives a value = pi


int main(int argc, char* argv[])
{
int my_rank, np, process;
long int n=10000,local_n,i;
double a=0.0 , b=1.0, h, local_a, local_b,x,local_integral, total_integral;
double time1,time2;
int *sendbuf, *recvbuf;
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
MPI_Comm_size(MPI_COMM_WORLD,&np);

		time1=MPI_Wtime();

		h = (b - a) / n; 				// h is the same for all processes
		local_n = n / np; 				// So is the number of trapezoids
		local_a = a + my_rank * local_n * h;	// To determine starting point of integration for each processor
		local_b = local_a + local_n * h;		// To determine the end point of integration for each processor

		local_integral=(f(local_a)+f(local_b))/2.0;

		for(i=1;i<local_n;i++)			// To calculate the value of all f(x) inside the local interval of the function
		{
			x=local_a+i*h;
			local_integral+=f(x);
		}

		local_integral=local_integral*h;		// Value of integral = (Value of all f's * width h)

		/* Add up the integrals calculated by each process */

		if (my_rank!=0)
		{
			 MPI_Send(&local_integral, 1, MPI_DOUBLE,0,0, MPI_COMM_WORLD);
		}

		else
		{
			total_integral = local_integral;
			for (process=1; process<np; process++)
			{
				MPI_Recv(&local_integral, 1, MPI_DOUBLE,process,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				total_integral+=local_integral;
			}
		}


/*Using MPI_Reduce
sendbuf = (int *)malloc( 3 * sizeof(int) );
recvbuf = (int *)malloc( 3 * sizeof(int) );
MPI_Barrier(MPI_COMM_WORLD);
MPI_Reduce( sendbuf, recvbuf, 3, MPI_INT, MPI_SUM,0,MPI_COMM_WORLD);
*/
		

		time2=MPI_Wtime();

		if(my_rank==0)
		{
			printf("\n\tValue of integration by the Trapezoidal Rule is		: %1.10f",total_integral);
			printf("\n\tTotal number of processes used in parallel is 		: %d",np);
			printf("\n\tTime taken for calculation in Parallel Processing is 	: %f\n",time2-time1);
		}

	MPI_Finalize();
	return 0;
}