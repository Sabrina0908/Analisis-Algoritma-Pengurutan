#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        int min_idx = i;
        for (int j = i+1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        swap(arr[min_idx], arr[i]);
    }
}

void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int i = 0; i < n2; i++)
        R[i] = arr[m + 1 + i];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

vector<int> generateRandomArray(int size) {    //Menghasilkan array acak
    vector<int> arr(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10000);
    for (int i = 0; i < size; i++) {
        arr[i] = dis(gen);
    }
    return arr;
}

vector<int> generateSortedArray(int size) {    //Menghasilkan array terurut
    vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
    return arr;
}

vector<int> generateReverseSortedArray(int size) {    //Menghasilkan array terurut terbalik
    vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
    return arr;
}

template <typename Func>
long long measureExecutionTime(Func func, vector<int>& arr) {
    auto start = high_resolution_clock::now();
    func(arr);
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

int main() {
    vector<int> sizes = {10, 100, 500, 1000, 10000};
    vector<string> algorithms = {"Bubble Sort", "Insertion Sort", "Selection Sort", "Merge Sort", "Quick Sort"};
    vector<void(*)(vector<int>&)> sortingFunctions = {bubbleSort, insertionSort, selectionSort, 
                                                      [](vector<int>& arr){ mergeSort(arr, 0, arr.size() - 1); }, 
                                                      [](vector<int>& arr){ quickSort(arr, 0, arr.size() - 1); }};
    
    vector<string> conditions = {"Random", "Sorted", "Reverse Sorted"};
    vector<vector<int>(*)(int)> dataGenerators = {generateRandomArray, generateSortedArray, generateReverseSortedArray};

    for (const auto& condition : conditions) {
        cout << "Condition: " << condition << endl;
        cout << "Algoritma\tN=10 (μs)\tN=100 (μs)\tN=500 (μs)\tN=1000 (μs)\tN=10000 (μs)\n";
        for (size_t i = 0; i < algorithms.size(); i++) {
            cout << algorithms[i] << "\t";
            for (int size : sizes) {
                vector<int> arr = dataGenerators[&condition - &conditions[0]](size);
                long long time = measureExecutionTime(sortingFunctions[i], arr);
                cout << time << "\t\t";
            }
            cout << "\n";
        }
        cout << "\n";
    }
    return 0;
}
