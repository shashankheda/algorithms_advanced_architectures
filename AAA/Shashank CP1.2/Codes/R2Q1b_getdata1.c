/* get_data1.c -- Parallel Trapezoidal Rule; uses a hand-coded 
 *     tree-structured broadcast.
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

#include <stdio.h>
#include "mpi.h"
#include <math.h> 

#define f(x) x*x*x
#define g(x) (sqrt((double)(4+3*x*x)))*x
#define h(x) log(x + 1)/(x*x + 4*x + 3)
#define i(x) sqrt((double)(log(x + 1)/(x*x + 4*x + 3)))*x*x

void Get_data1(float* a_ptr, float* b_ptr, long int* n_ptr, int* choice_ptr, int my_rank, int p);
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
    double	  starttime, endtime ;
    MPI_Status  status;

    /* Let the system do what it needs to start up MPI */
    MPI_Init(&argc, &argv);
    /* Get my process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    /* Find out how many processes are being used */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    starttime=MPI_Wtime();    

    Get_data1(&a, &b, &n, &choice, my_rank, p);

    h = (b-a)/n;    /* h is the same for all processes */
    local_n = n/p;  /* So is the number of trapezoids */

    /* Length of each process' interval of 
     * integration = local_n*h.  So my interval
     * starts at: */
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    integral = Trap(local_a, local_b, local_n, h, choice);

    /* Add up the integrals calculated by each process */
    if(my_rank == 0) {
      total = integral;
      for(source=1; source<p; source++) {
        MPI_Recv(&integral, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
        total = total + integral;
      }
    }
    else MPI_Send(&integral, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);

    endtime = MPI_Wtime();

    /* Print the result */
    if(my_rank == 0) {
      printf("With n = %ld trapezoids, our estimate\n", n);
      printf("of the integral from %f to %f = %e\n", a, b, total); 
    }

    if(my_rank == 0) {
    printf("Total time taken for execution = %e\n",endtime-starttime );        
    }

    /* Shut down MPI */
    MPI_Finalize();
} /*  main  */


/********************************************************************/
/* Ceiling of log_2(x) is just the number of times
 * times x-1 can be divided by 2 until the quotient
 * is 0.  Dividing by 2 is the same as right shift.
 */
int Ceiling_log2(int  x)
{
    /* Use unsigned so that right shift will fill
     * leftmost bit with 0 */
    unsigned temp = (unsigned) x - 1;
    int result = 0;

    while(temp != 0) {
         temp = temp >> 1;
         result = result + 1 ;
    }
    return result;
} /* Ceiling_log2 */

/********************************************************************/
int I_receive(int stage, int my_rank, int* source_ptr)
{
    int power_2_stage;

    /* 2^stage = 1 << stage */
    power_2_stage = 1 << stage;
    if((power_2_stage <= my_rank) && 
            (my_rank < 2*power_2_stage)){
        *source_ptr = my_rank - power_2_stage;
        return 1;
    } else return 0;
} /* I_receive */

/********************************************************************/
int I_send(int stage, int my_rank, int p, int* dest_ptr)
{
    int power_2_stage;

    /* 2^stage = 1 << stage */
    power_2_stage = 1 << stage;  
    if(my_rank < power_2_stage){
        *dest_ptr = my_rank + power_2_stage;
        if (*dest_ptr >= p) return 0;
        else return 1;
    } else return 0;
} /* I_send */            


/********************************************************************/
void Send(float a, float b, long int n, int choice, int dest)
{
    MPI_Send(&a, 1, MPI_FLOAT, dest, 0, MPI_COMM_WORLD);
    MPI_Send(&b, 1, MPI_FLOAT, dest, 1, MPI_COMM_WORLD);
    MPI_Send(&n, 1, MPI_LONG, dest, 2, MPI_COMM_WORLD);
    MPI_Send(&choice, 1, MPI_INT, dest, 3, MPI_COMM_WORLD); 

} /* Send */

/********************************************************************/
void Receive(float* a_ptr, float* b_ptr, long int* n_ptr, int* choice_ptr, int source)
{
    MPI_Status status;

    MPI_Recv(a_ptr, 1, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(b_ptr, 1, MPI_FLOAT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(n_ptr, 1, MPI_LONG, source, 2,  MPI_COMM_WORLD, &status);
    MPI_Recv(choice_ptr, 1, MPI_INT, source, 3, MPI_COMM_WORLD, &status); 

} /* Receive */


/********************************************************************/
/* Function Get_data1
 * Reads in the user input a, b, and n.
 * Input parameters:
 *     1.  int my_rank:  rank of current process.
 *     2.  int p:  number of processes.
 * Output parameters:  
 *     1.  double* a_ptr:  pointer to left endpoint a.
 *     2.  double* b_ptr:  pointer to right endpoint b.
 *     3.  long int* n_ptr:  pointer to number of trapezoids.
       4.  int* func_choice_ptr: pointer to choice of functions. 
 * Algorithm:
 *     1.  Process 0 prompts user for input and
 *         reads in the values.
 *     2.  Process 0 sends input values to other
 *         processes using hand-coded tree-structured
 *         broadcast.
 */
void Get_data1(float*  a_ptr, float* b_ptr, long int* n_ptr, int* choice_ptr, int my_rank, int p)
{
    int source;
    int dest;
    int stage;

    int Ceiling_log2(int  x);
    int I_receive( int stage, int my_rank, int*  source_ptr);
    int I_send(int stage, int my_rank, int p, int* dest_ptr);
    void Send(float a, float b, long int n, int choice, int dest);
    void Receive(float* a_ptr, float* b_ptr, long int* n_ptr, int* choice_ptr, int source);

    if (my_rank == 0){
    printf("\nThe following are the functions available for integration:\n\n");
    printf("1. f(x) = x * x * x\n\n");
    printf("2. g(x) = (sqrt(4 + 3*x*x)) * x\n\n");
    printf("3. h(x) = log(x + 1)/(x*x + 4*x + 3)\n\n");
    printf("4. i(x) = sqrt((double)(log(x + 1)/(x*x + 4*x + 3)))*x*x\n\n"); 
    while (*choice_ptr<1 || *choice_ptr>4){
      printf("Please choose a function (Enter a number from 1-4):\n");
      scanf("%d", choice_ptr);
      if (*choice_ptr<1 || *choice_ptr>4){
        printf("Invalid choice!\n");
      }
    }
 
        printf("Enter a, b, and n\n");
        scanf("%f %f %ld", a_ptr, b_ptr, n_ptr);
    }
 
    for (stage = 0; stage < Ceiling_log2(p); stage++)
        if (I_receive(stage, my_rank, &source))
            Receive(a_ptr, b_ptr, n_ptr, choice_ptr, source);
        else if (I_send(stage, my_rank, p, &dest))
            Send(*a_ptr, *b_ptr, *n_ptr, *choice_ptr, dest);
} /* Get_data1*/


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
  else printf("Choice is not within range!\n"); 

  exit(1);

  return(0.0);

} /* funct */
