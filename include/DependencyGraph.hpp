#ifndef DEPENDENCY_GRAPH_HPP
#define DEPENDENCY_GRAPH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

class DependencyGraph {
private:
    // Adjacency List: Node -> List of Dependent Nodes
    std::unordered_map<std::string, std::vector<std::string>> adjList;
    
    // In-Degree Map: Node -> Incoming Edge Count
    std::unordered_map<std::string, int> inDegree;

public:
    // Register a node if not already present
    void addNode(const std::string& node) {
        if (inDegree.find(node) == inDegree.end()) {
            inDegree[node] = 0;
            adjList[node] = {};
        }
    }

    // Add directed edge: 'parent' must run BEFORE 'dependent'
    void addDependency(const std::string& parent, const std::string& dependent) {
        addNode(parent);
        addNode(dependent);

        adjList[parent].push_back(dependent);
        inDegree[dependent]++;
        std::cout << "[Graph] Dependency added: '" << parent << "' ---> '" << dependent << "'" << std::endl;
    }

    // Kahn's Algorithm for Topological Sort O(V + E)
    std::vector<std::string> resolveExecutionOrder() {
        std::queue<std::string> zeroInDegreeQueue;
        std::vector<std::string> executionOrder;

        // Step 1: Find all nodes with 0 initial prerequisites
        for (const auto& pair : inDegree) {
            if (pair.second == 0) {
                zeroInDegreeQueue.push(pair.first);
            }
        }

        // Step 2: Process nodes in dependency order
        while (!zeroInDegreeQueue.empty()) {
            std::string current = zeroInDegreeQueue.front();
            zeroInDegreeQueue.pop();
            executionOrder.push_back(current);

            // Reduce in-degree for all dependent neighbors
            for (const std::string& neighbor : adjList[current]) {
                inDegree[neighbor]--;
                if (inDegree[neighbor] == 0) {
                    zeroInDegreeQueue.push(neighbor);
                }
            }
        }

        // Step 3: Check for dependency cycle (deadlock)
        if (executionOrder.size() != inDegree.size()) {
            std::cerr << "[Graph Error] Cyclic dependency detected! Cannot resolve order." << std::endl;
            return {};
        }

        return executionOrder;
    }
};

#endif // DEPENDENCY_GRAPH_HPP