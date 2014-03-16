//by Shashank Heda
// To calculate integral by Simpson's rule
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include "mpi.h"
#define f(x) 4/(1+x*x)
#define PI 3.1415926536


int main(int argc, char* argv[])
{
int my_rank;
long int i,n=1000, local_n;
int comm_sz;
double a=0.0 , b=1.0, h, local_a, local_b,x;
double local_int, total_int;
int source;
double t1,t2;
double error;

//scanf("%d",&n);
//scanf("%lf",&a);
//scanf("%lf",&b);


MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);

t1=MPI_Wtime();
h=(b-a)/n;
local_n=n/comm_sz;

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

	for(source=1;source<comm_sz;source++)
	{
		MPI_Recv(&local_int,1,MPI_DOUBLE,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		total_int+=local_int;
	}
}

t2=MPI_Wtime();
error = fabs((total_int - PI)/PI) * 100;
if(my_rank==0)
{
	printf("Value of integration by Simpson's rule is          : %11.10f\n",total_int);
       printf("Number of processes is                             : %d\n",comm_sz);
	printf("Time taken for calculation                         : %11.10f\n\n",t2-t1);
       printf("Known value of  PI                                 : %11.10f\n",PI);
	printf("Calculated value of pi is                          : %11.10f\n",total_int);
       printf("Percentage Error                                   : %10.8f\n", error);

}

MPI_Finalize();
return 0;
}