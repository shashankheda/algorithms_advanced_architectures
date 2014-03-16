#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
using std::ios;

#include <fstream>
using std::ofstream;

int main(int argc, char **argv)
{
	srand((unsigned)time(NULL));

	int i,j,k;
	int rowA, columnA, rowB, columnB;
	int rangeMatrix = 5;

	printf("Square matrix-matrix generator with matrix-matrix multiplication result provided\n");
	printf("------------------------------------------------------------------------------------\n");
	
	printf("Row size of A >> ");
	scanf("%d", &rowA);
	columnA=rowB=columnB=rowA;

	if (columnA!=rowB)
	{
		printf("Please make sure columnA=rowB\n");
		return 0;
	}

	int *matrixA;
	int *matrixB;
	int *matrixMultiplicationResult;

	matrixA=new int[rowA*columnA];
	matrixB=new int[rowB*columnB];
	matrixMultiplicationResult=new int[rowA*columnB];

	char fileName[30];
	sprintf(fileName, "matrix_matrix_%d.txt", rowA);

	ofstream outFile(fileName, ios::out);

	// Generate matrix size
	outFile << rowA << "\n";

	// Generate matrix A
	for (i=0; i<rowA; i++)
	{
		for (j=0; j<columnA-1; j++)
		{
			matrixA[i*columnA+j]=rand()%rangeMatrix;			
			outFile << matrixA[i*columnA+j] << " ";
		}

		matrixA[i*columnA+columnA-1]=rand()%rangeMatrix;		
		outFile << matrixA[i*columnA+columnA-1] << "\n";
	}

	// Generate matrix B
	for (i=0; i<rowB; i++)
	{
		for (j=0; j<columnB-1; j++)
		{
			matrixB[i*columnB+j]=rand()%rangeMatrix;
			outFile << matrixB[i*columnB+j] << " ";
		}

		matrixB[i*columnB+columnB-1]=rand()%rangeMatrix;
		outFile << matrixB[i*columnB+columnB-1] << "\n";
	}

	outFile.close();

	// For convenience, provide calculation of the output also
	sprintf(fileName, "matrix_matrix_%d_result.txt", rowA);
	ofstream outResultFile(fileName, ios::out);

	char message[100];
	sprintf(message, "Matrix-matrix Multiplication Result of matrix_matrix_%d.txt", rowA);

	outResultFile << message << "\n";
	for (i=0;i<strlen(message);i++)
		outResultFile << "-";
	outResultFile << "\n";

	for (i=0;i<rowA;i++)
	{
		for (j=0;j<columnB;j++)
		{
			matrixMultiplicationResult[i*columnB+j]=0;

			for (k=0;k<columnA;k++)	// for (k=0;k<rowB;k++)
			{
				matrixMultiplicationResult[i*columnB+j]+=matrixA[i*columnA+k]*matrixB[k*columnB+j];
			}
			
			outResultFile << matrixMultiplicationResult[i*columnB+j];

			if (j!=columnB-1)
			{
				outResultFile << " ";
			}
		}

		outResultFile << "\n";
	}
	
	outResultFile.close();

	delete matrixA;
	delete matrixB;
	delete matrixMultiplicationResult;

	return 0;
}
