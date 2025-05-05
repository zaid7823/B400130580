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
void seqBubbleSort(vector<int>& arr) {
	int n = arr.size();
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				swap(arr[j], arr[j + 1]);
			}
		}
	}
}
// ============================================
void parBubbleSort(vector<int>& arr) {
	int n = arr.size();

	// Odd-Even Transposition Sort
	for (int i = 0; i < n - 1; i++) {
		// Even Phase
		#pragma omp parallel for
		for (int j = 0; j < n - 1; j += 2) {
			if (arr[j] > arr[j + 1]) {
				swap(arr[j], arr[j + 1]);
			}
		}

		// Odd Phase
		#pragma omp parallel for
		for (int j = 1; j < n - 1; j += 2) {
			if (arr[j] > arr[j + 1]) {
				swap(arr[j], arr[j + 1]);
			}
		}
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

	cout << "Original Array:\n";
	// printArray(originalArr);

	// Sequential Sorting Time
	auto startTime = high_resolution_clock::now();
	seqBubbleSort(seqArr);
	auto endTime = high_resolution_clock::now();
	auto seqTime = duration_cast<milliseconds>(endTime - startTime);
	cout << "\n\nSorted Array (Sequential): ";
	// printArray(seqArr);

	startTime = high_resolution_clock::now();
	parBubbleSort(parArr);
	endTime = high_resolution_clock::now();
	auto parTime = duration_cast<milliseconds>(endTime - startTime);
	cout << "\n\nSorted Array (Parallel): ";
	// printArray(parArr);

	cout << "\n\nComputation Time (Sequential): " << seqTime.count() << " ms";
	cout << "\nComputation Time (Parallel): " << parTime.count() << " ms";

	return 0;
}