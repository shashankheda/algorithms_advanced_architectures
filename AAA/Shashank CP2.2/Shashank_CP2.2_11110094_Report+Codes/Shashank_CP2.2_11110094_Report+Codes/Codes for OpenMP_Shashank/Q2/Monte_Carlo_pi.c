/* Find the value of pi using Monte Carlo's method. A circle of radius r
 * is inscribed in a square of side '2r'. We throw N darts, of which 
 * only n fall into the circle. The ratio 4*(n/N) gives the value of pi.
*/


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

//float dist(float, float, float, float);

int main(){
    
    long int N,n=0;     // total number of darts (N) and darts inside circle (n)
    long int iter;
    double pi;          // the value of pi
    float radius,side;  // dimensions of square and circle
    float x, y;         // coordinates of the darts
    double sTime, eTime;
    int threads;
    
    //printf("The radius of the circle is:\t");
    //scanf("%f", &radius);
    //printf("%f\n",radius);
    radius=8;
    side = 2 * radius;

    scanf("%d", &threads);
    //printf("And you would like to throw N darts in it, where N(in millions) is:\t");
    scanf("%ld", &N); N *= 1000000;
    //printf("%ld\nWorking...\n", N);

    /* throw N random darts */
    srand( time(NULL) );
    
    sTime = omp_get_wtime();
    
    omp_set_num_threads(threads);
    #pragma omp parallel for reduction(+:n)
    for(iter=0; iter<N; iter++){
	// find coordinates of dart
	x = rand()/(float)RAND_MAX * side;
	y = rand()/(float)RAND_MAX * side;
	//printf("(%f,%f)-(%f,%f) = %f\n", x, y, radius, radius, dist(x, y, radius, radius));
	(sqrt( pow(x-radius,2) + pow(y-radius,2) ) <= radius ) && n++;
        //( dist(x,y,radius,radius) <= radius ) && n++; // if dart is inside circle, count it 
    }//end for

    pi = 4 * ( (float)n/(float)(N) );
    eTime=omp_get_wtime();
    printf("The value of pi for %ld darts is: %.26lf\n",N, pi);
    printf("Time taken: %.10lf\n", eTime-sTime);

    return 0;
}//end int main

/*
 * Function to measure distance between two points in a plane
 *
float dist(float x, float y, float p, float q){
    return sqrt( pow(x-p,2) + pow(y-q,2) );
}//end float dist
 */
