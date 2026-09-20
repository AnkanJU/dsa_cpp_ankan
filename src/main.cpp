#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include "../include/Record.hpp"
#include "../include/TransactionLog.hpp"
#include "../include/CommandEngine.hpp"
#include "../include/KeyIndex.hpp"
#include "../include/BSTIndex.hpp"
#include "../include/TaskScheduler.hpp"
#include "../include/DependencyGraph.hpp"

class NexusEngine {
private:
    std::vector<Record> storageBuffer;
    TransactionLog walLog;
    CommandEngine commandPipeline;
    KeyIndex hashIndex;
    BSTIndex bstIndex;
    TaskScheduler taskScheduler;
    DependencyGraph bootGraph;

    int currentTxId = 5000;
    bool isBooted = false;

public:
    // 1. Boot Subsystem using Graph Dependency Resolution
    void bootSystem() {
        std::cout << "=========================================================" << std::endl;
        std::cout << "           NEXUS ENGINE SYSTEM BOOT SEQUENCE             " << std::endl;
        std::cout << "=========================================================\n" << std::endl;

        // Uses addDependency matching DependencyGraph's public API
        bootGraph.addDependency("Allocate_Memory", "Init_WAL_Log");
        bootGraph.addDependency("Init_WAL_Log", "Build_Indices");
        bootGraph.addDependency("Build_Indices", "Start_Scheduler");
        bootGraph.addDependency("Start_Scheduler", "Ready");

        std::vector<std::string> bootOrder = bootGraph.resolveExecutionOrder();
        for (size_t i = 0; i < bootOrder.size(); ++i) {
            std::cout << "[Boot Step " << i + 1 << "] Initialized: " << bootOrder[i] << std::endl;
        }

        isBooted = true;
        std::cout << "\n[NexusEngine] All subsystems initialized and ready.\n" << std::endl;
    }

    // 2. Multi-Engine Write Operation
    void insertRecord(int id, const std::string& key, double value) {
        if (!isBooted) {
            std::cerr << "[NexusEngine Error] Engine not booted!" << std::endl;
            return;
        }

        std::cout << "\n>>> [Write Path] Processing Key: '" << key << "' (ID: " << id << ") <<<" << std::endl;

        // Step A: Queue transaction
        commandPipeline.enqueueCommand("INSERT", id, key, value);

        // Step B: Write-Ahead Log entry (Singly Linked List)
        walLog.logOperation(++currentTxId, "INSERT", key);

        // Step C: Execute write to primary buffer, Hash Table, and BST
        storageBuffer.emplace_back(id, key, value);
        hashIndex.insertOrUpdate(key, id, value);
        bstIndex.insert(id, key, value);

        // Step D: Push rollback frame to Stack
        commandPipeline.pushRollbackAction("REMOVE_RECORD", id, key, value);
    }

    // 3. Fast O(1) Key Retrieval
    void getByKey(const std::string& key) const {
        Record result(0, "", 0.0);
        if (hashIndex.findKey(key, result)) {
            std::cout << "[NexusEngine Hash Hit] ";
            result.print();
        } else {
            std::cout << "[NexusEngine Cache Miss] Key '" << key << "' not found." << std::endl;
        }
    }

    // 4. Tree-Based Range Query
    void queryRange(int minId, int maxId) const {
        std::cout << "\n=== [BST Range Query] Records with IDs [" << minId << " to " << maxId << "] ===" << std::endl;
        std::vector<Record> matches = bstIndex.rangeQuery(minId, maxId);
        for (const auto& r : matches) {
            r.print();
        }
        std::cout << "=========================================================\n" << std::endl;
    }

    // 5. Rollback Operation
    void undoLastWrite() {
        if (!commandPipeline.canRollback()) return;

        RollbackAction action = commandPipeline.popRollbackAction();
        std::cout << "\n<<< [Engine Rollback] Undoing ID " << action.recordId << " (" << action.key << ") <<<" << std::endl;

        hashIndex.removeKey(action.key);
        storageBuffer.erase(
            std::remove_if(storageBuffer.begin(), storageBuffer.end(),
                [action](const Record& r) { return r.id == action.recordId; }),
            storageBuffer.end()
        );
        std::cout << "[Engine Rollback] Record successfully evicted." << std::endl;
    }

    // 6. Schedule Maintenance
    void addMaintenanceTask(int priority, const std::string& name, const std::string& detail) {
        taskScheduler.scheduleTask(priority, name, detail);
    }

    void runMaintenance() {
        std::cout << "\n=== [Engine Maintenance Execution Pipeline] ===" << std::endl;
        while (taskScheduler.hasPendingTasks()) {
            taskScheduler.executeNextTask();
        }
        std::cout << "=========================================================\n" << std::endl;
    }

    // 7. System Diagnostics Output
    void printDiagnostics() const {
        std::cout << "\n=========================================================" << std::endl;
        std::cout << "               NEXUS ENGINE DIAGNOSTICS                  " << std::endl;
        std::cout << "=========================================================" << std::endl;
        std::cout << " Storage Buffer Records : " << storageBuffer.size() << std::endl;
        walLog.printLogHistory();
        hashIndex.displayIndexStats();
        bstIndex.printSorted();
    }
};

int main() {
    NexusEngine engine;

    // Phase 1: Boot subsystem
    engine.bootSystem();

    // Phase 2: Ingest sample workload
    engine.insertRecord(300, "db_port", 5432.0);
    engine.insertRecord(150, "max_connections", 250.0);
    engine.insertRecord(450, "cache_size_mb", 1024.0);
    engine.insertRecord(100, "idle_timeout_s", 60.0);

    // Phase 3: Hash lookups & range query
    std::cout << "\n=== Direct & Range Lookups ===" << std::endl;
    engine.getByKey("max_connections");
    engine.queryRange(120, 400);

    // Phase 4: Maintenance scheduler
    engine.addMaintenanceTask(1, "Routine Clean", "Sweeping stale sessions");
    engine.addMaintenanceTask(10, "Emergency WAL Flush", "Flushing dirty log pages to disk");
    engine.addMaintenanceTask(5, "Reindex BST", "Balancing range nodes");
    engine.runMaintenance();

    // Phase 5: Transaction Rollback
    engine.undoLastWrite();

    // Phase 6: Final Diagnostics Snapshot
    engine.printDiagnostics();

    return 0;
}