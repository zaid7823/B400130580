#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
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
void copyArray(vector<int>& source, vector<int>& dest) {
	for (int i = 0; i < source.size(); i++) {
		dest[i] = source[i];
	}
}
// ============================================
void printArray(vector<int>& arr) {
	for (int i : arr) {
		cout << i << " ";
	}
}
// ============================================
void merge(vector<int>& arr, int left, int mid, int right) {
    int i = left, j = mid + 1, k = 0;
    vector<int> temp(right - left + 1);

    while (i <= mid && j <= right) {
        if (arr[i] < arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }
    while(i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (int k = 0; k < temp.size(); k++) {
        arr[left + k] = temp[k];    // left is an offset
    }
}
// ============================================
void seqMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        seqMergeSort(arr, left, mid);
        seqMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}
// ============================================
void parMergeSort(vector<int>& arr, int left, int right, int depth = 0) {
    if (left < right) {
        int mid = (left + right) / 2;
        if (depth <= 3) {
            #pragma omp task shared(arr)
            parMergeSort(arr, left, mid, depth + 1);

            #pragma omp task shared(arr)
            parMergeSort(arr, mid + 1, right, depth + 1);

            #pragma omp taskwait
        }
        else {
            seqMergeSort(arr, left, mid);
            seqMergeSort(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }
}
// ============================================
int main() {
    int n;
    cout << "Enter size of array: ";
    cin >> n;

    vector<int> originalArr(n), seqArr(n), parArr(n);

    generateArray(originalArr, n);
    copyArray(originalArr, seqArr);
    copyArray(originalArr, parArr);

    cout << "\nOriginal Array:\n";
    // printArray(originalArr);

    auto startTime = high_resolution_clock::now();
    seqMergeSort(seqArr, 0, n - 1);
    auto endTime = high_resolution_clock::now();
    auto seqTime = duration_cast<milliseconds>(endTime - startTime);

    startTime = high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single
        parMergeSort(parArr, 0, n - 1);
    }
    endTime = high_resolution_clock::now();
    auto parTime = duration_cast<milliseconds>(endTime - startTime);

    cout << "\n\nSequential Array: ";
    // printArray(seqArr);

    cout << "\n\nParallel Array: ";
    // printArray(parArr);

    cout << "\n\nSorting Time (Sequential): " << seqTime.count() << " ms";
    cout << "\n\nSorting Time (Parallel): " << parTime.count() << " ms";
    
    return 0;
} 