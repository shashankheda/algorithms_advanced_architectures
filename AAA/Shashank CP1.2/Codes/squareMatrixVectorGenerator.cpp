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

	int i,j;
	int matrixRowSize, matrixColumnSize;	
	int rangeMatrix = 5;
	int rangeX = 5;

	printf("Square Matrix and vector generator with matrix vector multiplication result provided\n");
	printf("------------------------------------------------------------------------------------\n");

	printf("Row size >> ");
	scanf("%d", &matrixRowSize);
	matrixColumnSize=matrixRowSize;

	int *matrix;
	int *vector;
	int *matrixVectorMultiplication;

	matrix=new int[matrixRowSize*matrixColumnSize];
	vector=new int[matrixColumnSize];
	matrixVectorMultiplication=new int[matrixRowSize];

	char fileName[50];
	sprintf(fileName, "matrix_vector_%d.txt", matrixRowSize);

	ofstream outFile(fileName, ios::out);

	// Generate matrix size
	outFile << matrixRowSize << "\n";

	// Generate matrix
	for (i=0; i<matrixRowSize; i++)
	{
		for (j=0; j<matrixColumnSize-1; j++)
		{
			matrix[i*matrixColumnSize+j]=rand()%rangeMatrix;
			outFile << matrix[i*matrixColumnSize+j] << " ";
		}

		matrix[i*matrixColumnSize+matrixColumnSize-1]=rand()%rangeMatrix;
		outFile << matrix[i*matrixColumnSize+matrixColumnSize-1] << "\n";
	}

	// Generate x
	for (i=0; i<matrixColumnSize-1;i++)
	{
		vector[i]=rand()%rangeX;
		outFile << vector[i] << " ";
	}

	vector[matrixColumnSize-1]=rand()%rangeX;
	outFile << vector[matrixColumnSize-1] << "\n";	

	outFile.close();

	// For convenience, provide calculation of the output also
	sprintf(fileName, "matrix_vector_%d_result.txt", matrixRowSize);
	ofstream outResultFile(fileName, ios::out);

	char message[100];
	sprintf(message, "Square Matrix-vector Multiplication Result of matrix_vector_%d.txt", matrixRowSize);

	outResultFile << message << "\n";
	for (i=0;i<strlen(message);i++)
		outResultFile << "-";
	outResultFile << "\n";

	for (i=0;i<matrixRowSize;i++)
	{
		matrixVectorMultiplication[i]=0;

		for (j=0;j<matrixColumnSize;j++)
		{
			matrixVectorMultiplication[i]+=matrix[i*matrixColumnSize+j]*vector[j];
		}
			
		outResultFile << matrixVectorMultiplication[i];
		outResultFile << "\n";
	}
	
	outResultFile.close();

	delete matrix;
	delete vector;
	delete matrixVectorMultiplication;

	return 0;
}
