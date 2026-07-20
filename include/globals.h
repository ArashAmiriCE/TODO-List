#ifndef GLOBALS_H
#define GLOBALS_H

#include <ncurses.h>
#include "task.h"

enum whichWindow {winTask, winCategory, winSubTask, winSort};
extern enum whichWindow whereWeAre;
extern Task tasks[MAX_TASKS];
extern int taskCount;
extern int selectedTask;
extern int selectedSubTask;
extern int selectedCategory;
extern int selectedSort;
extern WINDOW *taskswin;
extern WINDOW *descriptionwin;
extern WINDOW *categorieswin;
extern WINDOW *subtaskswin;
extern WINDOW *deadlinewin;
extern WINDOW *sortdecisionwin;
void init_todo_color();
#endif