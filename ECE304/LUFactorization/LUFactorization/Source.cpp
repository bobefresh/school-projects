#include <iostream>
#include "stdlib.h"

using namespace std;

void luFact(int n, double **mat, double **L, double **U)
{
	U = mat;
	//Copy input mat to U mat
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			U[i][j] = mat[i][j];
		}
	}
	for (int pivot = 0; pivot < n; pivot++)
	{
		//Divide pivot row by diagonal value
		int diagTemp = mat[pivot][pivot];
		for (int j = pivot; j < n; j++)
		{
			//U: Divide current row by pivot
			U[pivot][j] = U[pivot][j] / diagTemp;

			//L: Divide current column by pivot
			L[j][pivot] = -U[j][pivot] / diagTemp;
		}
		//Multiply pivot row by other row's negative term and add to respective row
		//Row iteration
		for (int row = pivot + 1; row < n; row++)
		{
			//Temp to store leading term for current row
			int leadTerm = U[row][pivot];

			//Column iteration
			for (int j = pivot; j < n; j++)
			{
				U[row][j] -= U[pivot][j] * leadTerm;
			}
		}
	}
}

void printMat(int n, double **matrix)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << matrix[i][j] << "\t";
		}
		cout << endl;
	}
}

int main()
{
	const int DIM = 3;
	int i, j;
	double **input, **L, **U;
	input = new double* [DIM];
	L = new double* [DIM];
	U = new double* [DIM];

	//Initialize input, L, and U matricies
	for (i = 0; i < DIM; i++)
	{
		for (j = 0; j < DIM; j++)
		{
			cout << "Row: " << i << "Col: " << j << " = ";
			cin >> input[i][j];
			L[i][j] = U[i][j] = 0;
		}
	}

	//Perform LU Factorization
	luFact(DIM, input, L, U);

	//Print Input, L, and U matricies
	cout << "Input matrix: " << endl;
	printMat(DIM, input);
	cout << "\nL matrix: " << endl;
	printMat(DIM, L);
	cout << "\nU matrix: " << endl;
	printMat(DIM, U);

	//Deallocate
	for (i = 0; i < DIM; i++)
	{
		delete[] input[i];
		delete[] L[i];
		delete[] U[i];
	}
	delete[] input;
	delete[] L;
	delete[] U;
	
	return 0;
}