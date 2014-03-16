/* serial_mat_mult.c -- multiply two square matrices on a single processor
 *
 * Input: 
 *     n: order of the matrices
 *     A,B: factor matrices
 *
 * Output:
 *     C: product matrix
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include <pthread.h>
#include<math.h>
#define MAX_ORDER 256

typedef float MATRIX_T[MAX_ORDER][MAX_ORDER];

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

main() {
    struct timeval tvBegin, tvEnd, tvDiff;

    int       n,p,q,val=1;
    MATRIX_T  A;
    MATRIX_T  B;
    MATRIX_T  C;

    void Read_matrix(char* prompt, MATRIX_T A, int n);
    void Serial_matrix_mult(MATRIX_T A, MATRIX_T B, MATRIX_T C, int n);
    void Print_matrix(char* title, MATRIX_T C, int n);

    printf("What's the order of the matrices?\n");
    scanf("%d", &n);

    for (p = 0; p< n; p++)
       {
        for (q = 0; q < n; q++)
            {
              A[p][q] = val;
		B[p][q] = val;
            }
       }

    gettimeofday(&tvBegin, NULL);
    Serial_matrix_mult(A, B, C, n);
    gettimeofday(&tvEnd, NULL);
    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);

    //Print_matrix("Their product is", C, n);
    printf("Time taken in seconds		:%ld.%06ld\n\n",tvDiff.tv_sec,tvDiff.tv_usec);

}  /* main */


/*****************************************************************/
void Read_matrix(
         char*     prompt  /* in  */,
         MATRIX_T  A       /* out */,
         int       n       /* in  */) {
    int i, j;

    printf("%s\n", prompt);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%f", &A[i][j]);
}  /* Read_matrix */


/*****************************************************************/
/* MATRIX_T is a two-dimensional array of floats */
void Serial_matrix_mult(
        MATRIX_T   A   /* in  */,
        MATRIX_T   B   /* in  */,
        MATRIX_T   C   /* out */,
        int        n   /* in  */) {

    int i, j, k;

    void Print_matrix(char* title, MATRIX_T C, int n);

    //Print_matrix("In Serial_matrix_mult A = ", A, n);
    //Print_matrix("In Serial_matrix_mult B = ", B, n);

    for (i = 0; i < n; i++)    
        for (j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (k = 0; k < n; k++)
                C[i][j] = C[i][j] + A[i][k]*B[k][j];
            //printf("i = %d, j = %d, c_ij = %f\n", i, j, C[i][j]);
        }
}  /* Serial_matrix_mult */


/*****************************************************************/
void Print_matrix(
         char*     title  /* in  */,
         MATRIX_T  C       /* out */,
         int       n       /* in  */) {
    int i, j;

    printf("%s\n", title);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%4.1f ", C[i][j]);
        printf("\n");
    }
}  /* Read_matrix */