#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include <pthread.h>
#include<math.h>


int thread_count;

#define n 128
int A[n][n], x[n], y[n];

void* Pth_mat_vect(void* rank); 

/* Return 1 if the difference is negative, otherwise 0.  */
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

int main(int argc, char* argv[])
{
    struct timeval tvBegin, tvEnd, tvDiff;

    // begin
    gettimeofday(&tvBegin, NULL);
//    timeval_print(&tvBegin);

    int val = 1;
    long thread;
    int i,j;
    pthread_t* thread_handles;
    
//    printf("\n\nA= ");

    for (i = 0; i < n; i++)
       {
        for (j = 0; j < n; j++)
            {
              A[i][j] = val;
            }
        x[i] = val;
       }

//    printf("\n\nx=[");
 
//    for (i=0;i<n;i++)
//	{
//		printf(" %d ",x[i]);
//	}
//    printf("]\n\n");

    thread_count = strtol(argv[1], NULL, 10);
    
    thread_handles = malloc (thread_count*sizeof(pthread_t));
    
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Pth_mat_vect, (void*) thread);
        
        
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
        
    free(thread_handles);

    //end
    gettimeofday(&tvEnd, NULL);
//    timeval_print(&tvEnd);

    // diff
    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);

//    printf("\n\ny=Ax=[");
//    for (i = 0; i < n; i++)
//    {
//      printf(" %d", y[i]);
//    }
//    printf("]\n");
    printf("Number of threads			:%d\n",thread_count);
    printf("Order of the matrix			:%d\n",n);
    printf("Time taken in seconds		:%ld.%06ld\n\n",tvDiff.tv_sec,tvDiff.tv_usec);

    return 0;
}

void* Pth_mat_vect(void* rank)
{
      long my_rank = (long) rank;
      int i, j;
      int local_m = n/thread_count;

      if(n%thread_count != 0)local_m=n/thread_count+1;

      int my_first_row = my_rank*local_m;
      int my_last_row = (my_rank + 1)*local_m - 1;
      
      for (i = my_first_row; i <= my_last_row; i++)
      {
          y[i] = 0;
          for (j = 0; j < n; j++)
              y[i] += A[i][j]*x[j];
      }    
      return NULL;
}
