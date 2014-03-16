#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int thread_count;

double total_integral;
pthread_mutex_t mutex;
long int nx;
long int ny;


void* Thread_sum(void* rank); 
double f1(double x, double y);
double f2(double x, double y);

int main(int argc, char* argv[])
{
    
    long thread;
    pthread_t* thread_handles;
    clock_t stime, etime;
    double Ttime, Ttime2;

    
    /*
      struct timeval{
      time_t tv_sec;
      suseconds_t tv_usec;
      };
    */
    struct timeval t1, t2;

    
    //thread_count = strtol(argv[1], NULL, 10);
    scanf("%d", &thread_count);

    scanf("%ld %ld", &nx, &ny);

    stime = clock();
    gettimeofday(&t1, 0);

    pthread_mutex_init(&mutex, NULL);
    thread_handles = malloc (thread_count*sizeof(pthread_t));
    
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Thread_sum, (void*) thread);
        
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
    
    pthread_mutex_destroy(&mutex);
    free(thread_handles);
    
    etime = clock();
    gettimeofday(&t2, 0);
    Ttime = (double) (etime-stime)/CLOCKS_PER_SEC;
    if (t2.tv_usec > t1.tv_usec)
	Ttime2 = (double) ((t2.tv_sec - t2.tv_sec)*1000000 + t2.tv_usec - t1.tv_usec)/1000000;
    else
	Ttime2 = (double) ((t2.tv_sec - t2.tv_sec)*1000000 + t1.tv_usec - t2.tv_usec)/1000000;
    
    printf("Integral: %f\t Elements: %ld\nThreads: %d\t", total_integral, nx*ny, thread_count);
    printf("CPU Time: %.10lf\t Clock_time: %.10lf\n", Ttime, Ttime2);
    return 0;
}

double f1 (double x, double y)
{
  double val;
  
  val = 5*(x*x + y*y);
  return val;
}

double f2 (double x, double y)
{
  double val;
  
  val = 6 - 7*x*x - y*y;
  return val;
}

void* Thread_sum(void* rank)
{     
      long my_rank = (long) rank;
      double x, y;
      int i,j;
      long long my_nx = nx/thread_count;
      long long my_first_nx = my_nx*my_rank;
      long long my_last_nx = my_first_nx + my_nx;
      double xmin = -1;
      double xmax = 1;
      double ymin = -2;
      double ymax = 2;
      double hx = (xmax - xmin)/nx;
      double hy = (ymax - ymin)/ny; 
      double integral = 0;
      

      for (i = my_first_nx; i <= my_last_nx; i++)
        {
          x = xmin + i*hx;
         for (j = 0; j < ny; j++)
          {
            y = ymin + j*hy;

            if ( f1(x,y) > f2(x,y))
              {}
            else
              integral = integral + (f2(x,y) - f1(x,y))*hx*hy;
          }
        }

      pthread_mutex_lock (&mutex);
      total_integral += integral;
      pthread_mutex_unlock(&mutex);
      
      return NULL;
}
