#include <iostream>
#include <vector>
#include <stdexcept>
#include "../include/Record.hpp"

class NexusStorageEngine {
private:
    std::vector<Record> storageBuffer; // Dynamic memory buffer

public:
    // Append to end: O(1) amortized
    void pushBack(int id, const std::string& key, double value) {
        storageBuffer.emplace_back(id, key, value);
        std::cout << "[NexusEngine] Appended record ID " << id << std::endl;
    }

    // Insert at specific index: O(n) due to element shift
    void insertAtIndex(size_t index, int id, const std::string& key, double value) {
        if (index > storageBuffer.size()) {
            throw std::out_of_range("Index out of bounds for insertion.");
        }
        storageBuffer.emplace(storageBuffer.begin() + index, id, key, value);
        std::cout << "[NexusEngine] Inserted record ID " << id << " at index " << index << std::endl;
    }

    // Delete at specific index: O(n) due to element shift
    void deleteAtIndex(size_t index) {
        if (index >= storageBuffer.size()) {
            throw std::out_of_range("Index out of bounds for deletion.");
        }
        int deletedId = storageBuffer[index].id;
        storageBuffer.erase(storageBuffer.begin() + index);
        std::cout << "[NexusEngine] Deleted record ID " << deletedId << " at index " << index << std::endl;
    }

    // Read by index: O(1) random access
    const Record& getRecord(size_t index) const {
        if (index >= storageBuffer.size()) {
            throw std::out_of_range("Index out of bounds.");
        }
        return storageBuffer[index];
    }

    size_t size() const {
        return storageBuffer.size();
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
    try {
        NexusStorageEngine engine;

        // Populating dynamic memory
        engine.pushBack(101, "cpu_usage", 45.2);
        engine.pushBack(103, "disk_io", 120.4);

        // Insert in middle (index 1)
        engine.insertAtIndex(1, 102, "memory_usage", 78.9);
        engine.displayAll();

        // Delete element at index 0
        engine.deleteAtIndex(0);
        engine.displayAll();

        // Direct lookup via O(1) access
        std::cout << "Accessing Index 0 directly:" << std::endl;
        engine.getRecord(0).print();

    } catch (const std::exception& e) {
        std::cerr << "[Error] Exception caught: " << e.what() << std::endl;
    }

    return 0;
}