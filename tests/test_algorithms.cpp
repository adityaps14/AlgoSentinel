
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

int linearSearch(const vector<int>& arr, int target) {
    for (int i = 0; i < (int)arr.size(); i++)
        if (arr[i] == target) return i;
    return -1;
}

int binarySearch(const vector<int>& arr, int target) {
    int left = 0, right = (int)arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }

    return -1;
}

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

void merge(vector<int>& a, int l, int m, int r) {
    vector<int> temp;
    int i = l, j = m + 1;

    while (i <= m && j <= r) {
        if (a[i] <= a[j]) temp.push_back(a[i++]);
        else temp.push_back(a[j++]);
    }

    while (i <= m) temp.push_back(a[i++]);
    while (j <= r) temp.push_back(a[j++]);

    for (int k = 0; k < (int)temp.size(); k++)
        a[l + k] = temp[k];
}

void mergeSort(vector<int>& a, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(a, l, m);
    mergeSort(a, m + 1, r);
    merge(a, l, m, r);
}

int main() {
    cout << "Running AlgoSentinel tests...\n";

    vector<int> sorted = {1, 2, 3, 4, 5};
    assert(linearSearch(sorted, 3) == 2);
    assert(binarySearch(sorted, 3) == 2);
    assert(linearSearch(sorted, 9) == -1);
    assert(binarySearch(sorted, 9) == -1);
    cout << "[PASS] Search tests\n";

    vector<int> a = {5, 1, 4, 2, 3};
    vector<int> b = a;
    vector<int> c = a;

    bubbleSort(a);
    insertionSort(b);
    mergeSort(c, 0, (int)c.size() - 1);

    vector<int> expected = {1, 2, 3, 4, 5};

    assert(a == expected);
    assert(b == expected);
    assert(c == expected);
    cout << "[PASS] Sorting tests\n";

    vector<int> empty;
    assert(linearSearch(empty, 1) == -1);
    assert(binarySearch(empty, 1) == -1);
    cout << "[PASS] Empty-array tests\n";

    cout << "\nAll tests passed!\n";
    return 0;
}
