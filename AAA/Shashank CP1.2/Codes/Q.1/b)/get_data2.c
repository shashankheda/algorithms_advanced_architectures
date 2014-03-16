/* get_data2.c -- Parallel Trapezoidal Rule.  Uses 3 calls to MPI_Bcast to
 *     distribute input data.
 *
 * Input: 
 *    a, b: limits of integration.
 *    n: number of trapezoids.
 * Output:  Estimate of the integral from a to b of f(x) 
 *    using the trapezoidal rule and n trapezoids.
 *
 * Notes:  
 *    1.  f(x) is hardwired.
 *    2.  the number of processes (p) should evenly divide
 *        the number of trapezoids (n).
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h> 
#include "mpi.h"

#define f(x) x*x*x
#define g(x) (sqrt((double)(4+3*x*x)))*x
#define h(x) log(x + 1)/(x*x + 4*x + 3)
#define i(x) sqrt((double)(log(x + 1)/(x*x + 4*x + 3)))*x*x

void Get_data2(float* a_ptr, float* b_ptr, long int* n_ptr, int* choice_ptr, int my_rank);
double Trap(float local_a, float local_b, long int local_n, double h, int choice);
double funct(int choice, double x);

void main(int argc, char** argv)
{
    int         my_rank;   /* My process rank           */
    int         p;         /* The number of processes   */
    float       a;         /* Left endpoint             */
    float       b;         /* Right endpoint            */
    long int    n;         /* Number of trapezoids      */
    double      h;         /* Trapezoid base length     */
    int         choice;    /* User function choice      */ 
    float       local_a;   /* Left endpoint my process  */
    float       local_b;   /* Right endpoint my process */
    int         local_n;   /* Number of trapezoids for  */
                           /* my calculation            */
    double      integral;  /* Integral over my interval */
    double      total;     /* Total integral            */
    int         source;    /* Process sending integral  */
    int         dest = 0;  /* All messages go to 0      */
    int         tag = 0;
    MPI_Status  status;
    double tstart, tend, texec;

    /* Let the system do what it needs to start up MPI */
    MPI_Init(&argc, &argv);
    /* Get my process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    /* Find out how many processes are being used */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    tstart=MPI_Wtime();
    Get_data2(&a, &b, &n, &choice, my_rank);

    h = (b - a) / n;    /* h is the same for all processes */
    local_n = n / p;    /* So is the number of trapezoids */

    /* Length of each process' interval of integration = local_n * h.
     * So my interval starts at: */
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;
    integral = Trap(local_a, local_b, local_n, h, choice);

    /* Add up the integrals calculated by each process */
    if(my_rank == 0) {
      total = integral;
      for(source=1; source<p; source++) {
        MPI_Recv(&integral, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
        total += integral;
      }
    }
    else MPI_Send(&integral, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);

    tend=MPI_Wtime();
    texec = tend - tstart;
    /* Print the result */
    if(my_rank == 0) {
      printf("With n = %ld trapezoids, our estimate\n", n);
      printf("of the integral from %f to %f = %e\n", a, b, total);
      printf("Time taken to execute the program is %1.10f seconds with no. of processors = %d\n", texec, p);
    }

    /* Shut down MPI */
    MPI_Finalize();
} /*  main  */

/********************************************************************/
/* Function Get_data2
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
 *         processes using three calls to MPI_Bcast.
 */
void Get_data2(float* a_ptr, float* b_ptr, long int* n_ptr,
               int* choice_ptr, int my_rank)
{
    if(my_rank == 0) {
      printf("\nThe following are the functions available for integration:\n\n");
      printf("1. f(x) = x * x * x\n\n");
      printf("2. g(x) = (sqrt(4 + 3*x*x)) * x\n\n");
      printf("3. h(x) = log(x + 1)/(x*x + 4*x + 3)\n\n");
      printf("4. i(x) = sqrt((double)(log(x + 1)/(x*x + 4*x + 3)))*x*x\n\n"); 
      while(*choice_ptr<1 || *choice_ptr>4) {
        printf("Please choose a function (Enter a number from 1-4):\n");
        scanf("%d", choice_ptr);
        if(*choice_ptr<1 || *choice_ptr>4) printf("Invalid choice!\n");
    }

      printf("Enter a, b, and n\n");
      scanf("%f %f %ld", a_ptr, b_ptr, n_ptr);
    }

    MPI_Bcast(a_ptr, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b_ptr, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(n_ptr, 1, MPI_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(choice_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD); 
} /* Get_data2 */

/********************************************************************/
double Trap(float local_a, float local_b, long int local_n, 
            double h, int choice)
{ 
    double integral;   /* Store result in integral  */ 
    double x; 
    long int i; 

    integral = (funct(choice, local_a) + funct(choice, local_b))/2.0; 
    x = local_a; 
    for(i=1; i<=local_n-1; i++) { 
        x = x + h; 
        integral += funct(choice, x); 
    } 

    integral *= h; 
    return integral;
} /*  Trap  */

/********************************************************************/
double funct(int choice, double x) 
{ 
  if(choice == 1)      return(f(x));
  else if(choice == 2) return(g(x));
  else if(choice == 3) return(h(x));
  else if(choice == 4) return(i(x));
  else printf("Choice is not within range!\n"); exit(1);

  return(0.0);

} /* funct */
