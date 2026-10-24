
#include <iostream>
#include <vector>
using namespace std;

int linearSearch(const vector<int>& arr, int target,
                 long long& comparisons) {
    comparisons = 0;

    for (int i = 0; i < (int)arr.size(); i++) {
        comparisons++;

        if (arr[i] == target) {
            return i;
        }
    }

    return -1;
}

int binarySearch(const vector<int>& arr, int target,
                 long long& comparisons) {
    comparisons = 0;

    int left = 0;
    int right = (int)arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        comparisons++;

        if (arr[mid] == target) {
            return mid;
        }

        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;
}

int main() {
    vector<int> arr;

    for (int i = 1; i <= 10000; i++) {
        arr.push_back(i);
    }

    int target = 9876;
    long long linearComparisons = 0;
    long long binaryComparisons = 0;

    int linearResult =
        linearSearch(arr, target, linearComparisons);

    int binaryResult =
        binarySearch(arr, target, binaryComparisons);

    cout << "=== AlgoSentinel: Search Analysis ===\n\n";

    cout << "Array size: " << arr.size() << "\n";
    cout << "Target: " << target << "\n\n";

    cout << "Linear Search\n";
    cout << "Index: " << linearResult << "\n";
    cout << "Comparisons: " << linearComparisons << "\n";
    cout << "Time Complexity: O(n)\n\n";

    cout << "Binary Search\n";
    cout << "Index: " << binaryResult << "\n";
    cout << "Comparisons: " << binaryComparisons << "\n";
    cout << "Time Complexity: O(log n)\n";

    return 0;
}
