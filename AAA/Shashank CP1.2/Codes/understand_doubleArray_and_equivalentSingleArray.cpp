#include <cstdio>
#include <cstdlib>

#include <iostream>
using std::cout;
using std::endl;

void understandDoubleArray(int N);
void understandEquivalentSingleArray(int N);

int main(int argc, char **argv)
{
	int N = 4;

	understandDoubleArray(N);
	cout << endl;
	understandEquivalentSingleArray(N);

	return 0;
}

void understandDoubleArray(int N)
{
	int **A;
	int i, j;

	A = new int*[N];

	for (i=0; i<N; i++)
		A[i] = new int[N];

	for (i=0; i<N; i++)
		for (j=0; j<N; j++)
			A[i][j] = i*N + j;

	cout << A << endl;;

	cout << &(A[0]) << endl;
	cout << A[0] << endl;
	cout << &(A[0][0]) << endl;
	cout << A[0][0] << endl;	
	cout << &(A[0][N-1]) << endl;
	cout << A[0][N-1] << endl;

	cout << &(A[1]) << endl;
	cout << A[1] << endl;
	cout << &(A[1][0]) << endl;	
	cout << A[1][0] << endl;
	
}

void understandEquivalentSingleArray(int N)
{
	int *A;
	int i, j;

	A = new int[N*N];

	for (i=0; i<N; i++)
		for (j=0; j<N; j++)
			A[i*N+j] = i*N + j;

	cout << A << endl;

	cout << &(A[0]) << endl;
	cout << A[0] << endl;

	cout << &(A[1]) << endl;
	cout << A[1] << endl;	

	cout << &(A[N]) << endl;
	cout << A[N] << endl;
}