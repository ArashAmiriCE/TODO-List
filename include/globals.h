#ifndef GLOBALS_H
#define GLOBALS_H

#include <ncurses.h>
#include "task.h"

enum whichWindow {winTask, winCategory, winSubTask, winDeadline, winDescription};
extern enum whichWindow whereWeAre;
extern Task tasks[MAX_TASKS];
extern int taskCount;
extern int selectedTask;
extern int selectedSubTask;
extern int selectedCategory;
extern int running;
extern WINDOW *taskswin;
extern WINDOW *descriptionwin;
extern WINDOW *categorieswin;
extern WINDOW *subtaskswin;
extern WINDOW *deadlinewin;
void init_todo_color();
#endif