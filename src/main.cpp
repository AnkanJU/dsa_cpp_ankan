#include <iostream>
#include "../include/Record.hpp"
#include "../include/KeyIndex.hpp"

class NexusStorageEngine {
private:
    KeyIndex indexEngine;

public:
    void set(const std::string& key, int id, double value) {
        indexEngine.insertOrUpdate(key, id, value);
    }

    void get(const std::string& key) const {
        Record rec(0, "", 0.0);
        if (indexEngine.findKey(key, rec)) {
            std::cout << "[NexusEngine Cache Hit] ";
            rec.print();
        } else {
            std::cout << "[NexusEngine Cache Miss] Key '" << key << "' does not exist." << std::endl;
        }
    }

    void remove(const std::string& key) {
        if (!indexEngine.removeKey(key)) {
            std::cout << "[NexusEngine] Failed to delete key '" << key << "'. Not found." << std::endl;
        }
    }

    void showIndex() const {
        indexEngine.displayIndexStats();
    }
};

int main() {
    NexusStorageEngine engine;

    std::cout << "=== Phase 1: Fast O(1) Key-Value Insertions ===" << std::endl;
    engine.set("cpu_usage", 101, 45.2);
    engine.set("memory_usage", 102, 78.9);
    engine.set("disk_io", 103, 112.4);

    engine.showIndex();

    std::cout << "=== Phase 2: O(1) Direct Key Lookups ===" << std::endl;
    engine.get("memory_usage"); // Cache hit
    engine.get("network_out");  // Cache miss

    std::cout << "\n=== Phase 3: O(1) Key Deletion ===" << std::endl;
    engine.remove("cpu_usage");

    engine.showIndex();

    return 0;
}