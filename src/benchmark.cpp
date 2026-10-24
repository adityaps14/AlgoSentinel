

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
using namespace std;
using namespace chrono;

void bubbleSort(vector<int>& arr) {
    for (int i = 0; i < (int)arr.size() - 1; i++)
        for (int j = 0; j < (int)arr.size() - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

void insertionSort(vector<int>& arr) {
    for (int i = 1; i < (int)arr.size(); i++) {
        int key = arr[i], j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp;
    int i = left, j = mid + 1;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp.push_back(arr[i++]);
        else temp.push_back(arr[j++]);
    }

    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= right) temp.push_back(arr[j++]);

    for (int k = 0; k < (int)temp.size(); k++)
        arr[left + k] = temp[k];
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

template <typename Function>
double measureTime(vector<int> arr, Function sortFunction) {
    auto start = high_resolution_clock::now();
    sortFunction(arr);
    auto finish = high_resolution_clock::now();

    if (!is_sorted(arr.begin(), arr.end())) {
        cerr << "Sorting verification failed!\n";
        return -1.0;
    }

    return duration<double, milli>(finish - start).count();
}

int main() {
    srand(42);

    ofstream csv("reports/benchmark_results.csv");

    if (!csv.is_open()) {
        cerr << "Could not create CSV. Check project folder.\n";
        return 1;
    }

    csv << "Input,Bubble Sort (ms),Insertion Sort (ms),Merge Sort (ms)\n";

    int sizes[] = {1000, 5000, 10000};

    cout << "=== AlgoSentinel Benchmark ===\n";
    cout << "Execution times in milliseconds\n\n";

    cout << fixed << setprecision(3);
    csv << fixed << setprecision(3);

    cout << "Input,Bubble Sort,Insertion Sort,Merge Sort\n";

    for (int size : sizes) {
        vector<int> data(size);

        for (int i = 0; i < size; i++)
            data[i] = rand();

        double bubbleTime = -1;
        double insertionTime = -1;

        if (size <= 5000) {
            bubbleTime = measureTime(data, bubbleSort);
            insertionTime = measureTime(data, insertionSort);
        }

        double mergeTime = measureTime(data, [](vector<int>& arr) {
            mergeSort(arr, 0, (int)arr.size() - 1);
        });

        cout << size << ",";

        if (bubbleTime >= 0) cout << bubbleTime;
        else cout << "Skipped";

        cout << ",";

        if (insertionTime >= 0) cout << insertionTime;
        else cout << "Skipped";

        cout << "," << mergeTime << "\n";

        csv << size << ",";

        if (bubbleTime >= 0) csv << bubbleTime;
        else csv << "Skipped";

        csv << ",";

        if (insertionTime >= 0) csv << insertionTime;
        else csv << "Skipped";

        csv << "," << mergeTime << "\n";
    }

    csv.close();

    cout << "\nCSV report saved to reports/benchmark_results.csv\n";
    return 0;
}
