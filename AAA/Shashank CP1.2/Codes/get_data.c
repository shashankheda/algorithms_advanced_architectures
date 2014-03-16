/* Function Get_data
 * Reads in the user input a, b, and n.
 * Input parameters:
 *     1.  int my_rank:  rank of current process.
 *     2.  int p:  number of processes.
 * Output parameters:  
 *     1.  float* a_ptr:  pointer to left endpoint a.
 *     2.  float* b_ptr:  pointer to right endpoint b.
 *     3.  int* n_ptr:  pointer to number of trapezoids.
 * Algorithm:
 *     1.  Process 0 prompts user for input and
 *         reads in the values.
 *     2.  Process 0 sends input values to other
 *         processes.
 */

void Get_data(
         float*    a_ptr    /* out */, 
         float*    b_ptr    /* out */, 
         long int* n_ptr    /* out */,
         int       my_rank  /* in  */, 
         int       p        /* in  */)
{
  int source = 0;    /* All local variables used by */
  int dest;          /* MPI_Send and MPI_Recv       */
  int tag;
  MPI_Status status;

  if(my_rank == 0) {
    printf("Enter a, b, and n\n");
    scanf("%f %f %ld", a_ptr, b_ptr, n_ptr);
    for(dest=1; dest<p; dest++) {
      tag = 0;
      MPI_Send(a_ptr, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
      tag = 1;
      MPI_Send(b_ptr, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
      tag = 2;
      MPI_Send(n_ptr, 1, MPI_LONG_INT, dest, tag, MPI_COMM_WORLD);
    }
  }
  else {
    tag = 0;
    MPI_Recv(a_ptr, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
    tag = 1;
    MPI_Recv(b_ptr, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
    tag = 2;
    MPI_Recv(n_ptr, 1, MPI_LONG_INT, source, tag, MPI_COMM_WORLD, &status);
  }
} /* Get_data */

