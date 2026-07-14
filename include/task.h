#ifndef TASK_H
#define TASK_H

#include <stdbool.h>
#include "constants.h"

typedef struct
{
    char title[MAX_TITLE_LEN];
    bool completed;
} SubTask;

typedef struct
{
    char title[MAX_TITLE_LEN];
    char note[MAX_NOTE_LEN];
    int priority;
    bool completed;
    char deadline[11];
    char categories[MAX_CATEGORIES][MAX_CATEGORY_LEN];
    int categoryCount;
    SubTask subtasks[MAX_SUBTASKS];
    int subtaskCount;
} Task;

void init_tasks(void);

#endif