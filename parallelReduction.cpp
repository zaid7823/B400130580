#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

void generateArray(vector<int>& arr, int size) {
    srand(time(0));
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 10000;
    }
}
// =========================================
int findMinSeq(vector<int>& arr) {
    int minVal = numeric_limits<int>::max();

    #pragma omp parallel for reduction(min:minVal)
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}
int findMinPar(vector<int>& arr) {
    int minVal = numeric_limits<int>::max();

    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}
// =========================================
int findMaxSeq(vector<int>& arr) {
    int maxVal = numeric_limits<int>::min();

    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }
    return maxVal;
}
int findMaxPar(vector<int>& arr) {
    int maxVal = numeric_limits<int>::min();

    #pragma omp parallel for reduction(max:maxVal)
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }
    return maxVal;
}
// =========================================
long long calculateSumSeq(vector<int>& arr) {
    long long total = 0;
    
    for (int i = 0; i < arr.size(); i++) {
        total += arr[i];
    }
    return total;
}
long long calculateSumPar(vector<int>& arr) {
    long long total = 0;
    #pragma omp parallel for reduction(+:total)
    for (int i = 0; i < arr.size(); i++) {
        total += arr[i];
    }
    return total;
}
// =========================================
double calculateAvg(vector<int>& arr, long long total) {
    double average = static_cast<double>(total) / arr.size();
    return average;
}
// =========================================
int main() {
    int size;
    cout << "Enter size of array: ";
    cin >> size;
    vector<int> arr(size);

    generateArray(arr, size);
    
    auto startTime = high_resolution_clock::now();

    int minSeq = findMinSeq(arr);
    int maxSeq = findMaxSeq(arr);
    long long totalSeq = calculateSumSeq(arr);
    double avg = calculateAvg(arr, totalSeq);

    auto endTime = high_resolution_clock::now();
    auto seqTime = duration_cast<milliseconds>(endTime - startTime);

    cout << "\nResults without Reduction:";
    cout << "\nMinimum: " << minSeq;
    cout << "\nMaximum: " << maxSeq;
    cout << "\nSum: " << totalSeq;
    cout << "\nAverage: " << avg;
    // =====================================
    startTime = high_resolution_clock::now();

    int minPar = findMinPar(arr);
    int maxPar = findMaxPar(arr);
    long long totalPar = calculateSumPar(arr);
    double avgSeq = calculateAvg(arr, totalPar);

    endTime = high_resolution_clock::now();

    auto parTime = duration_cast<milliseconds>(endTime - startTime);

    cout << "\n\nResults using Parallel Reduction:";
    cout << "\nMinimum: " << minPar;
    cout << "\nMaximum: " << maxPar;
    cout << "\nSum: " << totalPar;
    cout << "\nAverage: " << avg;

    cout << "\n\nSequential Operation Time: " << seqTime.count() << " ms";
    cout << "\nParallel Operation Time: " << parTime.count() << " ms";
}