# include <stdio.h>
# include "mpi.h"

long int Flip (long int, int);

void main(int argc, char **argv)
{
  int p;
  int my_rank;
  long int local_head, total_head,f11,f12,f13,f21,f22,f31,n,N;
  float percent_head, percent_tail;
  double starttime, endtime;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if(my_rank == 0) {
   printf("\nThis program flips a coin and report on the outcome.\n");

   printf("Enter number of flips, n (n flips are perfomed by each p):\n");
   scanf("%ld", &n);
   }

  MPI_Bcast(&n, 1, MPI_LONG, 0, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);

  starttime=MPI_Wtime();

  local_head = Flip(n, my_rank);


  if(my_rank==1)
  {
	MPI_Send(&local_head, 1, MPI_LONG, 2, 12, MPI_COMM_WORLD); 			// Data from p=1 sent to p=2
	//printf("\n\n%d	local_head=%ld\n",my_rank,local_head);
  }

  else if(my_rank==2)
  {
	MPI_Recv(&f11, 1, MPI_LONG, 1, 12, MPI_COMM_WORLD, MPI_STATUS_IGNORE);		// Data from p=1 received by p=2
	MPI_Send(&f11, 1, MPI_LONG, 3, 123, MPI_COMM_WORLD);				// Data from p=1 sent to p=3
	//printf("\n\n%d	f11=%ld\n",my_rank,f11);

	MPI_Send(&local_head, 1, MPI_LONG, 3, 23, MPI_COMM_WORLD);			// Data from p=2 sent to p=3
	//printf("\n\n%d	local_head=%ld\n",my_rank,local_head);
  }

  else if(my_rank==3)
  {
	MPI_Recv(&f12, 1, MPI_LONG, 2, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	// Data from p=1 received by p=3
	MPI_Send(&f12, 1, MPI_LONG, 0, 1230, MPI_COMM_WORLD);				// Data from p=1 sent to p=0
	//printf("\n\n%d	f12=%ld\n",my_rank,f12);

	MPI_Recv(&f21, 1, MPI_LONG, 2, 23, MPI_COMM_WORLD, MPI_STATUS_IGNORE);		// Data from p=2 received by p=3
	MPI_Send(&f21, 1, MPI_LONG, 0, 230, MPI_COMM_WORLD);				// Data from p=2 sent to p=0
	//printf("\n\n%d	f21=%ld\n",my_rank,f21);

	MPI_Send(&local_head, 1, MPI_LONG, 0, 30, MPI_COMM_WORLD);			// Data from p=3 sent to p=0
	//printf("\n\n%d	local_head=%ld\n",my_rank,local_head);
  }

  else if(my_rank==0)
  {
	MPI_Recv(&f13, 1, MPI_LONG, 3, 1230, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	// Data from p=1
	//printf("\n\n%d	f13=%ld\n",my_rank,f13);

	MPI_Recv(&f22, 1, MPI_LONG, 3, 230, MPI_COMM_WORLD, MPI_STATUS_IGNORE);		// Data from p=2
	//printf("\n\n%d	f22=%ld\n",my_rank,f22);

	MPI_Recv(&f31, 1, MPI_LONG, 3, 30, MPI_COMM_WORLD, MPI_STATUS_IGNORE);		// Data from p=3
	//printf("\n\n%d	f31=%ld\n",my_rank,f31);

	total_head=local_head+f13+f22+f31;
	N=4*n;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  

  endtime=MPI_Wtime();

  if (my_rank == 0){
     percent_head = ((float)total_head / (float)N) * 100;
     percent_tail = ((float)(N - total_head) / (float)N) * 100;
     printf("Number of flips performed by each processor = %ld\n", n);
     printf("Percentage of flips turning in head 	    = %f\n", percent_head);
     printf("Percentage of flips turning in tail 	    = %f\n", percent_tail);
     printf("Total time elapsed			    = %e\n", endtime-starttime);
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

