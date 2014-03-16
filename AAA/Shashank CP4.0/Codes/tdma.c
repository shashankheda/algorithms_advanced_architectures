#include <stdio.h>
#define real double

int main()
{
  /* Matrix Size */
  int m = 15;
  
  /* Main Diagonal */
  real *b = (real*)malloc(m*sizeof(real));

  /* Lower Off Diagonal */
  real *a = (real*)malloc(m*sizeof(real));
  
  /* Upper Off Diagonal */
  real *c = (real*)malloc(m*sizeof(real));
  
  /* Right Hand Side */
  real *d = (real*)malloc(m*sizeof(real));

  /* Solution */
  real *x = (real*)malloc(m*sizeof(real));
  
  /* Initialize variables */
  int i;
  for ( i = 0 ; i < m ; i++)
    {
      b[i] = 2.0;
      a[i] = -1.0;
      c[i] = -1.0;
      d[i] = 1.0 ;
    }
  a[0]	= 0;
  c[m-1]= 0;
  
  /* Compute New Coefficients */
  for ( i = 1 ; i < m ; i++)
   {
    b[i] = c[i-1] - b[i]*b[i-1]/a[i];
    c[i] = -c[i]*b[i-1]/a[i];
    d[i] = d[i-1] - d[i]*b[i-1]/a[i];
   } 
  
  /* Back Substitution */
  x[m-1] = d[m-1]/b[m-1];
  
  for ( i = m-2 ; i >=0 ; i--)
    x[i] = d[i]/b[i]  -  c[i]*x[i+1]/b[i] ;
    
    
 /* Print Solution */
  for ( i = 0 ; i < m ; i++)
    printf("x[%d]=%lf\n",i,x[i]);  

  free(a);
  free(b);
  free(c);
  free(d);
       
 return 0;  
   }
    
      
