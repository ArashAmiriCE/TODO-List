#ifndef GLOBALS_H
#define GLOBALS_H

#include <ncurses.h>
#include "task.h"

extern Task tasks[MAX_TASKS];
extern int taskCount;
extern int selectedTask;
extern int running;
extern WINDOW *taskswin;
extern WINDOW *descriptionwin;
extern WINDOW *categorieswin;
extern WINDOW *subtaskswin;
extern WINDOW *deadlinewin;
void init_todo_color();
#endif