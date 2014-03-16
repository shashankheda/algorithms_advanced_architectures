#include <iostream>
#include <iomanip>
#include <mpi.h>
#include <cmath>


using namespace std;

int main(int argc, char *argv[])
{
 int i,j,k,size,index,numrows;
 int index1,index2;
 int mynode, totalnodes;
 double alpha,gamma,start,time1,time2,time3,end,dur;
scanf("%d",&numrows);

 MPI_Status status;
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD, &totalnodes);
 MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
 start = MPI_Wtime();
 size = (int) pow(2,log2(totalnodes+1)+1)-1;
 double ** A = new double*[numrows];
 for(i=0;i<numrows;i++)
 {
  A[i] = new double[size+1];
  for(j=0;j<size+1;j++)
   A[i][j] = 0.0;
 }
 if(mynode==0)
 {
  A[0][0] = -2.0; A[0][1] = 1.0;
  A[1][0] = 1.0; A[1][1] = -2.0; A[1][2] = 1.0;
  A[2][1] = 1.0; A[2][2] = -2.0; A[2][3] = 1.0;
 }
 else if(mynode==(totalnodes-1))
 {
  index = 2*mynode;
  A[0][index-1] = 1.0; A[0][index] = -2.0;
  A[0][index+1] = 1.0;
  index = 2*mynode+1;
  A[1][index-1] = 1.0; A[1][index] = -2.0;
  A[1][index+1] = 1.0;
  A[2][size-2] = 1.0; A[2][size-1] = -2.0;
 }
 else
 {
  for(i=0;i<3;i++)
  {
   index = i + 2*mynode;
   A[i][index-1] = 1.0;
   A[i][index]= -2.0;
   A[i][index+1] = 1.0;
  }
 }
 time1 = MPI_Wtime();



 for(i=0;i<3;i++)
   A[i][size] = 2*mynode+i;
 int numactivep = totalnodes;
 int * activep = new int[totalnodes];
 for(j=0;j<numactivep;j++)
   activep[j] = j;
 for(j=0;j<size+1;j++)
 {
   A[3][j] = A[0][j];
   A[4][j] = A[2][j];
 }

 time2 = MPI_Wtime();

 /* Part 2 */
 for(i=0;i<log2(size+1)-1;i++)
 {
  for(j=0;j<numactivep;j++)
  {
   if(mynode==activep[j])
   {
    index1 = 2*mynode + 1 - pow(2,i);
    index2 = 2*mynode + 1 + pow(2,i);
    alpha = A[1][index1]/A[3][index1];
    gamma = A[1][index2]/A[4][index2];
    for(k=0;k<size+1;k++)
      A[1][k] -= (alpha*A[3][k] + gamma*A[4][k]);
    if(numactivep>1)
    {
     if(j==0)
     {
       MPI_Send(A[1],size+1,MPI_DOUBLE,activep[1],0,MPI_COMM_WORLD);
      }
     else if(j==numactivep-1)
     {
       MPI_Send(A[1],size+1,MPI_DOUBLE,activep[numactivep-2],1,MPI_COMM_WORLD);
     }
     else if(j%2==0)
     {
       MPI_Send(A[1],size+1,MPI_DOUBLE,activep[j-1],1,MPI_COMM_WORLD);
       MPI_Send(A[1],size+1,MPI_DOUBLE,activep[j+1],0,MPI_COMM_WORLD);
     }
     else
     {
       MPI_Recv(A[3],size+1,MPI_DOUBLE,activep[j-1],0,MPI_COMM_WORLD,&status);
       MPI_Recv(A[4],size+1,MPI_DOUBLE,activep[j+1],1,MPI_COMM_WORLD,&status);
     }
    }
   }
  }
  numactivep = 0;
  for(j=activep[1];j<totalnodes;j=j+pow(2,i+1))
  {
    activep[numactivep++]=j;
  }
 }
 time3 = MPI_Wtime();


 /* Part 3 */
 
 double * x = new double[totalnodes];
 
 for(j=0;j<totalnodes;j++)
 x[j] = 0.0;
 if(mynode==activep[0])
 {
  x[mynode] = A[1][size]/A[1][(size-1)/2];
 }
 double tmp;
 for(i=log2(size+1)-3;i>=0;i--)
 {
  tmp = x[mynode];
  MPI_Allgather(&tmp,1,MPI_DOUBLE,x,1,MPI_DOUBLE,
  MPI_COMM_WORLD);
  numactivep = 0;
  for(j=activep[0]-pow(2,i);j<totalnodes;j=j+pow(2,i+1))
  {
   activep[numactivep++]=j;
  }
  for(j=0;j<numactivep;j++)
  {
   if(mynode == activep[j])
   {
    x[mynode] = A[1][size];
    for(k=0;k<totalnodes;k++)
    {
     if(k!=mynode)
       x[mynode] -= A[1][2*k+1]*x[k];
    }
    x[mynode] = x[mynode]/A[1][2*mynode+1];
   }
  }
 }
 tmp = x[mynode];
 MPI_Allgather(&tmp,1,MPI_DOUBLE,x,1,MPI_DOUBLE,MPI_COMM_WORLD);
 
 /* Part 4 */
 for(k=0;k<totalnodes;k++)
 {
  A[0][size] -= A[0][2*k+1]*x[k];
  A[2][size] -= A[2][2*k+1]*x[k];
 }
  A[0][size] = A[0][size]/A[0][2*mynode];
  A[1][size] = x[mynode];
  A[2][size] = A[2][size]/A[2][2*mynode+2];
  
 end = MPI_Wtime();
 MPI_Finalize();
  if (mynode==0)
  {
   cout<<"\nNumrows: "<<numrows<<"\n"<<totalnodes;
   //for(i=0;i<numrows;i++)
 //cout<<"\n"<<x[i];  
   dur=end-start;
   cout<<"\n"<<dur<<"\n";
   cout<<"\n"<<time1-start;
   cout<<"\n"<<time3-time2;
   cout<<"\n"<<end-time3<<"\n";
  }
}
