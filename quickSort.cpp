#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

void generateArray(vector<int>& arr, int size) {
    srand(time(0));
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000;
    }
}
// ============================================
void printArray(vector<int>& arr) {
    for (int i : arr) {
        cout << i << " ";
    }
}
// ============================================
void copyArray(vector<int>& source, vector<int>& dest) {
    for (int i = 0; i < source.size(); i++) {
        dest[i] = source[i];
    }
}
// ============================================
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[low];
    int start = low;
    int end = high;

    while (start < end) {
        while (arr[start] <= pivot) {
            start++;
        }
        while (arr[end] > pivot) {
            end--;
        }
        if (start < end) {
            swap(arr[start], arr[end]);
        }
    }
    swap(arr[end], arr[low]);
    return end;
}
// ============================================
void seqQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        seqQuickSort(arr, low, pivot - 1);
        seqQuickSort(arr, pivot + 1, high);
    }
}
// ============================================
void parQuickSort(vector<int>& arr, int low, int high, int depth = 0) {
    if (low < high) {
        int pivot = partition(arr, low, high);

        if (depth <= 3) {
            #pragma omp task shared(arr)
            parQuickSort(arr, low, pivot - 1, depth + 1);

            #pragma omp task shared(arr)
            parQuickSort(arr, pivot + 1, high, depth + 1);

            #pragma omp taskwait
        }
        else {
            seqQuickSort(arr, low, pivot - 1);
            seqQuickSort(arr, pivot + 1, high);
        }
    }
}
// ============================================
int main() {
    int size;
    cout << "Enter size of array: ";
    cin >> size;

    vector<int> originalArr(size), seqArr(size), parArr(size);

    generateArray(originalArr, size);
    copyArray(originalArr, seqArr);
    copyArray(originalArr, parArr);

    cout << "\nOriginal Array:\n";
    // printArray(originalArr);

    auto startTime = high_resolution_clock::now();
    seqQuickSort(seqArr, 0, size - 1);
    auto endTime = high_resolution_clock::now();
    auto seqTime = duration_cast<milliseconds>(endTime - startTime);

    cout << "\nSequential Array:\n";
    // printArray(seqArr);

    startTime = high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single
        parQuickSort(parArr, 0, size - 1);
    }
    endTime = high_resolution_clock::now();
    auto parTime = duration_cast<milliseconds>(endTime - startTime);

    cout << "\nParallel Array:\n";
    // printArray(parArr);

    cout << "\n\nSequential Computation Time: " << seqTime.count() << " ms";
    cout << "\n\nParallel Computation Time: " << parTime.count() << " ms";

    return 0;
}
/*
compilation instructions for linux:

g++ -fopenmp quickSort.cpp -o quick     # Compile with OpenMP support
ulimit -s unlimited                     # Remove stack size limit for deep recursion
./quick
*/
