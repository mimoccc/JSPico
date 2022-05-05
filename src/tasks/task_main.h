//-----------------------------------------------------------------------------

#ifndef JSPICO_TASK_MAIN_H
#define JSPICO_TASK_MAIN_H

//-----------------------------------------------------------------------------

void task_main_init();

//-----------------------------------------------------------------------------

void task_main_task();

//-----------------------------------------------------------------------------

VTASK TASK_MAIN = {
        .task_name = "main",
        .parameters = NULL,
        .priority = 1,
        .stack_depth = 255,
        .task_init = &task_main_init,
        .task_main = &task_main_task
};

//-----------------------------------------------------------------------------

#endif //JSPICO_TASK_MAIN_H

//-----------------------------------------------------------------------------