#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

void inputMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << "\nEnter [" << i << ", " << "]: ";
            cin >> matrix[i][j];
        }
    }
}
// =====================================
void inputVector(vector<int>& vec, int size) {
    for (int i = 0; i < size; i++) {
        cout << "\nEnter [" << i << "]: ";
        cin >> vec[i];
    }
}
// =====================================
void generateMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    srand(time(0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}
// =====================================
void generateVector(vector<int>& vec, int size) {
    // srand(time(0));
    for (int i = 0; i < size; i++) {
        vec[i] = rand() % 10;
    }
}
// =====================================
void printMatrix(vector<vector<int>>& matrix) {
    for (auto i : matrix) {
        for (int j : i) {
            cout << j << "\t";
        }
        cout << "\n";
    }
}
// =====================================
void printVector(vector<int>& vec) {
    for (int i : vec) {
        cout << i << "\t";
    }
    cout << "\n";
}
// =====================================
vector<int> multiplyMatrixVectorSeq(vector<vector<int>>& matrix, vector<int>& vec, int rows, int cols) {
    vector<int> result(rows, 0);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vec[j];
        }
    }
    return result;
}
// =====================================
vector<int> multiplyMatrixVectorPar(vector<vector<int>>& matrix, vector<int>& vec, int rows, int cols) {
    vector<int> result(rows, 0);

    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vec[j];
        }
    }
    return result;
}
// =====================================
int main() {
    int rows, cols;
    cout << "Enter size of matrix (rows x cols): ";
    cin >> rows >> cols;

    vector<vector<int>> matrix(rows, vector<int>(cols));
    vector<int> vec(cols);

    // inputMatrix(matrix, rows, cols);
    generateMatrix(matrix, rows ,cols);
    // inputVector(vec, cols);
    generateVector(vec, cols);

    cout << "\nMatrix:\n";
    // printMatrix(matrix);

    cout << "\nVector:\n";
    // printVector(vec);

    auto startTime = high_resolution_clock::now();
    vector<int> resultSeq = multiplyMatrixVectorSeq(matrix, vec, rows, cols);
    auto endTime = high_resolution_clock::now();
    auto seqTime = duration_cast<milliseconds>(endTime - startTime);

    cout << "\nResult Matrix (Sequential):\n";
    // printVector(resultSeq);

    startTime = high_resolution_clock::now();
    vector<int> resultPar = multiplyMatrixVectorPar(matrix, vec, rows, cols);
    endTime = high_resolution_clock::now();
    auto parTime = duration_cast<milliseconds>(endTime - startTime);


    cout << "\nResult Matrix (Parallel):\n";
    // printVector(resultPar);

    cout << "\nExecution Time (Sequential): " << seqTime.count() << " ms";
    cout << "\nExecution Time (Parallel): " << parTime.count() << " ms";

    return 0;
}