#include <iostream>
#include <vector>
#include "../include/Record.hpp"
#include "../include/TransactionLog.hpp"

class NexusStorageEngine {
private:
    std::vector<Record> storageBuffer;
    TransactionLog walLog; // Singly linked list for write-ahead logging
    int currentTxId = 1000;

public:
    void insertRecord(int id, const std::string& key, double value) {
        // Step 1: Write operation to linked list transaction log O(1)
        walLog.logOperation(++currentTxId, "INSERT", key);

        // Step 2: Write data to main storage buffer
        storageBuffer.emplace_back(id, key, value);
    }

    void showTransactionHistory() const {
        walLog.printLogHistory();
    }

    void displayAllRecords() const {
        std::cout << "--- Storage Buffer Snapshot (" << storageBuffer.size() << " records) ---" << std::endl;
        for (const auto& record : storageBuffer) {
            record.print();
        }
        std::cout << "-------------------------------------------\n" << std::endl;
    }
};

int main() {
    NexusStorageEngine engine;

    std::cout << "--- Executing Engine Operations ---" << std::endl;
    engine.insertRecord(101, "cpu_usage", 45.2);
    engine.insertRecord(102, "memory_usage", 78.9);
    engine.insertRecord(103, "disk_io", 112.4);

    // Display transaction logs (Traversing linked list)
    engine.showTransactionHistory();

    // Display current main storage records
    engine.displayAllRecords();

    return 0;
}