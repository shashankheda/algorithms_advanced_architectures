//by Shashank Heda
//Program to Estimate bandwidth and message latency on the HPC Cluster

#include<stdio.h>
#include<stdlib.h>				//for the function exit
#include "mpi.h"
#include"mpe.h"

int main(int argc, char* argv[])
{
      int my_rank,np,i,j,n;
      float data[10000];
      double time1,time2;
      
	//Starting MPI
       MPI_Init(&argc,&argv);
       MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
       MPI_Comm_size(MPI_COMM_WORLD,&np);
 
       if(np!=2)
       {
               printf("\n\n\tSorry!!! The number of processes has to be 2\n");
               exit(1);
       }
       
	for(i=1;i<=100;i++)
	{
		n=100*i;
      
		time1=MPI_Wtime();
	       for(j=0;j<10000;j++)
      		{
                       if(my_rank==0)
                       {                                
                                 MPI_Send(data,n,MPI_FLOAT,1,0,MPI_COMM_WORLD);
                                 MPI_Recv(data,n,MPI_FLOAT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);		
                       }
                       else
                       {
                                 MPI_Recv(data,n,MPI_FLOAT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                                 MPI_Send(data,n,MPI_FLOAT,0,0,MPI_COMM_WORLD);
					
                       }
      		}

	       time2=MPI_Wtime();
       	
		if(my_rank==0)
		{ 
		       printf("\nTime for the Ping-Pong process between two processors is %lf for size=%d for the variable float\n",time2-time1,n);
		}
	}

      
      
      MPI_Finalize();
      exit(1);
}