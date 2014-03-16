//Program to calculate value of pi using given thumb rule

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#define PI      3.141592653589793238462643

int thread_count;

# define n 10000000000

double sum;
clock_t starttime,endtime;
pthread_mutex_t mutex;

void* Thread_sum(void* rank); 

int main(int argc, char* argv[])
{   
    starttime=clock(); 
    long thread;
    pthread_t* thread_handles;
    
    double error;
    //To maintain thread count
    thread_count = strtol(argv[1], NULL, 10);

    //Mutex initialisation
    pthread_mutex_init(&mutex, NULL);
    thread_handles = malloc (thread_count*sizeof(pthread_t));
    
    //Creating given number of threads
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Thread_sum, (void*) thread);
        
  
    //Joining all the threads      
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    //To destroy mutex    
    pthread_mutex_destroy(&mutex);
    free(thread_handles);
    endtime=clock();

    error = fabs((4*sum - PI)/PI) * 100;

    printf("\n Value of pi 			: %11.20e\n", 4*sum);
    printf(" Maximum value of n: 		: %ld\n",n);
    printf(" Number of threads: 		: %d\n", thread_count);
    printf(" Percentage Error               : %11.10e\n", error);
    printf(" Total time taken               : %11.10e\n\n", (double)(endtime-starttime)/(double)CLOCKS_PER_SEC);

    return 0;
}

void* Thread_sum(void* rank)
{
      long my_rank = (long) rank;
      double factor;
      long long i;
      long long my_n = n/thread_count;
      long long my_first_i = my_n*my_rank;
      long long my_last_i = my_first_i + my_n;
      double my_sum = 0.0;
      
      if (my_first_i % 2 == 0)
          factor = 1.0;
      else
          factor = -1.0;
      
      for (i = my_first_i; i <= my_last_i; i++, factor = -factor)
      {
          my_sum += factor/(2*i + 1);
      }    
      pthread_mutex_lock (&mutex);
      sum += my_sum;
      pthread_mutex_unlock(&mutex);
      
      return NULL;
}
