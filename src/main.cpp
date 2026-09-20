#include <iostream>
#include "../include/TaskScheduler.hpp"

class NexusStorageEngine {
private:
    TaskScheduler scheduler;

public:
    void scheduleMaintenance(int priority, const std::string& name, const std::string& details) {
        scheduler.scheduleTask(priority, name, details);
    }

    void runMaintenanceCycle() {
        std::cout << "\n=== Running Engine Maintenance Cycle (" 
                  << scheduler.pendingTaskCount() << " pending) ===" << std::endl;
        while (scheduler.hasPendingTasks()) {
            scheduler.executeNextTask();
        }
        std::cout << "=========================================================\n" << std::endl;
    }
};

int main() {
    NexusStorageEngine engine;

    std::cout << "=== Phase 1: Scheduling Tasks with Varied Priorities ===" << std::endl;
    // Pushing tasks out of order
    engine.scheduleMaintenance(2, "Routine Index Cleanup", "Rebuilding fragmented indices");
    engine.scheduleMaintenance(10, "WAL Flush", "Flushing write-ahead log to persistent storage");
    engine.scheduleMaintenance(5, "Cache Eviction", "Clearing stale query cache entries");
    engine.scheduleMaintenance(1, "Log Archiving", "Archiving log history to secondary disk");

    // Execution order should follow priority scores: 10 -> 5 -> 2 -> 1
    engine.runMaintenanceCycle();

    return 0;
}