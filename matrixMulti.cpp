#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

void inputMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << "Enter element[" << i  << ", " << j << "] : ";
            cin >> matrix[i][j];
        }
    }
}

void generateMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    // cout << "\nGenerate matrix";
    srand(time(0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void printMatrix(const vector<vector<int>>& matrix) {
    for (auto i : matrix) {
        for (int j : i) {
            cout << j << "\t";
        }
        cout << "\n";
    }
}

vector<vector<int>> multiplyMatrices(const vector<vector<int>>& a, const vector<vector<int>>& b, int r, int c1, int c2) {
    vector<vector<int>> result(r, vector<int>(c2, 0));

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c2; j++) {
            for (int k = 0; k < c1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

int main() {
    int rows, cols;
    cout << "Enter dimensions of matrix A (rows x cols): ";
    cin >> rows >> cols;

    int cols2;
    cout << "Enter no. of columns of matrix B: ";
    cin >> cols2;

    vector<vector<int>> matrix_a(rows, vector<int>(cols));
    vector<vector<int>> matrix_b(cols, vector<int>(cols2));
    

    // cout << "\n\nEnter Matrix:\n";
    // inputMatrix(matrix, rows, cols);

    // cout << "\n\nEnter Vector:\n";
    // inputVector(vec, cols);

    cout << "\n\nGenerating Matrix A...\n";
    generateMatrix(matrix_a, rows, cols);

    cout << "\n\nGenerating Matrix B...\n";
    generateMatrix(matrix_b, cols, cols2);

    // cout << "Matrix A:\n";
    // printMatrix(matrix_a);

    // cout << "Matrix B:\n";
    // printMatrix(matrix_b);

    auto startTime = high_resolution_clock::now();
    vector<vector<int>> result = multiplyMatrices(matrix_a, matrix_b, rows, cols, cols2);
    auto endTime = high_resolution_clock::now();

    auto total = duration_cast<milliseconds>(endTime - startTime);

    cout << "\n\nResult Matrix:\n";
    // printMatrix(result);

    auto time = total.count();
    string unit = "ms";
    if (time > 1000) {
        time  = time / 1000.3f;
        unit = "s";
    }
          
    cout << "Time taken to calculate: " << time << " " << unit;
}