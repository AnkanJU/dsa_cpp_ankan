#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "../include/Record.hpp"

class NexusStorageEngine {
private:
    std::vector<Record> storageBuffer;
    bool isSorted = false;

    // --- Merge Sort Helper Methods ---
    void merge(std::vector<Record>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<Record> L, R;
        L.reserve(n1);
        R.reserve(n2);

        for (int i = 0; i < n1; ++i) L.push_back(arr[left + i]);
        for (int j = 0; j < n2; ++j) R.push_back(arr[mid + 1 + j]);

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i].id <= R[j].id) {
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }

    void mergeSortRecursive(std::vector<Record>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSortRecursive(arr, left, mid);
            mergeSortRecursive(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }

    // --- Quick Sort Helper Methods ---
    int partition(std::vector<Record>& arr, int low, int high) {
        int pivotId = arr[high].id;
        int i = low - 1;

        for (int j = low; j < high; ++j) {
            if (arr[j].id < pivotId) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    void quickSortRecursive(std::vector<Record>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSortRecursive(arr, low, pi - 1);
            quickSortRecursive(arr, pi + 1, high);
        }
    }

public:
    void pushBack(int id, const std::string& key, double value) {
        storageBuffer.emplace_back(id, key, value);
        isSorted = false;
    }

    // Custom Merge Sort Trigger
    void sortWithMergeSort() {
        if (storageBuffer.empty()) return;
        mergeSortRecursive(storageBuffer, 0, static_cast<int>(storageBuffer.size()) - 1);
        isSorted = true;
        std::cout << "[NexusEngine] Storage buffer sorted using Merge Sort O(n log n)." << std::endl;
    }

    // Custom Quick Sort Trigger
    void sortWithQuickSort() {
        if (storageBuffer.empty()) return;
        quickSortRecursive(storageBuffer, 0, static_cast<int>(storageBuffer.size()) - 1);
        isSorted = true;
        std::cout << "[NexusEngine] Storage buffer sorted using Quick Sort O(n log n)." << std::endl;
    }

    void displayAll() const {
        std::cout << "\n--- Storage Buffer Snapshot (" << storageBuffer.size() << " records) ---" << std::endl;
        for (size_t i = 0; i < storageBuffer.size(); ++i) {
            std::cout << "Index [" << i << "]: ";
            storageBuffer[i].print();
        }
        std::cout << "-------------------------------------------\n" << std::endl;
    }
};

int main() {
    NexusStorageEngine engine;

    // Populating unsorted records
    engine.pushBack(500, "network_in", 512.0);
    engine.pushBack(120, "cpu_load", 12.5);
    engine.pushBack(340, "gpu_temp", 68.2);
    engine.pushBack(210, "disk_read", 99.4);

    engine.displayAll();

    // Sorting via Merge Sort
    std::cout << "--- Sorting via Merge Sort ---" << std::endl;
    engine.sortWithMergeSort();
    engine.displayAll();

    // Adding more records to reset sorted flag
    engine.pushBack(050, "fan_speed", 2200.0);
    engine.pushBack(280, "ram_usage", 44.1);

    // Sorting via Quick Sort
    std::cout << "--- Sorting via Quick Sort ---" << std::endl;
    engine.sortWithQuickSort();
    engine.displayAll();

    return 0;
}