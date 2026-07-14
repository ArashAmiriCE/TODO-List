#include "../include/globals.h"
#include "../include/task.h"
#include <ncurses.h>

Task tasks[MAX_TASKS];
int taskCount;
int selectedTask;
int running;
WINDOW *taskswin;
WINDOW *descriptionwin;
WINDOW *categorieswin;
WINDOW *subtaskswin;
WINDOW *deadlinewin; 