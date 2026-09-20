#ifndef COMMAND_ENGINE_HPP
#define COMMAND_ENGINE_HPP

#include <iostream>
#include <string>
#include <queue>
#include <stack>

// Represents a pending engine command
struct Command {
    std::string type; // "INSERT", "DELETE"
    int recordId;
    std::string key;
    double value;

    Command(std::string t, int id, std::string k, double v)
        : type(std::move(t)), recordId(id), key(std::move(k)), value(v) {}
};

// Represents an inverse action stored on the rollback stack
struct RollbackAction {
    std::string reverseType; // "REMOVE_RECORD", "RESTORE_RECORD"
    int recordId;
    std::string key;
    double value;

    RollbackAction(std::string rt, int id, std::string k, double v)
        : reverseType(std::move(rt)), recordId(id), key(std::move(k)), value(v) {}
};

class CommandEngine {
private:
    std::queue<Command> incomingQueue;     // Queue for FIFO command execution
    std::stack<RollbackAction> undoStack; // Stack for LIFO transaction rollback

public:
    // Queue an incoming database request (FIFO)
    void enqueueCommand(const std::string& type, int id, const std::string& key, double val) {
        incomingQueue.emplace(type, id, key, val);
        std::cout << "[Command Engine] Enqueued command: " << type << " for ID " << id << std::endl;
    }

    bool hasPendingCommands() const {
        return !incomingQueue.empty();
    }

    Command getNextCommand() {
        Command cmd = incomingQueue.front();
        incomingQueue.pop(); // Remove processed command from front of queue
        return cmd;
    }

    // Push reverse action to stack (LIFO)
    void pushRollbackAction(const std::string& revType, int id, const std::string& key, double val) {
        undoStack.emplace(revType, id, key, val);
        std::cout << "[Undo Engine] Pushed rollback action to stack for ID " << id << std::endl;
    }

    bool canRollback() const {
        return !undoStack.empty();
    }

    RollbackAction popRollbackAction() {
        RollbackAction action = undoStack.top();
        undoStack.pop(); // Remove top action from stack
        return action;
    }
};

#endif // COMMAND_ENGINE_HPP