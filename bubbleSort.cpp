// Implement Sequential and Parallel Bubble Sort using OpenMP and compare performance

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Function to generate a random array
void generateArray(vector<int>& arr, int size) {
    srand(time(0)); // seed the random number generator
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000; // random numbers between 0 and 99999
    }
}

// Function to display array
void printArray(vector<int>& arr) {
    for (auto i: arr) {
        cout << i << " ";
    }
    cout << "\n";
}

// Sequential Bubble Sort
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        // cout << "sequential bubble sort\n";
        for (int j = 0; j < n - 1; j++) {
            if (arr[j] > arr[j+1]) {
                // cout << "S swap\n";
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Parallel Bubble Sort using OpenMP (Odd-Even Transposition Sort)
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i <n; i++) {
        // cout << "Parallel bubble sort\n";
        // Even Phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j+1]) {
                // cout << "p swap\n";
                swap(arr[j], arr[j+1]);
            }
        }

        // Odd Phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            // cout << "Parallel bubble sort\n";
            if (arr[j] > arr[j+1]) {
                // cout << "p swap\n";
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

void copyArray(vector<int>& source, vector<int>& dest) {
    for (int i = 0; i < source.size(); i++) {
        dest[i] = source[i];
    }
}

int main(int argc, char const *argv[]) {
    int size;
    cout << "Enter size of array: ";
    cin >> size;

    vector<int> originalArr(size), seqArr(size), parArr(size);

    generateArray(originalArr, size);
    copyArray(originalArr, seqArr);
    copyArray(originalArr, parArr);

    cout << "\nOriginal Array: ";
    // printArray(originalArr);

    // Sequential Bubble Sort
    cout << "Calculating sequential time...\n";
    auto startTime = high_resolution_clock::now();
    sequentialBubbleSort(seqArr);
    auto endTime = high_resolution_clock::now();
    auto seqTime = duration_cast<seconds>(endTime - startTime);

    // Parallel Bubble Sort
    cout << "Calculating parallel time...\n";
    startTime = high_resolution_clock::now();
    parallelBubbleSort(parArr);
    endTime = high_resolution_clock::now();
    auto parTime = duration_cast<seconds>(endTime - startTime);

    cout << "Sorted Array (Sequential): ";
    // printArray(seqArr);

    cout << "Sorted Array (Parallel): ";
    // printArray(parArr);

    cout << "Performance\n";

    cout << "Sequential Time: " << seqTime.count() << " seconds\n";
    cout << "Parallel Time: " << parTime.count() << " seconds\n";

    return 0;
}
/*
    auto startTime = high_resolution_clock::now();
    sequentialQuickSort(seqArr, 0, size - 1);
    auto endTime = high_resolution_clock::now();

    auto seqTime = duration_cast<seconds>(endTime - startTime);
*/
