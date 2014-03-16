 /* mat_vect_mult.c -- multiply a square matrix with a vector using OpenMP
 *
 * Input: 
 *     n: order of the matrix and no. of rows of vector
 *     A: Factor matrix
 *	B: Column vector
 * Output:
 *     C: product Column Vector
 */

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int     m,n,i;
    float *  A;
    float *  B;
    float *  C;

    double stime, etime;
    double Ttime,a[35];

    void Read_matrix(char* prompt, float * A, int n);
    void Read_vector(char* prompt, float * A, int n);
    void Serial_matrix_vector_mult(float * A, float * B, float * C, int n);
    void Print_matrix(char* title, float * C, int n);
    void Print_vector(char* title, float * C, int n);

for(i=0;i<35;i++){
    //printf("What's the number of threads:");
    scanf("%d", &m);
    omp_set_num_threads(m);
    
    //printf("What's the order of the matrix and the no. of rows of vector? ");
    scanf("%d", &n);

    stime = omp_get_wtime();

    A = (float *) malloc( n*n * sizeof(float));
    B = (float *) malloc( n * sizeof(float));
    C = (float *) malloc( n * sizeof(float));

    Read_matrix("Enter A", A, n);
    //Print_matrix("A:", A, n);

    Read_vector("Enter B", B, n);
    //Print_vector("B:", B, n);

    Serial_matrix_vector_mult(A, B, C, n);

//    Print_vector("Multiplied vector is:", C, n);


    free(A);
    free(B);
    free(C);

    etime = omp_get_wtime();
    Ttime = etime - stime;
    a[i]=Ttime;
    printf("For n=%4d and %2d threads, Total time : %.10lf\n", n, m, Ttime);
}


for(i=0;i<35;i++)
printf("\n%e",a[i]);
    return 0;
}  /* main */


/*****************************************************************/
void Read_matrix(
         char*     prompt  /* in  */,
         float *   A       /* out */,
         int       n       /* in  */) {
    int i, j, m=0;

    for (i = 0; i < n; i++)
    {
	for (j = 0; j < n; j++)
	{
	  A[i*n + j]=1;
	}
    }
}  /* Read_matrix */


/*****************************************************************/
void Read_vector(
         char*     prompt  /* in  */,
         float *   A       /* out */,
         int       n       /* in  */) {
    int i, m=0;
#pragma omp parallel for
    for (i = 0; i < n; i++)
    {
          A[i]=1;
    }
}  /* Read_vector */
/*****************************************************************/


/* MATRIX_T is a two-dimensional array of floats */
void Serial_matrix_vector_mult(
        float *   A   /* in  */,
        float *   B   /* in  */,
        float *   C   /* out */,
        int        n   /* in  */) {

    int i, j, k;

    void Print_matrix(char* title, float * C, int n);

    #pragma omp parallel for 					//Nested parallel regions
    for (i = 0; i < n; i++)
    {
	C[i]=0.0;
            #pragma omp parallel for				//Nested parallel regions
	    for (k = 0; k < n; k++)
	    {
		(C[i]) = (C[i]) + (A[i*n +k]) * (B[k]);
	    }
	    //printf("i = %d, j = %d, c_ij = %f\n", i, j, C[i][j]);
    }
}  /* Serial_matrix_mult */


/*****************************************************************/
void Print_matrix(
         char*     title   /* in  */,
         float *   C       /* out */,
         int       n       /* in  */) {
    int i, j;

    printf("%s\n", title);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%4.1f ", C[i*n +j]);
        printf("\n");
    }
}  /* Print_matrix */


/*****************************************************************/
void Print_vector(
         char*     title   /* in  */,
         float *   C       /* out */,
         int       n       /* in  */) {
    int i;

    printf("%s\n", title);
    for (i = 0; i < n; i++) {
            printf("%4.1f \n", C[i]);
    }
}  /* Print_vector */
