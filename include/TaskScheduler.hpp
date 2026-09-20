#ifndef TASK_SCHEDULER_HPP
#define TASK_SCHEDULER_HPP

#include <iostream>
#include <string>
#include <queue>
#include <vector>

struct EngineTask {
    int priority; // Higher integer = Higher urgency
    std::string taskName;
    std::string details;

    EngineTask(int p, std::string name, std::string d)
        : priority(p), taskName(std::move(name)), details(std::move(d)) {}

    // Overload '<' operator so std::priority_queue ranks higher priority values first
    bool operator<(const EngineTask& other) const {
        return priority < other.priority;
    }
};

class TaskScheduler {
private:
    std::priority_queue<EngineTask> taskHeap;

public:
    void scheduleTask(int priority, const std::string& taskName, const std::string& details) {
        taskHeap.emplace(priority, taskName, details);
        std::cout << "[Scheduler] Enqueued Task: '" << taskName 
                  << "' (Priority level: " << priority << ")" << std::endl;
    }

    bool hasPendingTasks() const {
        return !taskHeap.empty();
    }

    void executeNextTask() {
        if (taskHeap.empty()) {
            std::cout << "[Scheduler] No pending background tasks." << std::endl;
            return;
        }

        EngineTask topTask = taskHeap.top(); // Get highest priority element O(1)
        taskHeap.pop(); // Remove highest priority element O(log n)

        std::cout << "\n[Scheduler Executing] Priority " << topTask.priority 
                  << " -> Task: " << topTask.taskName 
                  << " | Details: " << topTask.details << std::endl;
    }

    size_t pendingTaskCount() const {
        return taskHeap.size();
    }
};

#endif // TASK_SCHEDULER_HPP