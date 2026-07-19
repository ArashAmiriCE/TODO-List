#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include "../include/task.h"
#include "../include/globals.h"
#include "../include/tui.h"

static void clear_line(WINDOW *win, int row, int col_start, int col_end) {
    for (int i = col_start; i < col_end; i++) {
        mvwaddch(win, row, i, ' ');
    }
}

void add_task() {
    selectedTask = 0;
    draw_tasks();
    char title[MAX_TITLE_LEN + 1] = {0};
    int pos = 0;
    int cursorRow = taskCount + 1; 
    int ch;
    
    wattron(taskswin, COLOR_PAIR(1));
    wattron(taskswin, A_REVERSE);
    mvwprintw(taskswin, cursorRow, 2, "%d. [ ] ", taskCount + 1);
    wrefresh(taskswin);
    
    while ((ch = getch()) != '\n' && ch != KEY_ENTER) {
        if (ch == KEY_BACKSPACE || ch == 127){
            if (pos > 0) {
                pos--;
                title[pos] = '\0';
            }
        }
        else if (ch >= 32 && ch <= 126 && pos < MAX_TITLE_LEN) {
            title[pos] = (char)ch;
            pos++;
            title[pos] = '\0';
        }
        wattroff(taskswin, A_REVERSE);
        wattroff(taskswin, COLOR_PAIR(1));
        clear_line(taskswin, cursorRow, 2, getmaxx(taskswin) - 1);
        wattron(taskswin, COLOR_PAIR(1));
        wattron(taskswin, A_REVERSE);
        mvwprintw(taskswin, cursorRow, 2, "%d. [ ] %s", taskCount + 1, title);
        wrefresh(taskswin);
    }
    wattroff(taskswin, A_REVERSE);
    wattroff(taskswin, COLOR_PAIR(1));
    taskCount++;
    strncpy(tasks[taskCount].title, title, MAX_TITLE_LEN - 1);
    tasks[taskCount].title[MAX_TITLE_LEN - 1] = '\0';
    tasks[taskCount].description[0] = '\0';
    tasks[taskCount].priority = 0;
    tasks[taskCount].completed = false;
    tasks[taskCount].deadlineDay = 0;
    tasks[taskCount].deadlineMonth = 0;
    tasks[taskCount].deadlineYear = 0;
    tasks[taskCount].categoryCount = 1;
    for (int i = 0; i < MAX_CATEGORIES; i++) {
        tasks[taskCount].categories[i][0] = '\0';
    }
    tasks[taskCount].subtaskCount = 0;
    for (int i = 0; i < MAX_SUBTASKS; i++) {
        tasks[taskCount].subtasks[i].title[0] = '\0';
        tasks[taskCount].subtasks[i].completed = false;
    }
    selectedTask = 1;
    selectedSubTask = 0;
    selectedCategory = 0;
    draw_all_windows();
    refresh();
}

void del_task(){
    if(selectedTask == 0 || whereWeAre != winTask) return;
    for(int i = selectedTask; i < taskCount; i++){
        tasks[i] = tasks[i + 1];
    }
    for(int i = 0; i < MAX_TITLE_LEN; i++) tasks[taskCount].title[i] = '\0';
    for(int i = 0; i < MAX_DESCRIPTION_LEN; i++) tasks[taskCount].description[i] = '\0';
    tasks[taskCount].priority = 0;
    tasks[taskCount].completed = false;
    tasks[taskCount].deadlineDay = 0;
    tasks[taskCount].deadlineMonth = 0;
    tasks[taskCount].deadlineYear = 0;
    for(int i = 0; i < MAX_CATEGORIES; i++)
        for(int j = 0; j < MAX_CATEGORY_LEN; j++)
            tasks[taskCount].categories[i][j] = '\0';
    tasks[taskCount].categoryCount = 0;
    for(int i = 0; i < MAX_SUBTASKS; i++){
        tasks[taskCount].subtasks[i].completed = false;
        for(int j = 0; j < MAX_TITLE_LEN; j++){
            tasks[taskCount].subtasks[i].title[j] = '\0';
        }
    }
    tasks[taskCount].subtaskCount = 0;
    taskCount--;
    if(selectedTask > taskCount) selectedTask = taskCount;
    draw_all_windows();
}

void check_task(){
    if(whereWeAre != winTask || selectedTask == 0) return;
    tasks[selectedTask].completed = true;
    for(int i = 1; i <= tasks[selectedTask].subtaskCount; i++){
        tasks[selectedTask].subtasks[i].completed = true;
    }
    draw_all_windows();
}

void uncheck_task(){
    if(whereWeAre != winTask || selectedTask == 0) return;
    tasks[selectedTask].completed = false;
    for(int i = 1; i <= tasks[selectedTask].subtaskCount; i++){
        tasks[selectedTask].subtasks[i].completed = false;
    }
    draw_all_windows();
}

void edit_task(){
    if(whereWeAre != winTask || selectedTask == 0) return;
    int pos = 0;
    int ch;
    char title [MAX_TITLE_LEN + 1] = {0};
    clear_line(taskswin, selectedTask, 2, getmaxx(taskswin) - 1);
    wattron(taskswin, COLOR_PAIR(1));
    wattron(taskswin, A_REVERSE);
    wmove(taskswin, selectedTask, 2);
    wprintw(taskswin, "%d. [%c] ", selectedTask, tasks[selectedTask].completed ? 'x' : ' ');
    wrefresh(taskswin);
    while((ch = getch()) != '\n' && ch != KEY_ENTER){
        if (ch == KEY_BACKSPACE || ch == 127){
            if (pos > 0) {
                pos--;
                title[pos] = '\0';
            }
        }
        else if (ch >= 32 && ch <= 126 && pos < MAX_TITLE_LEN) {
            title[pos] = (char)ch;
            pos++;
            title[pos] = '\0';
        }
        wattroff(taskswin, A_REVERSE);
        wattroff(taskswin, COLOR_PAIR(1));
        clear_line(taskswin, selectedTask, 2, getmaxx(taskswin) - 1);
        wattron(taskswin, COLOR_PAIR(1));
        wattron(taskswin, A_REVERSE);
        mvwprintw(taskswin, selectedTask, 2, "%d. [%c] %s", selectedTask,tasks[selectedTask].completed ? 'x' : ' ', title);
        wrefresh(taskswin);
    }
    wattroff(taskswin, A_REVERSE);
    wattroff(taskswin, COLOR_PAIR(1));
    for(int i = 0; i < MAX_TITLE_LEN; i++)
        tasks[selectedTask].title[i] = title[i];
    draw_tasks();
}