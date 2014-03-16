#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include "mpe.h"
#define f(x,y) 6-7*(x*x)-(y*y)
#define g(x,y) 5*(x*x+y*y)
/* Calculate local integral  */
double Trap(float local_x1, float local_x2, long int local_n, long int n, double h, int my_rank);

void Build_derived_type(
         float*         a_ptr           /* in   */,
         float*         b_ptr           /* in   */,
         long int*           n_ptr           /* in   */,
         MPI_Datatype*  mesg_mpi_t_ptr  /* out  */);
void Get_data3(
         float*  a_ptr    /* out */,
         float*  b_ptr    /* out */,
         long int*    n_ptr    /* out */,
         int     my_rank  /* in  */);
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
  Get_data3 (&a, &b, &n, my_rank);
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

/********************************************************************/
void Build_derived_type(
         float*         a_ptr           /* in   */,
         float*         b_ptr           /* in   */,
         long int*           n_ptr           /* in   */,
         MPI_Datatype*  mesg_mpi_t_ptr  /* out  */) {
                        /* pointer to new MPI type */

    /* The number of elements in each "block" of the   */
    /*     new type.  For us, 1 each.                  */
    int block_lengths[3];      

    /* Displacement of each element from start of new  */
    /*     type.  The "d_i's."                         */   
    /* MPI_Aint ("address int") is an MPI defined C    */
    /*     type.  Usually an int.                      */
    MPI_Aint displacements[3];

    /* MPI types of the elements.  The "t_i's."        */
    MPI_Datatype typelist[3];  
                              
    /* Use for calculating displacements               */
    MPI_Aint start_address; 
    MPI_Aint address;

    block_lengths[0] = block_lengths[1] 
                     = block_lengths[2] = 1;

    /* Build a derived datatype consisting of  */
    /* two floats and an int                   */
    typelist[0] = MPI_FLOAT;
    typelist[1] = MPI_FLOAT;
    typelist[2] = MPI_INT;

    /* First element, a, is at displacement 0      */
    displacements[0] = 0;

    /* Calculate other displacements relative to a */
    MPI_Address(a_ptr, &start_address);

    /* Find address of b and displacement from a   */
    MPI_Address(b_ptr, &address);
    displacements[1] = address - start_address;

    /* Find address of n and displacement from a   */
    MPI_Address(n_ptr, &address);
    displacements[2] = address - start_address;

    /* Build the derived datatype */
    MPI_Type_struct(3, block_lengths, displacements, 
        typelist, mesg_mpi_t_ptr);

    /* Commit it -- tell system we'll be using it for */
    /* communication.                                 */
    MPI_Type_commit(mesg_mpi_t_ptr);
} /* Build_derived_type */

/********************************************************************/
void Get_data3(
         float*  a_ptr    /* out */,
         float*  b_ptr    /* out */,
         long int*    n_ptr    /* out */,
         int     my_rank  /* in  */) {
    MPI_Datatype  mesg_mpi_t; /* MPI type corresponding */
                              /* to 3 floats and an int */
    
    if (my_rank == 0){
        printf("Enter n\n");
        scanf("%ld", n_ptr);
    }

    Build_derived_type(a_ptr, b_ptr, n_ptr, &mesg_mpi_t);
    MPI_Bcast(a_ptr, 1, mesg_mpi_t, 0, MPI_COMM_WORLD);
} /* Get_data3 */


/********************************************************************/
