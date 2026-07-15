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

void init_todo_color(){
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
}