//by Shashank Heda
/* Serial Code for calculating definite integral using the trapezoidal rule.
 * The function f(x) is hardwired.
 * Input : a, b, n.
 * Output: estimate of the integral from a to b of f(x) using n trapezoids.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>
#define f(x) log(x + 1)/(x*x + 4*x + 3)

int main()
{
  double integral;  /* Store end-result in integral         */
  float a, b;       /* Left and right endpoints             */
  long int n;       /* Number of trapezoids                 */
  double h;         /* Trapezoid base width                 */
  double x,timer;
  long int i;
  clock_t start_t, stop_t;

  a = 0.0;
  b = 30.0;
  n = 10000000;
  
  start_t = clock();
  
  h = (b - a) / n;
  integral = (f(a) + f(b)) / 2.0;
  x = a;
  for (i=1; i<n; i++)
  {
    x += h;
    integral += f(x);
  }
  integral *= h;
  
  stop_t = clock();
  timer = (double)((stop_t - start_t) / CLOCKS_PER_SEC);

  printf("With n = %ld trapezoids,\n", n);
  printf("our estimate of the integral from %.2f to %.2f = %e.\n", a, b, integral);
  printf("CPU time elapsed: %lf seconds\n", timer);
  
  return 0;
}  /* main */
