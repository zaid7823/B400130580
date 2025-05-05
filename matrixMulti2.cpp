/*
================================================
PERFORM BOTH Sequential AND Parallel & COMPARE
================================================
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// =========================================
void generateMatrix(vector<vector<int>>& matrix, int rows, int cols) {
	srand(time(0));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			matrix[i][j] = rand() % 10;
		}
	}
}
// =========================================
void inputMatrix(vector<vector<int>>& matrix, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << "Enter [" << i << ", " << j << "]: ";
			cin >> matrix[i][j];
		}
	}
}
// =========================================
void printMatrix(vector<vector<int>>& matrix) {
	for (auto i : matrix) {
		for (int j : i) {
			cout << j << "\t";
		}
		cout << "\n";
	}
}
// =========================================
vector<vector<int>> multiplyMatricesSeq(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, int r1, int c1, int c2) {
	vector<vector<int>> result(r1, vector<int>(c2, 0));
	
	for (int i = 0; i < r1; i++) {
		for (int j = 0; j < c2; j++) {
			for (int k = 0; k < c1; k++) {
				result[i][j] += matrixA[i][k] * matrixB[k][j];
			}
		}
	}
	return result;
}
// =========================================
vector<vector<int>> multiplyMatricesPar(vector<vector<int>>& matrixA, vector<vector<int>>& matrixB, int r1, int c1, int c2) {
	vector<vector<int>> result(r1, vector<int>(c2, 0));
	#pragma omp parallel for
	for (int i = 0; i < r1; i++) {
		for (int j = 0; j < c2; j++) {
			for (int k = 0; k < c1; k++) {
				result[i][j] += matrixA[i][k] * matrixB[k][j];
			}
		}
	}
	return result;
}
// =========================================
int main() {
	int rows, cols1, cols2;
	cout << "Enter size of matrix A (rows x cols): ";
	cin >> rows >> cols1;
	vector<vector<int>> matrixA(rows, vector<int>(cols1));
	generateMatrix(matrixA, rows, cols1);

	cout << "Enter no. of columns for matrix B: ";
	cin >> cols2;
	vector<vector<int>> matrixB(cols1, vector<int>(cols2));
	generateMatrix(matrixB, cols1, cols2);

	cout << "\nMatrix A:\n";
	// printMatrix(matrixA);

	cout << "\nMatrix B:\n";
	// printMatrix(matrixB);

	auto startTime = high_resolution_clock::now();
	vector<vector<int>> resultSeq = multiplyMatricesSeq(matrixA, matrixB, rows, cols1, cols2);
	auto endTime = high_resolution_clock::now();

	auto seqTime = duration_cast<milliseconds>(endTime - startTime);
	cout << "\nSequential Matrix:\n";
	// printMatrix(resultSeq);

	// ====================
	startTime = high_resolution_clock::now();
	vector<vector<int>> resultPar = multiplyMatricesPar(matrixA, matrixB, rows, cols1, cols2);
	endTime = high_resolution_clock::now();

	auto parTime = duration_cast<milliseconds>(endTime - startTime);

	cout << "\nParallel Matrix:\n";
	// printMatrix(resultPar);

	cout << "\nSequential Computation Time: " << seqTime.count() << " ms";
	cout << "\nParallel Computation Time: " << parTime.count() << " ms";

	return 0;
}