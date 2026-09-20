#include <iostream>
#include <vector>
#include "../include/Record.hpp"
#include "../include/BSTIndex.hpp"

class NexusStorageEngine {
private:
    BSTIndex treeIndex;

public:
    void insertRecord(int id, const std::string& key, double value) {
        treeIndex.insert(id, key, value);
    }

    void displayInOrder() const {
        treeIndex.printSorted();
    }

    void executeRangeQuery(int minId, int maxId) const {
        std::cout << "=== Executing Range Query: IDs [" << minId << " to " << maxId << "] ===" << std::endl;
        std::vector<Record> matches = treeIndex.rangeQuery(minId, maxId);
        
        if (matches.empty()) {
            std::cout << "No records found in range." << std::endl;
        } else {
            for (const auto& r : matches) {
                r.print();
            }
        }
        std::cout << "=========================================================\n" << std::endl;
    }
};

int main() {
    NexusStorageEngine engine;

    std::cout << "=== Phase 1: Populating Hierarchical BST Index ===" << std::endl;
    engine.insertRecord(300, "db_port", 5432.0);
    engine.insertRecord(150, "max_conn", 100.0);
    engine.insertRecord(450, "cache_size", 512.0);
    engine.insertRecord(100, "timeout", 30.0);
    engine.insertRecord(200, "retry_count", 3.0);

    // Display elements automatically sorted via in-order traversal
    engine.displayInOrder();

    std::cout << "=== Phase 2: Range Query Filtering ===" << std::endl;
    // Retrieve records with ID between 120 and 350
    engine.executeRangeQuery(120, 350);

    return 0;
}