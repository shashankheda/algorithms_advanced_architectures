# include <stdio.h>
# include "mpi.h"

long int Flip (long int, int);

void main(int argc, char **argv)
{
  int p;
  int my_rank;
  int flag=1, n=1, local_n;
  long int head, total_head;
  float percent_head, percent_tail;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if(my_rank == 0) {
    printf("\nThis program flips a coin and report on the outcome.\n");

    while(flag) {
      printf("Enter number of flips, n (n must be divisible by p):\n");
      scanf("%ld", &n);
      if(n%p == 0) flag = 0;
    }
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  local_n = n/p;

  head = Flip(local_n, my_rank);

  MPI_Reduce (&head, &total_head, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

  if (my_rank == 0){
     percent_head = ((float)total_head / (float)n) * 100;
     percent_tail = ((float)(n - total_head) / (float)n) * 100;
     printf("Percentage of flips turning in head = %f\n", percent_head);
     printf("Percentage of flips turning in tail = %f\n", percent_tail);
  }

  MPI_Finalize();
}

long int Flip(long int local_n, int my_rank)
{
  int x, i;
  long int head;

  srand(my_rank+1);
  head = 0;
  for(i=0; i<local_n; i++){
    x = rand();
    if (x%2 == 0.0) head += 1;
  }

  return (head);
}

