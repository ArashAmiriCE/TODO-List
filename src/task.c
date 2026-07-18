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

void add_subtask(){
    char title[MAX_TITLE_LEN + 1] = {0};
    selectedSubTask = 0;
    draw_subtasks();
    int pos = 0;
    int cursorRow = tasks[selectedTask].subtaskCount + 1; 
    int ch;
    wattron(subtaskswin, COLOR_PAIR(1));
    wattron(subtaskswin, A_REVERSE);
    mvwprintw(subtaskswin, cursorRow, 2, "%d. [ ] ", tasks[selectedTask].subtaskCount + 1);
    wrefresh(subtaskswin);
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
        wattroff(subtaskswin, A_REVERSE);
        wattroff(subtaskswin, COLOR_PAIR(1));
        clear_line(subtaskswin, cursorRow, 2, getmaxx(subtaskswin) - 1);
        wattron(subtaskswin, COLOR_PAIR(1));
        wattron(subtaskswin, A_REVERSE);
        mvwprintw(subtaskswin, cursorRow, 2, "%d. [ ] %s", tasks[selectedTask].subtaskCount + 1, title);
        wrefresh(subtaskswin);
    }
    wattroff(subtaskswin, A_REVERSE);
    wattroff(subtaskswin, COLOR_PAIR(1));
    tasks[selectedTask].subtaskCount++;
    strncpy(tasks[selectedTask].subtasks[tasks[selectedTask].subtaskCount].title, title, MAX_TITLE_LEN - 1);
    tasks[selectedTask].subtasks[tasks[selectedTask].subtaskCount].title[MAX_TITLE_LEN - 1] = '\0';
    tasks[selectedTask].subtasks[tasks[selectedTask].subtaskCount].completed = false;
    selectedSubTask = 1;
    draw_all_windows();
    refresh();
}