//by Shashank Heda
//Calculation of an integral using Trapezoidal Rule using Parallel Programming


#include<stdio.h>
#include<math.h>						// To incorporate usage of functions like sin, log etc.
#include<stdlib.h>						// For the function exit(1)
#include "mpi.h"						// To incorporate MPI Library
#include "mpe.h"

#define x(y) 	  sqrt((1-(y*y))/2)				// This is equation of plane where both 3d curves intersect
#define z1(x,y) (5*((x*x)+(y*y)))				// First surface curve
#define z2(x,y) (6-7*(x*x)-(y*y))				// Second Surface curve

int main(int argc, char* argv[])
{
	int my_rank, np, process;
	long int n=100,l=100;					// Total number of parts (in which y is divided)
	long int local_ay,local_y,local_x,width,local_local_volume,i,j;
	double ay1=-1.0 , ay2=1.0;
	double h, local_l1, local_l2,x,local_volume=0.0, total_volume;
	double time1,time2;
	//int *sendbuf, *recvbuf;

//MPI Starts
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&np);

		
		time1=MPI_Wtime();

		h = (ay1 - ay2) / n; 			// number of strips in which y axis is divided
		local_ay = n / np; 				// Number of strips given to each process

		local_l1 = ay1 + my_rank * local_ay * h;	// To determine starting point of integration for each process on the axis y
		local_l2 = local_l1 + local_ay * h;	// To determine the end point of integration for each process on the axis y

		
		*********************************************************************************************************
		for(j=0;j<l;j++)
			{
				local_volume=(z1(local_x*j/l, local_l1)+z1(local_x*j/l, local_l2)+z2(local_x*j/l, local_l1)+z2(local_x*j/l, local_l2);
			}

		for(i=1;i<local_ay;i++)			// To calculate the value of all f(x) inside the local interval of the function
		{
			local_y=local_l1+i*h;
			local_x=x(local_y);
			width=local_x/l;
			
			for(j=0;j<l;j++)
			{
				local_local_volume+=2*z1((local_x*j)/l, local_y);
				local_local_volume+=2*z2((local_x*j)/l, local_y);
			}

			local_volume+=local_local_volume*h*width;		// Value of integral = (Value of all f's * width h)

			//x=local_a+i*h;
			//local_integral+=f(x);
		}



		/* Add up the volumes calculated by each process */

		if (my_rank!=0)
		{
			 MPI_Send(&local_volume, 1, MPI_DOUBLE,0,0, MPI_COMM_WORLD);
		}

		else
		{
			total_volume = local_volume;
			for (process=1; process<np; process++)
			{
				MPI_Recv(&local_volume, 1, MPI_DOUBLE,process,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				total_volume+=local_volume;
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
			printf("\n\tValue of integration by the Trapezoidal Rule is		: %1.10f",total_volume);
			printf("\n\tTotal number of processes used in parallel is 		: %d",np);
			printf("\n\tTime taken for calculation in Parallel Processing is 	: %f\n",time2-time1);
		}

	MPI_Finalize();
	return 0;
}