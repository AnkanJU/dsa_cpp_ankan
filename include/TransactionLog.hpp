#ifndef TRANSACTION_LOG_HPP
#define TRANSACTION_LOG_HPP

#include <string>
#include <iostream>

struct LogNode {
    int transactionId;
    std::string operation; // e.g., "INSERT", "UPDATE", "DELETE"
    std::string recordKey;
    LogNode* next; // Pointer to the next log node

    LogNode(int t_id, std::string op, std::string key)
        : transactionId(t_id), operation(std::move(op)), recordKey(std::move(key)), next(nullptr) {}
};

class TransactionLog {
private:
    LogNode* head;
    int logCount;

public:
    TransactionLog() : head(nullptr), logCount(0) {}

    // Destructor to free dynamically allocated linked list memory
    ~TransactionLog() {
        LogNode* current = head;
        while (current != nullptr) {
            LogNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    // O(1) prepend operation: Append new transaction to the front of the list
    void logOperation(int txId, const std::string& op, const std::string& key) {
        LogNode* newNode = new LogNode(txId, op, key);
        newNode->next = head; // Point new node to old head
        head = newNode;       // Update head to point to new node
        logCount++;
        std::cout << "[WAL Log] Added Tx #" << txId << " [" << op << " -> " << key << "]" << std::endl;
    }

    // Traversal: Read transaction history from newest to oldest
    void printLogHistory() const {
        std::cout << "\n=== Write-Ahead Transaction Log History (" << logCount << " entries) ===" << std::endl;
        LogNode* current = head;
        while (current != nullptr) {
            std::cout << "[Tx #" << current->transactionId << "] "
                      << current->operation << " on key: '" << current->recordKey << "'" << std::endl;
            current = current->next;
        }
        std::cout << "=========================================================\n" << std::endl;
    }
};

#endif // TRANSACTION_LOG_HPP