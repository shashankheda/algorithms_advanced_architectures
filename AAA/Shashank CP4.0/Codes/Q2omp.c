#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<sys/time.h>
#include <omp.h>
#define real double

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}

void timeval_print(struct timeval *tv)
{
    char buffer[30];
    time_t curtime;

    printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
    curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    printf(" = %s.%06ld\n", buffer, tv->tv_usec);
}

int main()
{
  struct timeval tvBegin1, tvBegin2, tvBegin3, tvBegin4, tvBegin5, tvBegin6, tvDiff1, tvDiff2, tvDiff3, tvDiff4, tvDiff5, tvDiff6;;

  /* Matrix Size */
  int m;

  printf("\nEnter matrix size\n");
  scanf("%d",&m);

  gettimeofday(&tvBegin1, NULL);
  
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
  
  gettimeofday(&tvBegin2, NULL);

  /* Initialize variables */
  int i;
#pragma omp parallel for
  for ( i = 0 ; i < m ; i++)
    {
      b[i] = 2.0;
      a[i] = -1.0;
      c[i] = -1.0;
      d[i] = 1.0 ;
    }
  a[0]	= 0;
  c[m-1]= 0;
  
  gettimeofday(&tvBegin3, NULL);

  /* Compute New Coefficients */
#pragma omp parallel for
  for ( i = 1 ; i < m ; i++)
   {
    b[i] = c[i-1] - b[i]*b[i-1]/a[i];
    c[i] = -c[i]*b[i-1]/a[i];
    d[i] = d[i-1] - d[i]*b[i-1]/a[i];
   } 
  
  gettimeofday(&tvBegin4, NULL);

  /* Back Substitution */
  x[m-1] = d[m-1]/b[m-1];

#pragma omp parallel for
  for ( i = m-2 ; i >=0 ; i--)
    x[i] = d[i]/b[i]  -  c[i]*x[i+1]/b[i] ;
    
    
 /* Print Solution
#pragma omp parallel for
  for ( i = 0 ; i < m ; i++)
    printf("x[%d]=%lf\n",i,x[i]);  */

  gettimeofday(&tvBegin5, NULL);

  free(a);
  free(b);
  free(c);
  free(d);

  gettimeofday(&tvBegin6, NULL);
 
    timeval_subtract(&tvDiff1, &tvBegin2, &tvBegin1);
    timeval_subtract(&tvDiff2, &tvBegin3, &tvBegin2);
    timeval_subtract(&tvDiff3, &tvBegin4, &tvBegin3);
    timeval_subtract(&tvDiff4, &tvBegin5, &tvBegin4);
    timeval_subtract(&tvDiff5, &tvBegin6, &tvBegin5);
    timeval_subtract(&tvDiff6, &tvBegin6, &tvBegin1);

  printf("\nMatrix Size = %d",m);
  printf("Time taken in seconds		:%ld.%06ld",tvDiff6.tv_sec,tvDiff6.tv_usec);
  printf("\n1.Memory Allocation		:%ld.%06ld",tvDiff1.tv_sec,tvDiff1.tv_usec);
  printf("\n2.Variable Initialisation	:%ld.%06ld",tvDiff2.tv_sec,tvDiff2.tv_usec);
  printf("\n3.Coefficient Computation	:%ld.%06ld",tvDiff3.tv_sec,tvDiff3.tv_usec);
  printf("\n4.Back Substitution		:%ld.%06ld",tvDiff4.tv_sec,tvDiff4.tv_usec);
  printf("\n5.Freeing memory		:%ld.%06ld\n",tvDiff5.tv_sec,tvDiff5.tv_usec);

       
 return 0;  
   }