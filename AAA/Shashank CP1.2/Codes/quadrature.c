/* Parallel Trapezoidal Rule
 *
 * Output: Estimation of the integral from a to b of f(x)
 *         using the simpson's rule.
 *
 * Algorithm:
 *    1.  Each process calculates "its" interval of
 *        integration.
 *    2.  Each process estimates the integral of f(x)
 *        over its interval using the trapezoidal rule.
 *    3a. Each process != 0 sends its integral to 0.
 *    3b. Process 0 sums the calculations received from
 *        the individual processes and prints the result.
 *
 * Notes:  
 *    1.  f(x), a, b, n are all hardwired.
 *    2.  The number of processes (p) should divide evenly by
 *        the number of trapezoids (n = 10000000)
 */

#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include "mpe.h"
#define Z1(X,Y) (5*( (X)*(X) + (Y)*(Y) ))
#define Z2(X,Y) (6-7*(X)*(X)-(Y)*(Y))
#define HEIGHT(X,Y) (Z1(X,Y) - Z2(X,Y))

void roots(double*, double*, double);
int main(int argc, char** argv)
{
  int         my_rank;          /* My process rank           */
  int         p;                /* The number of processes   */
  double      ax,ay;            /* Left endpoint             */
  double      bx,by;            /* Right endpoint            */
  double      ht;               /* Difference in heights     */
  long int    nx,ny;            /* Number of trapezoids      */
  double      hx,hy;            /* Trapezoid base length     */
  double      local_ax;         /* Left endpoint my process  */
  double      local_bx;         /* Right endpoint my process */
  long int    local_nx;         /* Number of trapezoids for  */
                                /* my calculation            */
  double      integral=0;       /* Integral over my interval */
  double      total_integral=0; /* Total integral            */
  double      x,y;              /* Iterators for loops       */
  int         event1a, event1b;
  double      startTime, endTime, timeDifference;
  FILE        *fp;

  scanf("%ld %ld", &nx, &ny);

  /* Let the system do what it needs to start up MPI */
  MPI_Init(&argc, &argv);
  /* Get my process rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  /* Find out how many processes are being used */
  MPI_Comm_size(MPI_COMM_WORLD, &p);  

  MPI_Bcast(&nx, 1, MPI_LONG, 0, MPI_COMM_WORLD);
  MPI_Bcast(&ny, 1, MPI_LONG, 0, MPI_COMM_WORLD);

  event1a = MPE_Log_get_event_number(); 
  event1b = MPE_Log_get_event_number();

  MPE_Describe_state(event1a, event1b, "Compute", "red");

  if (my_rank==0)
 	  startTime=MPI_Wtime();

  ax=(-1)/sqrt(2); bx=-ax;
  hx = (bx - ax) / nx;  /* h is the same for all processes */
  local_nx = nx / p;    /* So is the number of trapezoids */

  /* Length of each process' interval of integration = local_n * h.
   * So my interval starts at: */
  MPE_Log_event(event1a, 0, "start compute");
  local_ax = ax + my_rank * local_nx * hx;
  local_bx = local_ax + local_nx * hx;
 
  for(x=local_ax; x<local_bx; x+=hx)
  {
    roots(&ay, &by, x);
    hy = (by-ay)/ny;
    for(y = ay; y < by; y += hy)
    {
      ht = HEIGHT(x,y);
      integral += ( (ht>0) ? ht : -ht) * hx * hy; 
    }
    //integral = integrate(local_a, local_b, local_n, h, ay, by);
  }
  MPE_Log_event(event1b, 0, "end compute");

  /* Add up the integrals calculated by each process */
  /*
  if (my_rank == 0)
  {
    total_integral = integral;
    for (source=1; source<p; source++)
    {
      MPI_Recv(&integral, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
      total_integral = total_integral + integral;
      printf("The integral by process %d is = %lf\n",source, integral);
    }
  }
  else MPI_Send(&integral, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
 */ 

  MPI_Reduce(&integral, &total_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (my_rank==0)
  {
    endTime = MPI_Wtime();
    timeDifference = endTime - startTime;	  
  }

  /* Print the result */
  if(my_rank == 0)
  {
    printf("With [nx ny] = [%ld %ld], total n = %ld, ", nx, ny, nx * ny);
    printf("our estimate of the quadrature is %.25lf\n", total_integral);
    printf("Time taken for whole computation = %.25lf seconds\n", timeDifference);
  
    fp = fopen("/home/ravikumar/AACA/Lab/CP1.2_codes/1/integral_db", "a");
    if ( fp != NULL )
      fprintf(fp,"%ld\t%ld\t%-16ld\t%.25lf\t%.25lf\n", nx, ny, nx * ny, total_integral, timeDifference);
    else
      printf("The File could not be opened up for writing.....!!\n");
    fclose(fp);
  }
  
  /* Shut down MPI */
  MPI_Finalize();
  
  return 0;
} /* main */

/********** Generate ay and by for given x *********/
void roots(double *ay, double *by, double x)
{
  double a;
  a = sqrt(1-2*x*x);
  if(a>0) { *ay = -a; *by = a;} 
  else { *ay = a; *by = -a;}
}
