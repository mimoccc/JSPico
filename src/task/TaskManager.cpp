#include "Task.h"
#include "TaskManager.h"

TaskManager::TaskManager() {
    taskIndex = 0;
}

void TaskManager::operator+=(Task *task) {
    tasks.push_back(task);
}

size_t TaskManager::size() {
    return tasks.size();
}

void TaskManager::addTask(Task *task) {
    tasks.push_back(task);
}

bool TaskManager::isEmpty() {
    return tasks.empty();
}

bool TaskManager::isNotEmpty() {
    return (!tasks.empty());
}

void TaskManager::clear() {
    for (Task *t: tasks) free(t);
    tasks.clear();
}

bool TaskManager::processNextTask() {
    if (isEmpty()) {
        return false;
    } else {
        Task *t = tasks.at(taskIndex);
        if (t->isInitialized()) {
            if (t->isScheduledTime()) {
                t->proc();
            }
        } else {
            t->init();
        }
        taskIndex++;
        if (taskIndex >= size()) {
            taskIndex = 0;
        }
        return true;
    }
}

TaskManager::~TaskManager() {
    clear();
}
