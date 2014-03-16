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
#include <omp.h>

#define Z1(X,Y) (5*( (X)*(X) + (Y)*(Y) ))
#define Z2(X,Y) (6-7*(X)*(X)-(Y)*(Y))
#define HEIGHT(X,Y) (Z1(X,Y) - Z2(X,Y))

void roots(double*, double*, double);
int main(int argc, char** argv)
{
    double      ax,ay=0;            /* Left endpoint             */
    double      bx,by=0;            /* Right endpoint            */
    double      temp,ht;          /* Difference in heights     */
    long int    nx,ny;            /* Number of trapezoids      */
    double      hx,hy=0;            /* Trapezoid base length     */
    double      local_x=0;         /* Left endpoint my process  */
    /* my calculation            */
    double      integral=0;       /* Integral over my interval */
    double      y;  /* Iterators for loops       */
    int         iterx, x, threads;
    double      startTime, endTime, timeDifference;
    FILE        *fp;
  
    scanf("%d", &threads);
    scanf("%ld %ld", &nx, &ny);
  
    startTime=omp_get_wtime();
  
    ax=(-1)/sqrt(2); bx=-ax;
    hx = (bx - ax) / nx;  /* h is the same for all processes */

    /* Length of each process' interval of integration = local_n * h.
     * So my interval starts at: */

    iterx = (int)(( bx - ax ) / hx);

    omp_set_num_threads(threads);
#pragma omp parallel for reduction(+:integral) private(local_x, temp, ay ,by, ht, y)
    for(x=0; x<iterx; x++)
    {
	local_x = ax + x*hx;

	temp = sqrt(1-2*local_x*local_x);
	if(temp>0) ay = -temp; 
	else ay = temp;
	by = -ay;

	hy = (by-ay)/ny;
	for(y = ay; y < by; y += hy)
	{
	    ht = HEIGHT(local_x,y);
	    integral += ( (ht>0) ? ht : -ht) * hx * hy; 
	}
    }

    endTime = omp_get_wtime();
    timeDifference = endTime - startTime;	  
    
    
    /* Print the result */
    printf("With [nx ny] = [%ld %ld], total n = %ld, ", nx, ny, nx * ny);
    printf("our estimate of the quadrature is %.25lf\n", integral);
    printf("Time taken for whole computation = %.25lf seconds\n", timeDifference);
  
    fp = fopen("/home/ravikumar/AACA/Lab/P2.2/q3/omp_integral_db", "a");
    if ( fp != NULL )
	fprintf(fp,"%ld\t%ld\t%-16ld\t%.25lf\t%.25lf\n", nx, ny, nx * ny, integral, timeDifference);
    else
	printf("The File could not be opened up for writing.....!!\n");
    fclose(fp);
  
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
