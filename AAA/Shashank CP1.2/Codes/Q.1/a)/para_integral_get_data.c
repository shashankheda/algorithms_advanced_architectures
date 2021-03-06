#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include "mpe.h"
#define f(x,y) 6-7*(x*x)-(y*y)
#define g(x,y) 5*(x*x+y*y)
/* Calculate local integral  */
double Trap(float local_x1, float local_x2, long int local_n, long int n, double h, int my_rank);
void Get_data(float* a_ptr, float* b_ptr, long int* n_ptr, int my_rank, int p);

int main(int argc, char** argv)
{
  int         my_rank;          		/* My process rank           */
  int         p;                		/* The number of processes   */
  float       a = -((float)1/sqrt(2));          /* Left endpoint             */
  float       b =((float)1/sqrt(2)) ;           /* Right endpoint            */
  long int    n = 1000;     			/* Number of intervals       */
  double      h;                		/* Interval base length      */
  float       local_a;          		/* Left endpoint my process  */
  float       local_b;          		/* Right endpoint my process */
  long int    local_n;          		/* Number of intervals  for  */
                                		/* my calculation            */
  double      volume;         			/* Integral over my interval */
  double      total_volume;   			/* Total integral            */
  int         source;           		/* Process sending integral  */
  int         dest = 0;         		/* All messages go to 0      */
  int         tag = 0;
  MPI_Status  status;
  double startTime, endTime, timeDifference;

  /* Let the system do what it needs to start up MPI */
  MPI_Init(&argc, &argv);
  /* Get my process rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  /* Find out how many processes are being used */
  MPI_Comm_size(MPI_COMM_WORLD, &p);  

  if (my_rank==0)
	  startTime=MPI_Wtime();
  Get_data (&a, &b, &n, my_rank, p);
  h = (b - a) / n;    /* h is the same for all processes */
  local_n = n / p;    /* So is the number of intervals */

  /* Length of each process' interval of integration = local_n * h.
   * So my interval starts at: */
  local_a = a + my_rank * local_n * h;
  local_b = local_a + local_n * h;
  volume = Trap(local_a, local_b, local_n, n, h, my_rank);
  /* Add up the integrals calculated by each process */
  if (my_rank == 0)
  {
    total_volume = volume;
    for (source=1; source<p; source++)
    {
      MPI_Recv(&volume, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
      total_volume = total_volume + volume;
    }
  }
  else MPI_Send(&volume, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);

  if (my_rank==0)
  {
    endTime = MPI_Wtime();
    timeDifference = endTime - startTime;	  
  }

  /* Print the result */
  if(my_rank == 0)
  {
    printf("With n = %ld intervals, \n", n);
    printf("our estimate of the volume from %f to %f = %1.10f\n", a, b, total_volume);
    printf("Time taken for whole computation = %f seconds with no. of processors = %d\n", timeDifference, p);
  }  

  /* Shut down MPI */
  MPI_Finalize();
  
  return 0;
} /* main */

double Trap(float    local_x1   /* in */,
            float    local_x2   /* in */,
            long int local_n   /* in */,
		long int n   /* in */,
            double   h         /* in */,
	    int      my_rank   /* in */)
{
  float    local_y1,local_y2,h_y;
  double fintegral=0.0, gintegral=0.0, volume=0.0, tempfintegral=0.0, tempgintegral=0.0;
  double x,y;
  long int i,j;
  x=local_x1;

  for(i=1;i<local_n;i++)
  {
  	local_y2= sqrt(1-(2*x*x));
  	local_y1= -(local_y2);
	tempfintegral=f(x,local_y1)+f(x,local_y2)/2.0;
	y = local_y1;
	h_y=(local_y2-local_y1)/n;
	for(j=1;j<n;j++)
	{
		y=y+h_y;
		tempfintegral= tempfintegral + (6-7*(x*x)-(y*y));
	}
	tempfintegral= tempfintegral * h_y;
	fintegral = fintegral + tempfintegral;
	x=x+h;
  }
fintegral = fintegral * h;
  x=local_x1;
  for(i=1;i<local_n;i++)
  {
  	local_y2=sqrt(1-(2*x*x));
  	local_y1= -(local_y2);
	tempgintegral=g(x,local_y1)+g(x,local_y2)/2.0;
	y=local_y1;
	h_y=(local_y2-local_y1)/local_n;
	for(j=1;j<local_n;j++)
	{
		y=y+h_y;
		tempgintegral= tempgintegral + 5*(x*x+y*y);
	}
	tempgintegral = tempgintegral * h_y;
	gintegral = gintegral + tempgintegral;
	x=x+h;
  }
gintegral = gintegral * h;

  volume = fintegral - gintegral;
  return volume;
}

/* Function Get_data
 * Reads in the user input a, b, and n.
 * Input parameters:
 *     1.  int my_rank:  rank of current process.
 *     2.  int p:  number of processes.
 * Output parameters:  
 *     1.  float* a_ptr:  pointer to left endpoint a.
 *     2.  float* b_ptr:  pointer to right endpoint b.
 *     3.  int* n_ptr:  pointer to number of trapezoids.
 * Algorithm:
 *     1.  Process 0 prompts user for input and
 *         reads in the values.
 *     2.  Process 0 sends input values to other
 *         processes.
 */

void Get_data(
         float*    a_ptr    /* out */, 
         float*    b_ptr    /* out */, 
         long int* n_ptr    /* out */,
         int       my_rank  /* in  */, 
         int       p        /* in  */)
{
  int source = 0;    /* All local variables used by */
  int dest;          /* MPI_Send and MPI_Recv       */
  int tag;
  MPI_Status status;

  if(my_rank == 0) {
    printf("Enter a, b, and n\n");
    scanf("%f %f %ld", a_ptr, b_ptr, n_ptr);
    for(dest=1; dest<p; dest++) {
      tag = 0;
      MPI_Send(a_ptr, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
      tag = 1;
      MPI_Send(b_ptr, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
      tag = 2;
      MPI_Send(n_ptr, 1, MPI_LONG_INT, dest, tag, MPI_COMM_WORLD);
    }
  }
  else {
    tag = 0;
    MPI_Recv(a_ptr, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
    tag = 1;
    MPI_Recv(b_ptr, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
    tag = 2;
    MPI_Recv(n_ptr, 1, MPI_LONG_INT, source, tag, MPI_COMM_WORLD, &status);
  }
} /* Get_data */
