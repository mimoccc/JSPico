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

#include "Task.h"

class TaskManager {
private:
    uint32_t taskIndex;
    std::vector<Task*>tasks ={};
public:
    TaskManager();
    ~TaskManager();

    size_t size();

    void addTask(Task* task);

    void operator+=(Task* task);

    bool isEmpty();

    bool isNotEmpty();

    void clear();

    bool processNextTask();
};

#endif //JS_PICO_TASK_MANAGER_H