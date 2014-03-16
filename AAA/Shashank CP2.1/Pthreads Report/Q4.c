#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define n 512
#define NUM_THREADS 4

int A [n][n];
int B [n][n] ;
int C [n][n];

struct v {
   int i; /* row */
   int j; /* column */
};

void *runner(void *param); /* the thread */

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

int main(int argc, char *argv[]) {

   struct timeval tvBegin, tvEnd, tvDiff;
   int i,j,p,q, count = 0, val=1;

   gettimeofday(&tvBegin, NULL);

    for (p = 0; p< n; p++)
       {
        for (q = 0; q < n; q++)
            {
              A[p][q] = val;
		B[p][q] = val;
            }
       }

   for(i = 0; i < n; i++) {
      for(j = 0; j < n; j++) {
         //Assigning a row and column for each thread
         struct v *data = (struct v *) malloc(sizeof(struct v));
         data->i = i;
         data->j = j;
         /* Creating the thread passing it data as a parameter */
         pthread_t tid;       //Thread ID
         pthread_attr_t attr; //Set of thread attributes
         //Get the default attributes
         pthread_attr_init(&attr);
         //Creating the thread
         pthread_create(&tid,&attr,runner,data);
         pthread_join(tid, NULL);
         count++;
      }
   }
   gettimeofday(&tvEnd, NULL);
   timeval_subtract(&tvDiff, &tvEnd, &tvBegin);

/*   //Print out the resulting matrix
   for(i = 0; i < n; i++) {
      for(j = 0; j < n; j++) {
         printf("%d ", C[i][j]);
      }
      printf("\n");
   }
*/
    printf("Number of threads			:%d\n",NUM_THREADS);
    printf("Order of the matrix			:%d\n",n);
    printf("Time taken in seconds			:%ld.%06ld\n\n",tvDiff.tv_sec,tvDiff.tv_usec);
    return 0;
}

//The thread will begin control in this function
void *runner(void *param) {
   struct v *data = param; // the structure that holds our data
   int p, sum = 0; //the counter and sum

   //Row multiplied by column
   for(p = 0; p< n; p++){
      sum += A[data->i][p] * B[p][data->j];
   }
   //assign the sum to its coordinate
   C[data->i][data->j] = sum;

   //Exit the thread
   pthread_exit(0);
}