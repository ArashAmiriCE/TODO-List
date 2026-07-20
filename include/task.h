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
    char description[MAX_DESCRIPTION_LEN];
    int priority;
    bool completed;
    int deadlineDay;
    int deadlineMonth;
    int deadlineYear;
    char categories[MAX_CATEGORIES][MAX_CATEGORY_LEN];
    int categoryCount;
    SubTask subtasks[MAX_SUBTASKS];
    int subtaskCount;
} Task;
void add_task();
void add_subtask();
void del_subtask();
void check_subtask();
void uncheck_subtask();
void edit_subtask();
void del_task();
void check_task();
void uncheck_task();
void edit_task();
void edit_description();
void insert_deadline();
void add_category();
#endif