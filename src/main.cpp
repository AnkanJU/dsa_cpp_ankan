#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/Record.hpp"
#include "../include/CommandEngine.hpp"

class NexusStorageEngine {
private:
    std::vector<Record> storageBuffer;
    CommandEngine commandPipeline;

public:
    // Step 1: Submit client requests to the Queue
    void submitRequest(const std::string& type, int id, const std::string& key, double value) {
        commandPipeline.enqueueCommand(type, id, key, value);
    }

    // Step 2: Process queued requests sequentially (FIFO)
    void processNextCommand() {
        if (!commandPipeline.hasPendingCommands()) {
            std::cout << "[NexusEngine] No pending commands in queue." << std::endl;
            return;
        }

        Command cmd = commandPipeline.getNextCommand();
        std::cout << "\n[Engine Processing] Executing " << cmd.type << " for ID: " << cmd.recordId << std::endl;

        if (cmd.type == "INSERT") {
            storageBuffer.emplace_back(cmd.recordId, cmd.key, cmd.value);
            // Push reverse action (REMOVE) to Undo Stack
            commandPipeline.pushRollbackAction("REMOVE_RECORD", cmd.recordId, cmd.key, cmd.value);
        }
    }

    // Step 3: Undo last executed action (LIFO)
    void rollbackLastAction() {
        if (!commandPipeline.canRollback()) {
            std::cout << "[NexusEngine] Nothing to rollback." << std::endl;
            return;
        }

        RollbackAction action = commandPipeline.popRollbackAction();
        std::cout << "\n[Engine Rollback] Undoing action for ID: " << action.recordId << "..." << std::endl;

        if (action.reverseType == "REMOVE_RECORD") {
            storageBuffer.erase(
                std::remove_if(storageBuffer.begin(), storageBuffer.end(),
                    [action](const Record& r) { return r.id == action.recordId; }),
                storageBuffer.end()
            );
            std::cout << "[Engine Rollback] Record ID " << action.recordId << " removed successfully." << std::endl;
        }
    }

    void displayAll() const {
        std::cout << "\n--- Current Storage Buffer State (" << storageBuffer.size() << " records) ---" << std::endl;
        for (const auto& r : storageBuffer) {
            r.print();
        }
        std::cout << "---------------------------------------------------------\n" << std::endl;
    }
};

int main() {
    NexusStorageEngine engine;

    std::cout << "=== Phase 1: Queuing Client Requests ===" << std::endl;
    engine.submitRequest("INSERT", 101, "cpu_usage", 45.2);
    engine.submitRequest("INSERT", 102, "memory_usage", 78.9);

    std::cout << "\n=== Phase 2: Processing Queue (FIFO) ===" << std::endl;
    engine.processNextCommand(); // Executes 101
    engine.processNextCommand(); // Executes 102

    engine.displayAll();

    std::cout << "=== Phase 3: Rolling Back Transactions (LIFO Stack) ===" << std::endl;
    engine.rollbackLastAction(); // Undoes ID 102 (most recent)

    engine.displayAll();

    return 0;
}