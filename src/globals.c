#include "../include/globals.h"
#include "../include/task.h"
#include <ncurses.h>
enum whichWindow whereWeAre;
Task tasks[MAX_TASKS];
int taskCount;
int selectedTask;
int selectedSubTask;
int selectedCategory;
int selectedSort;
int running;
WINDOW *taskswin;
WINDOW *descriptionwin;
WINDOW *categorieswin;
WINDOW *subtaskswin;
WINDOW *deadlinewin;
WINDOW *sortdecisionwin;

void init_todo_color(){
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_RED);
}