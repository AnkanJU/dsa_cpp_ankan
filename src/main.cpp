#include <iostream>
#include <vector>
#include "../include/DependencyGraph.hpp"

class NexusStorageEngine {
private:
    DependencyGraph depEngine;

public:
    void registerDependency(const std::string& prerequisite, const std::string& dependentTask) {
        depEngine.addDependency(prerequisite, dependentTask);
    }

    void executePipeline() {
        std::cout << "\n=== Resolving Engine Execution Order (Topological Sort) ===" << std::endl;
        std::vector<std::string> order = depEngine.resolveExecutionOrder();

        if (!order.empty()) {
            std::cout << "\n[Pipeline Step Sequence]:" << std::endl;
            for (size_t i = 0; i < order.size(); ++i) {
                std::cout << " Step " << i + 1 << ": Execute -> " << order[i] << std::endl;
            }
        }
        std::cout << "=========================================================\n" << std::endl;
    }
};

int main() {
    NexusStorageEngine engine;

    std::cout << "=== Phase 1: Registering Engine Task Dependencies ===" << std::endl;
    // Task dependencies setup
    engine.registerDependency("Init_Storage", "Load_Indexes");
    engine.registerDependency("Load_Indexes", "Start_QueryEngine");
    engine.registerDependency("Init_Storage", "Allocate_Buffer");
    engine.registerDependency("Allocate_Buffer", "Start_QueryEngine");
    engine.registerDependency("Start_QueryEngine", "Accept_Client_Connections");

    // Execute pipeline in strictly valid dependency order
    engine.executePipeline();

    return 0;
}