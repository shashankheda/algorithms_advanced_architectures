/* ROW STRIPED MVM
AUTHOR: VRG | DATE: 8th Feb 2013
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
 
#include "mpi.h"
#define AROW 16         // NUMBER OF ROWS OF MATRIX A DEFINED
#define ACOL 16         // NUMBER OF COLUMNS OF MATRIX A DEFINED
#define MAX_VALUE 10    // MAX VALUE OF MATRIX OR VECTOR ELEMENTS

int main(int argc, char* argv[])
{

int my_rank, 
    size,                   //SIZE OF THE COMMUNICATOR
    n,i,j,                  // MATRIX ORDER, COUNTERS i AND j
    a[AROW][ACOL],          // MATRIX A OF ORDER AROW x ACOL
    x[ACOL],c[AROW],    	// VECTOR X TO BE MULTIPLIED TO A AND THE VECTOR C THAT WILL BE THE RESULT OF MULTIPLICATION
    burden=AROW/8,          // NUMBER OF ROWS THAT EACH PROCESSOR WILL HAVE
    temp,                   // TEMPORARY VARIABLE
    buffer[burden][ACOL];   // INTERMEDIATE MATRIX THAT WILL STORE THE DECOMPOSED MATRIX FOR EACH PROCESSOR

double t2,t1;

for(i=0;i<AROW;i++)         //INITIALISE C TO ZERO
	c[i]=0;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
MPI_Comm_size(MPI_COMM_WORLD,&size);
t1=MPI_Wtime();                                                     //START TIME
if (my_rank == 0)
    {
        
 
        /* Generating Random Values for A Array*/
        for (i=0;i<AROW;i++)
        {
            for (j=0;j<ACOL;j++)
            {
                a[i][j] = 1;
            }
        }
 
        /* Printing the Matrix
 
        printf("Matrix A :\n");
        for (i=0;i<AROW;i++)
        {
            for (j=0;j<ACOL;j++)
            {
                printf("%3d ", a[i][j]);
            }
            printf("\n");
        }*/

        printf("\nMatrix X :\n");
        for (i=0;i<ACOL;i++)
        {
            printf("%3d ", x[i]);
        }
        printf("\n\n");

}

if(my_rank==0)
{
	
	for (j=1;j<size;j++)
        {
            MPI_Send(x, ACOL, MPI_INTEGER, j, 99, MPI_COMM_WORLD);              // SEND THE ENTIRE VECTORS TO ALL THE PROCESSORS
        }

	for(i=1;i<size;i++)
	{
		temp=i*burden;
		MPI_Send(&a[temp][0],ACOL*burden,MPI_INT,i,0,MPI_COMM_WORLD);      //SEND THE RESPECTIVE ELEMENTS OF A TO PROCESSES 1 TO SIZE-1
	}

                                  	
	for(i=0;i<burden;i++)                                                     //PROCESS ZERO DOES ITS COMPUTATION
		{
		for(j=0;j<ACOL;j++)
				c[i]=c[i]+a[i][j]*x[j];
		}

	
		
	for(i=1;i<size;i++)                                                       //PROCESS ZERO RECIEVES THE ANSWERS FROM THE OTHER PROCESSES
		{
		temp=i*burden;
		MPI_Recv(&c[temp],burden,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);		
		}
}

else
{
 	MPI_Recv(x, ACOL, MPI_INTEGER, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);   // RECIEVE X VECTOR FROM PROCESS ZERO
	MPI_Recv(buffer,ACOL*burden,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);  // RECIEVE THE ROW DECOMPOSED MATRIX FROM PROCESS ZERO


	for(i=0;i<burden;i++)
		for(j=0;j<ACOL;j++)
				c[i]=c[i]+buffer[i][j]*x[j];                          
	MPI_Send(c,burden,MPI_INT,0,0,MPI_COMM_WORLD);

}
t2=MPI_Wtime();
if(my_rank==0)
{
	for(i=0;i<ACOL;i++)
		printf("c[%d]=%d\n",i,c[i]);
	printf("Time for execution=%lf\n",t2-t1);
}

MPI_Finalize();
    return 0;
}
