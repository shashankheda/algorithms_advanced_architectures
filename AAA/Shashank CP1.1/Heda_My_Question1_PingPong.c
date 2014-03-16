//by Shashank Heda
//Question 1 Ping Pong between two processors

#include <stdio.h>
#include<stdlib.h>					//For the function exit(1)
#include "mpi.h"
#include "mpe.h"

int main(int argc, char** argv)
{
	int my_rank;
	int p;
	int received_rank;
	double time1, time2;
	
	MPI_Init(&argc, &argv);						// Start up MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);				// Get my process rank
	MPI_Comm_size(MPI_COMM_WORLD, &p);					// To find number of processes are being used

	time1 = MPI_Wtime();

	// Checking if number of processes is equal to 2
	if (p!=2)
	{
		if (my_rank==0)
			printf("The number of processes must be 2\n");
		
		exit(1);
	}
	
	
	// Start pingpong-ing between the processors
	
		if (my_rank == 0)
		{
			MPI_Send(&my_rank,1,MPI_INT,1,0, MPI_COMM_WORLD);
			printf("\nProcessor 0 sends %d to processor 1\n",my_rank);
			MPI_Recv(&received_rank,1,MPI_INT,1,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			printf("\nProcessor 0 receives %d from Processor 1\n",received_rank);
		}
		else if (my_rank == 1)
		{
			
			MPI_Send(&my_rank,1,MPI_INT,0,2, MPI_COMM_WORLD);
			printf("\nProcessor 1 sends %d to processor 0\n",my_rank);
MPI_Recv(&received_rank,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			printf("\nProcessor 1 receives %d from Processor 0\n",received_rank);
		}
	time2 = MPI_Wtime();
	printf("\nTotal Time Elapsed: %lf\n\n\n",time2-time1);
	/* Shut down MPI */
	MPI_Finalize();
	
	return 0;
}