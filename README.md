\# AlgoSentinel

\### Algorithm Performance Analysis and Benchmarking System



AlgoSentinel is a C++ project designed to analyze and compare the performance of searching and sorting algorithms. It measures execution time, counts algorithm operations, and generates benchmark results in CSV format.



\## Features



\- Compare Linear Search and Binary Search.

\- Implement Bubble Sort, Insertion Sort, and Merge Sort.

\- Measure execution time for different input sizes.

\- Count comparisons, swaps, and shifts where supported.

\- Export benchmark results to a CSV file.

\- Display Big-O time complexity explanations.

\- Run automated tests for searching, sorting, and empty arrays.



\## Technologies Used



\- \*\*Language:\*\* C++

\- \*\*Standard:\*\* C++11

\- \*\*Compiler:\*\* GCC / G++

\- \*\*Libraries:\*\* Standard C++ Library, `<chrono>`, `<fstream>`, `<algorithm>`



\## Project Structure



```text

AlgoSentinel/

├── src/

│   ├── main.cpp

│   ├── searching.cpp

│   ├── sorting.cpp

│   └── benchmark.cpp

├── tests/

│   └── test\_algorithms.cpp

├── reports/

│   └── benchmark\_results.csv

├── screenshots/

├── docs/

└── README.md

```



\## How to Compile and Run



Open Command Prompt in the project folder.



\### 1. Run the project introduction



```cmd

g++ -std=c++11 src\\main.cpp -o AlgoSentinel.exe

AlgoSentinel.exe

```



\### 2. Run searching algorithms



```cmd

g++ -std=c++11 src\\searching.cpp -o searching.exe

searching.exe

```



\### 3. Run sorting algorithms



```cmd

g++ -std=c++11 src\\sorting.cpp -o sorting.exe

sorting.exe

```



\### 4. Run the benchmark and export CSV results



```cmd

g++ -std=c++11 -O2 src\\benchmark.cpp -o benchmark.exe

benchmark.exe

```



The benchmark report is saved to `reports/benchmark\_results.csv`.



\### 5. Run automated tests



```cmd

g++ -std=c++11 tests\\test\_algorithms.cpp -o tests\\test\_algorithms.exe

tests\\test\_algorithms.exe

```



\## Algorithms and Complexity



| Algorithm | Best Case | Average Case | Worst Case |

|---|---|---|---|

| Linear Search | O(1) | O(n) | O(n) |

| Binary Search | O(1) | O(log n) | O(log n) |

| Bubble Sort | O(n) | O(n²) | O(n²) |

| Insertion Sort | O(n) | O(n²) | O(n²) |

| Merge Sort | O(n log n) | O(n log n) | O(n log n) |



Binary Search requires sorted input. Actual benchmark times depend on the computer, compiler settings, and input data.



\## Testing



Automated tests currently cover:

\- Searching algorithms

\- Sorting algorithms

\- Empty-array cases



\*\*Current test result:\*\* All 3 test groups passed.



\## Future Improvements



\- Add a graphical performance dashboard.

\- Improve timing accuracy using repeated benchmark runs.

\- Add more algorithms and input distributions.

\- Generate comparison charts automatically.



\## Author



Developed as an academic C++ project to demonstrate algorithm design, analysis, testing, and performance benchmarking.

