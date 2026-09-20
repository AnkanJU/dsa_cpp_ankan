#ifndef BST_INDEX_HPP
#define BST_INDEX_HPP

#include <iostream>
#include <vector>
#include "Record.hpp"

struct BSTNode {
    Record record;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Record rec) 
        : record(std::move(rec)), left(nullptr), right(nullptr) {}
};

class BSTIndex {
private:
    BSTNode* root;

    // Helper: Recursive insertion
    BSTNode* insertRecursive(BSTNode* node, const Record& rec) {
        if (node == nullptr) {
            return new BSTNode(rec);
        }
        if (rec.id < node->record.id) {
            node->left = insertRecursive(node->left, rec);
        } else if (rec.id > node->record.id) {
            node->right = insertRecursive(node->right, rec);
        }
        return node;
    }

    // Helper: Recursive In-Order Traversal (Sorted Output)
    void inOrderRecursive(BSTNode* node) const {
        if (node == nullptr) return;
        inOrderRecursive(node->left);
        node->record.print();
        inOrderRecursive(node->right);
    }

    // Helper: Recursive Range Search [lowId, highId]
    void rangeSearchRecursive(BSTNode* node, int lowId, int highId, std::vector<Record>& results) const {
        if (node == nullptr) return;

        // Check left subtree if low bound is smaller than current node ID
        if (lowId < node->record.id) {
            rangeSearchRecursive(node->left, lowId, highId, results);
        }

        // Check if current node falls within range
        if (lowId <= node->record.id && node->record.id <= highId) {
            results.push_back(node->record);
        }

        // Check right subtree if high bound is larger than current node ID
        if (highId > node->record.id) {
            rangeSearchRecursive(node->right, lowId, highId, results);
        }
    }

    // Helper: Memory cleanup
    void destroyTree(BSTNode* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    BSTIndex() : root(nullptr) {}

    ~BSTIndex() {
        destroyTree(root);
    }

    void insert(int id, const std::string& key, double val) {
        Record rec(id, key, val);
        root = insertRecursive(root, rec);
        std::cout << "[BST Index] Inserted Node ID: " << id << std::endl;
    }

    void printSorted() const {
        std::cout << "\n--- BST Sorted Index View ---" << std::endl;
        inOrderRecursive(root);
        std::cout << "-----------------------------\n" << std::endl;
    }

    std::vector<Record> rangeQuery(int lowId, int highId) const {
        std::vector<Record> results;
        rangeSearchRecursive(root, lowId, highId, results);
        return results;
    }
};

#endif // BST_INDEX_HPP