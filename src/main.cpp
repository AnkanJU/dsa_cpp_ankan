#include <iostream>
#include <vector>
#include "../include/Record.hpp"

class NexusStorageEngine {
private:
    std::vector<Record> storageBuffer; // Dynamic memory buffer

public:
    void insertRecord(int id, const std::string& key, double value) {
        storageBuffer.emplace_back(id, key, value);
        std::cout << "[NexusEngine] Successfully stored record ID: " << id << std::endl;
    }

    void displayAll() const {
        std::cout << "\n--- Current Storage Buffer State ---" << std::endl;
        for (const auto& record : storageBuffer) {
            std::cout << "ID: " << record.id 
                      << " | Key: " << record.key 
                      << " | Value: " << record.value << std::endl;
        }
    }
};

int main() {
    std::cout << "Starting NexusEngine v1.0.0..." << std::endl;

    NexusStorageEngine engine;
    engine.insertRecord(101, "cpu_usage", 45.2);
    engine.insertRecord(102, "memory_usage", 78.9);

    engine.displayAll();

    return 0;
}