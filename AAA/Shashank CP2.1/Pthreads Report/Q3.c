#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int thread_count;

double total_integral;
pthread_mutex_t mutex;

void* Thread_sum(void* rank); 
double f1(double x, double y);
double f2(double x, double y);

int main(int argc, char* argv[])
{
    clock_t start,stop;
    start=clock();

    long thread;
    pthread_t* thread_handles;
   
    
    thread_count = strtol(argv[1], NULL, 10);

    pthread_mutex_init(&mutex, NULL);
    thread_handles = malloc (thread_count*sizeof(pthread_t));
    
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Thread_sum, (void*) thread);
        
        
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
    
    pthread_mutex_destroy(&mutex);
    free(thread_handles);

    stop=clock();

    printf("\n\n Total computation time = %f seconds ", (float)(stop-start)/CLOCKS_PER_SEC);  
    printf(" Value of integral %f\n\n", total_integral);
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
      clock_t start_time, end_time;
      float seconds; 
      long my_rank = (long) rank;
      double x, y;
      int i,j;
      long long nx = 100000;
      long long ny = 100000;
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
      
      start_time = clock();
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
      end_time = clock();
      seconds = (float)(end_time - start_time)/CLOCKS_PER_SEC;
      printf("Calculation Time taken by thread %ld is %f seconds. \n", my_rank, seconds);  
      pthread_mutex_lock (&mutex);
      total_integral += integral;
      pthread_mutex_unlock(&mutex);
      
      return NULL;
}
