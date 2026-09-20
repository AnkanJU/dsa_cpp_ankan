#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "../include/Record.hpp"

class NexusStorageEngine {
private:
    std::vector<Record> storageBuffer;
    bool isSorted = false; // Tracks if buffer is currently sorted by ID

public:
    void pushBack(int id, const std::string& key, double value) {
        storageBuffer.emplace_back(id, key, value);
        isSorted = false; // New un-sorted element added
    }

    // Helper: Sort records by ID to enable Binary Search
    void sortById() {
        std::sort(storageBuffer.begin(), storageBuffer.end(), 
            [](const Record& a, const Record& b) {
                return a.id < b.id;
            });
        isSorted = true;
        std::cout << "[NexusEngine] Storage buffer sorted by ID." << std::endl;
    }

    // 1. Linear Search: O(n) - Works on unsorted data
    int linearSearchById(int targetId) const {
        for (size_t i = 0; i < storageBuffer.size(); ++i) {
            if (storageBuffer[i].id == targetId) {
                return static_cast<int>(i); // Found index
            }
        }
        return -1; // Not found
    }

    // 2. Binary Search: O(log n) - Requires sorted data
    int binarySearchById(int targetId) {
        if (!isSorted) {
            sortById(); // Ensure sorted state before search
        }

        int low = 0;
        int high = static_cast<int>(storageBuffer.size()) - 1;

        while (low <= high) {
            int mid = low + (high - low) / 2; // Avoid potential integer overflow

            if (storageBuffer[mid].id == targetId) {
                return mid; // Found index
            }
            if (storageBuffer[mid].id < targetId) {
                low = mid + 1; // Search right half
            } else {
                high = mid - 1; // Search left half
            }
        }

        return -1; // Not found
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

    // Populating unsorted IDs
    engine.pushBack(405, "network_out", 1024.5);
    engine.pushBack(101, "cpu_usage", 45.2);
    engine.pushBack(302, "disk_io", 88.1);
    engine.pushBack(204, "memory_usage", 78.9);

    engine.displayAll();

    // Perform Linear Search on unsorted data
    std::cout << "--- Testing Linear Search O(n) ---" << std::endl;
    int idx1 = engine.linearSearchById(302);
    if (idx1 != -1) {
        std::cout << "Linear Search found ID 302 at index: " << idx1 << std::endl;
    }

    // Perform Binary Search (triggers auto-sort first)
    std::cout << "\n--- Testing Binary Search O(log n) ---" << std::endl;
    int idx2 = engine.binarySearchById(302);
    if (idx2 != -1) {
        std::cout << "Binary Search found ID 302 at index: " << idx2 << std::endl;
    }

    engine.displayAll();

    return 0;
}