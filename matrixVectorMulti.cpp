#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Function to input a matrix
void inputMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << "Enter element[" << i  << ", " << j << "] : ";
            cin >> matrix[i][j];
        }
    }
}

void generateMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    cout << "\nGenerate matrix";
    srand(time(0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}
void generateVector(vector<int>& vec, int size) {
    cout << "\nGenerate vector";
    srand(time(0));
    for(int i = 0; i < size; i++) {
        vec[i] = rand() % 10;
    }
}
// ==============================
void inputVector(vector<int>& vec, int size) {
    for (int i = 0; i < size; i++) {
        cout << "Enter element [" << i << "]: ";
        cin >> vec[i];
    }
}
// ==============================
void printMatrix(const vector<vector<int>>& matrix) {
    for (auto i : matrix) {
        for (int j : i) {
            cout << j << "\t";
        }
        cout << "\n";
    }
}
// ==============================
void printVector(vector<int>& vec) {
    for (auto i : vec) {
        cout << i << "\t";
    }
    cout << "\n";
}
// ==============================
vector<int> multiplyMatrixVector(const vector<vector<int>>& matrix, const vector<int>& vec, int rows, int cols) {

    vector<int> result(rows, 0);    // stores the product

    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vec[j];
        }
    }
    return result;
}
// ==============================
int main() {
    int rows, cols;
    cout << "Enter dimensions of matrix (rows x cols): ";
    cin >> rows >> cols;

    vector<vector<int>> matrix(rows, vector<int>(cols));
    vector<int> vec(cols);

    // cout << "\n\nEnter Matrix:\n";
    // inputMatrix(matrix, rows, cols);

    // cout << "\n\nEnter Vector:\n";
    // inputVector(vec, cols);

    cout << "\n\nGenerating Matrix...\n";
    generateMatrix(matrix, rows, cols);

    cout << "\n\nGenerating Vector...\n";
    generateVector(vec, cols);

    cout << "Matrix:\n";
    // printMatrix(matrix);

    cout << "Vector:\n";
    // printVector(vec);

    auto startTime = high_resolution_clock::now();
    vector<int>result = multiplyMatrixVector(matrix, vec, rows, cols);
    auto endTime = high_resolution_clock::now();

    auto total = duration_cast<milliseconds>(endTime - startTime);

    cout << "\n\nResult Vector:\n";
    // printVector(result);

    cout << "Time taken to calculate: " << total.count() << " ms\n";

    return 0;
}