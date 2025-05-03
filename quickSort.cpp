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
// =====================================
void printArray(vector<int>& arr) {
	for (auto i : arr) {
		cout << i << " ";
	}
	cout << "\n";
}
// =====================================
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
		
		if (start < end)  {
			swap(arr[start], arr[end]);
		}
	}
	// only happens when 'start' has crossed 'end'
	swap(arr[end], arr[low]);   // because pivot is as index 'low'
	return end;
}
// =====================================
void sequentialQuickSort(vector<int>& arr, int low, int high) {
	if (low < high) {
		int pivot = partition(arr, low ,high);
		sequentialQuickSort(arr, low, pivot - 1);
		sequentialQuickSort(arr, pivot + 1, high);
	}
}
// =====================================
// void parallelQuickSort(vector<int>& arr, int low, int high) {
// 	if (low < high) {
// 		int pivot = partition(arr, low, high);

// 		#pragma omp parallel sections
// 		{
// 			#pragma omp section
// 			{
// 				parallelQuickSort(arr, low, pivot - 1);
// 			}
// 			#pragma omp section
// 			{
// 				parallelQuickSort(arr, pivot + 1, high);
// 			}
// 		}
// 	}
// }
// =====================================
void parallelQuickSort(vector<int>& arr, int low, int high, int depth = 0) {
	if (low < high) {
		int pivot = partition(arr, low, high);
		if (depth <= 4) {
			#pragma omp task shared(arr)
			parallelQuickSort(arr, low, pivot - 1, depth + 1);

			#pragma omp task shared(arr)
			parallelQuickSort(arr, pivot + 1, high, depth + 1);
		}
		else {
			// switch to sequential to avoid excessive thread overhead
			sequentialQuickSort(arr, low, pivot - 1);
			sequentialQuickSort(arr, pivot + 1, high);
		}
	}
}
// =====================================
void copyArray(const vector<int>& source, vector<int>& dest) {
	for (int i = 0; i < source.size(); i++) {
		dest[i] = source[i];
	}
}
// =====================================
int main() {
	int size;
	cout << "Enter size of array: ";
	cin >> size;

	vector<int> originalArr(size), seqArr(size), parArr(size);

	generateArray(originalArr, size);
	copyArray(originalArr, seqArr);
	copyArray(originalArr, parArr);

	cout << "Original Array:\n";
	// printArray(originalArr);

	// Sequential Quick Sort
	auto startTime = high_resolution_clock::now();
	sequentialQuickSort(seqArr, 0, size - 1);
	auto endTime = high_resolution_clock::now();
	auto seqTime = duration_cast<milliseconds>(endTime - startTime);
	// cout << "\nSorted Array (Sequential):\n";
	// printArray(seqArr);
	
	
	// Parallel Quick Sort
	startTime = high_resolution_clock::now();
	#pragma omp parallel
	{
		#pragma omp single
		parallelQuickSort(parArr, 0, size - 1); // depth is a default argument, so no need to pass here
	}
	endTime = high_resolution_clock::now();
	
	// cout << "\nSorted Array (Parallel):\n";
	// printArray(parArr);
	
	auto parTime = duration_cast<milliseconds>(endTime - startTime);
	cout << "Sequential Quick Sort Time: " << seqTime.count() << " ms\n";
	cout << "Parallel Quick Sort Time: " << parTime.count() << " ms\n";
	
	return 0;
}