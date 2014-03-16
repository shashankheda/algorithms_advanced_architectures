#include <stdio.h>
#define real double
//=======================================================
// Solves the Tridiagonal System of Equations Ax=B
//      Using the method of cyclic reduction
//                     dt:21072009
//=======================================================

int main()
{
  /* Matrix Size */
  
  int m = 7, q= 3, nt;
  
  /* Main Diagonal */
  real *b = (real*)malloc(m*q*sizeof(real));
  
  /* Lower Off Diagonal */
  real *a = (real*)malloc(m*q*sizeof(real));
  
  /* Upper Off Diagonal */
  real *c = (real*)malloc(m*q*sizeof(real));
  
  /* Right Hand Side */
  real *d = (real*)malloc(m*q*sizeof(real));
  
   /* Solution */
  real *x = (real*)malloc(m*sizeof(real));
  
  /* Initialize variables , This is the first set and unchanged coefficients*/
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
  
  /* Compute New Coefficients at the next level nt+1 */
  /* The Inner Loop can be perfomed in parallel easily */
 for ( nt = 0 ; nt < q-1 ; nt++)
  {
  for ( i = 0 ; i < pow(2,q-nt-1)-1; i++)
   {
    a[(nt+1)*m+i] = a[nt*m+2*i];
    b[(nt+1)*m+i] = a[nt*m+2*i+2]*b[nt*m+2*i]*c[nt*m+2*i+1]/(a[nt*m+2*i+1]*b[nt*m+2*i+2])-b[nt*m+2*i+1]*b[nt*m+2*i]/a[nt*m+2*i+1]+c[nt*m+2*i];
    c[(nt+1)*m+i] = c[nt*m+2*i+2]*c[nt*m+2*i+1]*b[nt*m+2*i]/(a[nt*m+2*i+1]*b[nt*m+2*i+2]);
    d[(nt+1)*m+i] = d[nt*m+2*i+2]*c[nt*m+2*i+1]*b[nt*m+2*i]/(a[nt*m+2*i+1]*b[nt*m+2*i+2])-d[nt*m+2*i+1]*b[nt*m+2*i]/a[nt*m+2*i+1]+d[nt*m+2*i]; 
   }
   
  }
  
  /* Back Substitution Phase*/
  /* This is the 'Middle' Solution  */
    x[ (m+1)/2 -1 ] = d[(q-1)*m+0]/b[(q-1)*m+0];
  
  /*Now Using this, we compute rest of the solutions*/
  /* The inner loop can be performed easily in parallel*/
  
  for ( nt = q-2 ; nt >=0 ; nt--) // Go in the reverse order:start from last but one state  
  {
   for ( i = pow(2,nt)-1; i <= pow(2,q)-pow(2,nt)-1; i+=pow(2,nt+1) )
    { 
      int ind1 = (i+1)/pow(2,nt) - 1;
      int ind2 =  i + pow(2,nt);
      int ind3 =  i - pow(2,nt);
      x[i] = (d[nt*m+ind1] - c[nt*m+ind1]*x[ind2] - a[nt*m+ind1]*x[ind3])/b[nt*m+ind1] ;  
    }
      
  }
 
  // Print Solution 
  for ( i = 0 ; i < m ; i++) 
  printf("x[%d]=%lf\n", i , x[i]);
 
 
 
  free(a); 
  free(b);
  free(c); 
  free(d); 
       
 return 0;  
   }
    
      
