#ifndef KEY_INDEX_HPP
#define KEY_INDEX_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include "Record.hpp"

class KeyIndex {
private:
    // O(1) Average Lookup Hash Map: String Key -> Record Pointer/Data
    std::unordered_map<std::string, Record> primaryIndex;

    // O(1) Average Set: Tracks unique registered keys
    std::unordered_set<std::string> activeKeys;

public:
    // O(1) Insertion
    void insertOrUpdate(const std::string& key, int id, double value) {
        Record newRecord(id, key, value);
        primaryIndex.insert_or_assign(key, newRecord);
        activeKeys.insert(key);
        std::cout << "[Hash Index] Indexed key: '" << key << "' with value: " << value << std::endl;
    }

    // O(1) Direct Key Lookup
    bool findKey(const std::string& key, Record& resultRecord) const {
        auto it = primaryIndex.find(key);
        if (it != primaryIndex.end()) {
            resultRecord = it->second;
            return true; // Found
        }
        return false; // Not found
    }

    // O(1) Key Deletion
    bool removeKey(const std::string& key) {
        if (activeKeys.count(key)) {
            activeKeys.erase(key);
            primaryIndex.erase(key);
            std::cout << "[Hash Index] Deleted key: '" << key << "' from hash table." << std::endl;
            return true;
        }
        return false;
    }

    // Check key existence in O(1)
    bool hasKey(const std::string& key) const {
        return activeKeys.find(key) != activeKeys.end();
    }

    void displayIndexStats() const {
        std::cout << "\n=== KeyIndex Hash Table Snapshot (" << primaryIndex.size() << " keys) ===" << std::endl;
        for (const auto& pair : primaryIndex) {
            std::cout << "Key: '" << pair.first << "' -> Record ID: " << pair.second.id 
                      << " | Value: " << pair.second.value << std::endl;
        }
        std::cout << "=========================================================\n" << std::endl;
    }
};

#endif // KEY_INDEX_HPP