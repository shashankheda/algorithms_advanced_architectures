//by Shashank Heda
// To calculate integral by Simpson's rule
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include "mpi.h"
#include "mpe.h"
#define f(x) log(x + 1)/(x*x + 4*x + 3)


int main(int argc, char* argv[])
{
	int my_rank,np,process;
	long int i,n=1000, local_n;
double a=0.0 , b=30.0, h, local_a, local_b,x;
double local_int, total_int;
double time1,time2,error;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
MPI_Comm_size(MPI_COMM_WORLD,&np);

time1=MPI_Wtime();
h=(b-a)/n;
local_n=n/np;

local_a = a + my_rank*local_n*h;
local_b = local_a + local_n*h;


local_int=f(local_a)+f(local_b);

x=local_a;
for(i=1;i<local_n;i=i+2)
{
x=local_a+i*h;
local_int = local_int + 4*f(x);
}

x=local_a;
for(i=2;i<local_n-1;i=i+2)
{
x=local_a+i*h;
local_int = local_int + 2*f(x);
}

local_int=local_int*h/3.0;

if(my_rank!=0)
{
	MPI_Send(&local_int,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
}
else
{
	total_int=local_int;

	for(process=1;process<np;process++)
	{
		MPI_Recv(&local_int,1,MPI_DOUBLE,process,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		total_int+=local_int;
	}
}

time2=MPI_Wtime();

if(my_rank==0)
{
	printf("\n\tValue of integration by Simpson's rule is          : %e",total_int);
       printf("\n\tNumber of processes is                             : %d",np);
	printf("\n\tTime taken for calculation                         : %e",time2-time1);
}

MPI_Finalize();
return 0;
}