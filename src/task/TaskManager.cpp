#include "Task.h"
#include "TaskManager.h"

uint32_t TaskManager::taskIndex = 0;
std::vector<Task *> TaskManager::tasks = {};
std::vector<TaskArgs *> TaskManager::task_args = {};

TaskManager::TaskManager() = default;

void TaskManager::operator+=(Task *task) {
    tasks.push_back(task);
}

void TaskManager::operator+=(TaskArgs *taskArgs) {
    task_args.push_back(taskArgs);
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
                TaskArgs *task_a = findTaskArgs(t->task_name);
                if (task_a != nullptr) {
                    t->setArgs(task_a);
                }
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

bool TaskManager::addTaskArgs(TaskArgs *args) {
    task_args.push_back(args);
    return true;
}

TaskArgs *TaskManager::findTaskArgs(const char *name) {
    if (task_args.empty()) {
        return nullptr;
    } else {
        for (auto tArgs: task_args) {
            if (strncmp(name, tArgs->destination, strlen(name)) != 0) {
                remove(task_args.begin(), task_args.end(), tArgs);
                return tArgs;
            }
        }
        return nullptr;
    }
}
