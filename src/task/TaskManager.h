#ifndef JS_PICO_TASK_MANAGER_H
#define JS_PICO_TASK_MANAGER_H

#include <cassert>
#include <cinttypes>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

#include "Task.h"

class TaskManager {
private:
    static uint32_t taskIndex;
    static std::vector<Task*>tasks;
    static std::vector<TaskArgs*>task_args;
public:
    TaskManager();
    ~TaskManager();

    static size_t size();

    static void addTask(Task* task);

    void operator+=(Task* task);

    void operator+=(TaskArgs* task);

    static bool isEmpty();

    static bool isNotEmpty();

    static void clear();

    static bool processNextTask();

    static bool addTaskArgs(TaskArgs * args);

    static TaskArgs *findTaskArgs(const char *name);
};

#endif //JS_PICO_TASK_MANAGER_H