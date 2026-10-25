#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

using namespace std;
using namespace chrono;

struct BenchmarkResult {
    string algorithm;
    double time_ms;
    long long comparisons;
    long long swaps;
};

struct Counters {
    long long comparisons = 0;
    long long swaps = 0;
};

// Bubble Sort
void bubbleSort(vector<int>& arr, Counters& count) {
    int n = static_cast<int>(arr.size());

    for (int i = 0; i < n - 1; ++i) {
        bool changed = false;

        for (int j = 0; j < n - i - 1; ++j) {
            count.comparisons++;

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                count.swaps++;
                changed = true;
            }
        }

        if (!changed) {
            break;
        }
    }
}

// Insertion Sort
void insertionSort(vector<int>& arr, Counters& count) {
    int n = static_cast<int>(arr.size());

    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            count.comparisons++;

            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                count.swaps++;
                --j;
            } else {
                break;
            }
        }

        arr[j + 1] = key;
    }
}

// Merge Sort
void mergeParts(
    vector<int>& arr,
    vector<int>& temp,
    int left,
    int middle,
    int right,
    Counters& count
) {
    int i = left;
    int j = middle + 1;
    int k = left;

    while (i <= middle && j <= right) {
        count.comparisons++;

        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }

        count.swaps++;
    }

    while (i <= middle) {
        temp[k++] = arr[i++];
        count.swaps++;
    }

    while (j <= right) {
        temp[k++] = arr[j++];
        count.swaps++;
    }

    for (int index = left; index <= right; ++index) {
        arr[index] = temp[index];
    }
}

void mergeSortRecursive(
    vector<int>& arr,
    vector<int>& temp,
    int left,
    int right,
    Counters& count
) {
    if (left >= right) {
        return;
    }

    int middle = left + (right - left) / 2;

    mergeSortRecursive(arr, temp, left, middle, count);
    mergeSortRecursive(arr, temp, middle + 1, right, count);

    mergeParts(arr, temp, left, middle, right, count);
}

void mergeSort(vector<int>& arr, Counters& count) {
    if (arr.empty()) {
        return;
    }

    vector<int> temp(arr.size());

    mergeSortRecursive(
        arr, temp, 0, static_cast<int>(arr.size()) - 1, count
    );
}

// Run one algorithm and measure its execution time.
BenchmarkResult runBenchmark(
    const string& algorithm,
    const vector<int>& original
) {
    vector<int> arr = original;
    Counters count;

    auto start = steady_clock::now();

    if (algorithm == "bubble") {
        bubbleSort(arr, count);
    } else if (algorithm == "insertion") {
        insertionSort(arr, count);
    } else if (algorithm == "merge") {
        mergeSort(arr, count);
    } else {
        throw invalid_argument("Unknown algorithm");
    }

    auto finish = steady_clock::now();

    // Verify the algorithm actually sorted the array.
    if (!is_sorted(arr.begin(), arr.end())) {
        throw runtime_error("Sorting verification failed");
    }

    double elapsed = duration<double, milli>(finish - start).count();

    return {
        algorithm,
        elapsed,
        count.comparisons,
        count.swaps
    };
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            cerr << "Usage: benchmark_engine <algorithm> <size>\n";
            return 1;
        }

        string algorithm = argv[1];
        int size = stoi(argv[2]);

        if (algorithm != "all" &&
            algorithm != "bubble" &&
            algorithm != "insertion" &&
            algorithm != "merge") {
            cerr << "Invalid algorithm\n";
            return 1;
        }

        // Safety limits for the website.
        if (size < 1 || size > 10000) {
            cerr << "Input size must be between 1 and 10000\n";
            return 1;
        }

        vector<int> original;
        original.reserve(size);

        mt19937 generator(12345);
        uniform_int_distribution<int> distribution(1, 1000000);

        for (int i = 0; i < size; ++i) {
            original.push_back(distribution(generator));
        }

        cout << "algorithm,time_ms,comparisons,swaps\n";
        cout << fixed << setprecision(3);

        vector<string> algorithms;

        if (algorithm == "all") {
            algorithms = {"bubble", "insertion", "merge"};
        } else {
            algorithms = {algorithm};
        }

        for (const string& name : algorithms) {
            BenchmarkResult result = runBenchmark(name, original);

            cout << result.algorithm << ","
                 << result.time_ms << ","
                 << result.comparisons << ","
                 << result.swaps << "\n";
        }

        return 0;
    } catch (const exception& error) {
        cerr << "Benchmark error: " << error.what() << "\n";
        return 1;
    }
}