
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

long long comparisons = 0;
long long swaps = 0;

void bubbleSort(vector<int>& arr) {
    comparisons = 0;
    swaps = 0;

    for (int i = 0; i < (int)arr.size() - 1; i++) {
        for (int j = 0; j < (int)arr.size() - i - 1; j++) {
            comparisons++;

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swaps++;
            }
        }
    }
}

void insertionSort(vector<int>& arr) {
    comparisons = 0;
    swaps = 0;

    for (int i = 1; i < (int)arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            comparisons++;

            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                swaps++;
                j--;
            } else {
                break;
            }
        }

        arr[j + 1] = key;
    }
}

void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp;
    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right) {
        comparisons++;

        if (arr[i] <= arr[j]) {
            temp.push_back(arr[i++]);
        } else {
            temp.push_back(arr[j++]);
        }
    }

    while (i <= mid) {
        temp.push_back(arr[i++]);
    }

    while (j <= right) {
        temp.push_back(arr[j++]);
    }

    for (int k = 0; k < (int)temp.size(); k++) {
        arr[left + k] = temp[k];
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int main() {
    vector<int> original;

    for (int i = 1000; i >= 1; i--) {
        original.push_back(i);
    }

    vector<int> a = original;
    vector<int> b = original;
    vector<int> c = original;

    cout << "=== AlgoSentinel: Sorting Analysis ===\n\n";

    bubbleSort(a);
    cout << "Bubble Sort\n";
    cout << "Comparisons: " << comparisons << "\n";
    cout << "Swaps: " << swaps << "\n";
    cout << "Sorted correctly: "
         << (is_sorted(a.begin(), a.end()) ? "Yes" : "No")
         << "\n\n";

    insertionSort(b);
    cout << "Insertion Sort\n";
    cout << "Comparisons: " << comparisons << "\n";
    cout << "Shifts: " << swaps << "\n";
    cout << "Sorted correctly: "
         << (is_sorted(b.begin(), b.end()) ? "Yes" : "No")
         << "\n\n";

    comparisons = 0;
    swaps = 0;
    mergeSort(c, 0, (int)c.size() - 1);

    cout << "Merge Sort\n";
    cout << "Comparisons: " << comparisons << "\n";
    cout << "Sorted correctly: "
         << (is_sorted(c.begin(), c.end()) ? "Yes" : "No")
         << "\n";

    cout << "\nTime Complexity:\n";
    cout << "Bubble Sort: O(n^2)\n";
    cout << "Insertion Sort: O(n^2) worst case\n";
    cout << "Merge Sort: O(n log n)\n";

    return 0;
}
