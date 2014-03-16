 /* serial_mat_mult.c -- multiply two square matrices on a single processor
 *
 * Input: 
 *     n: order of the matrices
 *     A,B: factor matrices
 *
 * Output:
 *     C: product matrix
 */

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int     m,n;
    float *  A;
    float *  B;
    float *  C;

    double stime, etime;
    double Ttime;

    void Read_matrix(char* prompt, float * A, int n);
    void Serial_matrix_mult(float * A, float * B, float * C, int n);
    void Print_matrix(char* title, float * C, int n);

    //printf("What's the number of threads:");
    scanf("%d", &m);
    omp_set_num_threads(m);
    
    //printf("What's the order of the matrices?\n");
    scanf("%d", &n);

    stime = omp_get_wtime();

    A = (float *) malloc( n*n * sizeof(float));
    B = (float *) malloc( n*n * sizeof(float));
    C = (float *) malloc( n*n * sizeof(float));

    Read_matrix("Enter A", A, n);
    //(n<=80)? Print_matrix("A = ", A, n): printf("Matrix A is of order >80\n");
    Read_matrix("Enter B", B, n);
    //(n<=80) ? Print_matrix("B = ", B, n): printf("Matrix B is of order >80\n");
    Serial_matrix_mult(A, B, C, n);
    //(n<=80) ? Print_matrix("Their product is", C, n): printf("Matrix C is of order >80\n");
   
    free(A);
    free(B);
    free(C);

    etime = omp_get_wtime();
    Ttime = etime - stime;
    
    printf("\nFor n=%d and %d threads, Total time : %.10lf\n", n, m, Ttime);

    return 0;
}  /* main */


/*****************************************************************/
void Read_matrix(
         char*     prompt  /* in  */,
         float *   A       /* out */,
         int       n       /* in  */) {
    int i, j, m=0;

    //#pragma omp parallel for    
    for (i = 0; i < n; i++)
    {
        //#pragma omp parallel for
	for (j = 0; j < n; j++)
	{
	  A[i*n + j]=m++;
	}
    }
}  /* Read_matrix */


/*****************************************************************/
/* MATRIX_T is a two-dimensional array of floats */
void Serial_matrix_mult(
        float *   A   /* in  */,
        float *   B   /* in  */,
        float *   C   /* out */,
        int        n   /* in  */) {

    int i, j, k;

    void Print_matrix(char* title, float * C, int n);

    #pragma omp parallel for
    for (i = 0; i < n; i++)
    {
	
        #pragma omp parallel for 
	for (j = 0; j < n; j++) {
	    (C[i*n +j]) = 0.0;

            #pragma omp parallel for 
	    for (k = 0; k < n; k++)
	    {
		(C[i*n +j]) = (C[i*n +j]) + (A[i*n +k]) * (B[k*n +j]);
	    } 
	    //printf("i = %d, j = %d, c_ij = %f\n", i, j, C[i][j]);
        }
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
}  /* Read_matrix */
