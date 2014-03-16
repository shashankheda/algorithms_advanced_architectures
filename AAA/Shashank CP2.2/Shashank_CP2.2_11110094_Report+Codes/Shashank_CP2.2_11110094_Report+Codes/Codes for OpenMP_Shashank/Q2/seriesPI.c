
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(){
    
    long int N=0;                   // total number of terms in series
    long int iter;
    double pi,sum=0, sign=1.0;      // the value of pi
    double sTime, eTime;
    int threads,q=0;
    
    scanf("%d", &threads);
    //printf("And you would like to use N terms of the series, where N(in millions) is:\t");
    scanf("%ld", &N); N *= 1000000;

    sTime = omp_get_wtime();
    omp_set_num_threads(threads);

    #pragma omp parallel for reduction( + : sum )
    for ( iter = 0 ; iter < N ; iter++ ){
	( iter % 2 ) ? ( sign = -1 ) : ( sign = 1 );
	sum = sum + ( sign / ( 2 * ( double ) iter + 1 ) );
	//(q++<=20) && printf("thread - %d\n", omp_get_thread_num());
	// sign = -1.0 * sign;
    }//end for

    pi = 4 * sum;
    eTime=omp_get_wtime();
    printf("The value of pi for %ld terms is: %.26lf\n",N, pi);
    printf("Time taken: %.10lf\n", eTime-sTime);

    return 0;
}//end int main
