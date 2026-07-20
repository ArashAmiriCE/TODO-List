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
    tasks[taskCount].categoryCount = 0;
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

void del_subtask(){
    if(selectedTask == 0) return;
    if(whereWeAre != winSubTask) return;
    if(selectedSubTask == 0) return;
    int count_of_subtasks = tasks[selectedTask].subtaskCount;
    for(int i = selectedSubTask; i < count_of_subtasks; i++){
        tasks[selectedTask].subtasks[i] = tasks[selectedTask].subtasks[i + 1];
    }
    tasks[selectedTask].subtasks[count_of_subtasks].completed = false;
    for(int i = 0; i < MAX_TITLE_LEN; i++) tasks[selectedTask].subtasks[count_of_subtasks].title[i] = '\0';
    tasks[selectedTask].subtaskCount--;
    if(selectedSubTask > count_of_subtasks - 1) selectedSubTask = count_of_subtasks - 1;
    draw_all_windows();
}

void check_subtask(){
    if(whereWeAre != winSubTask || selectedSubTask == 0) return;
    tasks[selectedTask].subtasks[selectedSubTask].completed = true;
    bool isAllCheck = true;
    for(int i = 1; i <= tasks[selectedTask].subtaskCount; i++)
        if(tasks[selectedTask].subtasks[i].completed == false){
            isAllCheck = false;
            break;
        }
    if(isAllCheck) tasks[selectedTask].completed = true;
    draw_all_windows();
}

void uncheck_subtask(){
    if(whereWeAre != winSubTask || selectedSubTask == 0) return;
    tasks[selectedTask].subtasks[selectedSubTask].completed = false;
    if(tasks[selectedTask].completed == true) tasks[selectedTask].completed = false;
    draw_all_windows();
}

void edit_subtask(){
    if(whereWeAre != winSubTask || selectedTask == 0) return;
    int pos = 0;
    int ch;
    char title [MAX_TITLE_LEN + 1] = {0};
    clear_line(subtaskswin, selectedSubTask, 2, getmaxx(subtaskswin) - 1);
    wattron(subtaskswin, COLOR_PAIR(1));
    wattron(subtaskswin, A_REVERSE);
    wmove(subtaskswin, selectedSubTask, 2);
    wprintw(subtaskswin, "%d. [%c] ", selectedSubTask, tasks[selectedTask].subtasks[selectedSubTask].completed ? 'x' : ' ');
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
        clear_line(subtaskswin, selectedSubTask, 2, getmaxx(subtaskswin) - 1);
        wattron(subtaskswin, COLOR_PAIR(1));
        wattron(subtaskswin, A_REVERSE);
        mvwprintw(subtaskswin, selectedSubTask, 2, "%d. [%c] %s", selectedSubTask,tasks[selectedTask].subtasks[selectedSubTask].completed ? 'x' : ' ', title);
        wrefresh(subtaskswin);
    }
    wattroff(subtaskswin, A_REVERSE);
    wattroff(subtaskswin, COLOR_PAIR(1));
    for(int i = 0; i < MAX_TITLE_LEN; i++)
        tasks[selectedTask].subtasks[selectedSubTask].title[i] = title[i];
    draw_subtasks();
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

void edit_description(){
    if(whereWeAre != winTask || selectedTask == 0) return;
    char description[MAX_DESCRIPTION_LEN + 1] = {0};
    strncpy(description, tasks[selectedTask].description, MAX_DESCRIPTION_LEN);
    int pos = strlen(description);
    int ch;
    wattron(descriptionwin, COLOR_PAIR(1));
    werase(descriptionwin);
    box(descriptionwin, 0, 0);
    mvwprintw(descriptionwin, 0, 1, " Description ");
    wattroff(descriptionwin, COLOR_PAIR(1));
    int line = 1;
    int col = 2;
    wattron(descriptionwin, COLOR_PAIR(1));
    wattron(descriptionwin, A_REVERSE);
    for (int i = 0; description[i] != '\0'; i++) {
        if (col >= getmaxx(descriptionwin) - 2) {
            line++;
            col = 2;
        }
        mvwprintw(descriptionwin, line, col, "%c", description[i]);
        col++;
    }
    wattroff(descriptionwin, A_REVERSE);
    wattroff(descriptionwin, COLOR_PAIR(1));
    wrefresh(descriptionwin);
    while ((ch = getch()) != '\n' && ch != KEY_ENTER) {
        if (ch == KEY_BACKSPACE || ch == 127) {
            if (pos > 0) {
                pos--;
                description[pos] = '\0';
            }
        }
        else if (ch >= 32 && ch <= 126 && pos < MAX_DESCRIPTION_LEN) {
            description[pos] = (char)ch;
            pos++;
            description[pos] = '\0';
        }
        wattron(descriptionwin, COLOR_PAIR(1));
        werase(descriptionwin);
        box(descriptionwin, 0, 0);
        mvwprintw(descriptionwin, 0, 1, " Description ");
        wattroff(descriptionwin, COLOR_PAIR(1));
        int line = 1;
        int col = 2;
        wattron(descriptionwin, COLOR_PAIR(1));
        wattron(descriptionwin, A_REVERSE);
        for (int i = 0; description[i] != '\0'; i++) {
            if (col >= getmaxx(descriptionwin) - 2) {
                line++;
                col = 2;
            }
            mvwprintw(descriptionwin, line, col, "%c", description[i]);
            col++;
        }
        wattroff(descriptionwin, A_REVERSE);
        wattroff(descriptionwin, COLOR_PAIR(1));
        wrefresh(descriptionwin);
    }
    strncpy(tasks[selectedTask].description, description, MAX_DESCRIPTION_LEN);
    tasks[selectedTask].description[MAX_DESCRIPTION_LEN - 1] = '\0';
    draw_all_windows();
}

void insert_deadline(){
    int day, month, year = 0;
    wattron(deadlinewin, COLOR_PAIR(1));
    wmove(deadlinewin, 1, 2);
    wprintw(deadlinewin,"0000/00/");
    wattron(deadlinewin, A_REVERSE);
    wprintw(deadlinewin, "_");
    wattroff(deadlinewin, A_REVERSE);
    wprintw(deadlinewin, "0");
    wrefresh(deadlinewin);
    char ch = getch();
    if((ch - '0') >= 0 && (ch - '0') <= 3) day = 10 * (ch - '0');
    else {
        draw_deadline();
        return;
    }
    mvwprintw(deadlinewin, 1, 10, "%c", ch);
    wattron(deadlinewin, A_REVERSE);
    mvwprintw(deadlinewin, 1, 11, "_");
    wattroff(deadlinewin, A_REVERSE);
    wrefresh(deadlinewin);
    ch = getch();
    day += ch - '0';
    if (day < 1 || day > 32){
        draw_deadline();
        return;
    }
    mvwprintw(deadlinewin, 1, 11, "%c", ch);
    wattron(deadlinewin, A_REVERSE);
    mvwprintw(deadlinewin, 1, 7, "_");
    wattroff(deadlinewin, A_REVERSE);
    wrefresh(deadlinewin);
    ch = getch();
    month = 10 * (ch - '0');
    if((ch - '0') != 0 && (ch - '0') != 1){
        draw_deadline();
        return;
    }
    mvwprintw(deadlinewin, 1, 7, "%c", ch);
    wattron(deadlinewin, A_REVERSE);
    mvwprintw(deadlinewin, 1, 8, "_");
    wattroff(deadlinewin, A_REVERSE);
    wrefresh(deadlinewin);
    ch = getch();
    month += ch - '0';
    if(month < 1 || month > 12){
        draw_deadline();
        return;
    }
    mvwprintw(deadlinewin, 1, 8, "%c", ch);
    for(int i = 0; i < 4; i++){
        wattron(deadlinewin, A_REVERSE);
        mvwprintw(deadlinewin, 1, 2 + i, "_");
        wattroff(deadlinewin, A_REVERSE);
        wrefresh(deadlinewin);
        ch = getch();
        year = year * 10 + (ch - '0');
        mvwprintw(deadlinewin, 1, 2 + i, "%c", ch);
    }
    tasks[selectedTask].deadlineDay = day;
    tasks[selectedTask].deadlineMonth = month;
    tasks[selectedTask].deadlineYear = year;
    draw_all_windows();
}

void add_category(){
    if(selectedTask == 0 || whereWeAre != winCategory) return;
    selectedCategory = 0;
    draw_categories();
    char title[MAX_CATEGORY_LEN + 1] = {0};
    int pos = 0;
    int cursorRow = tasks[selectedTask].categoryCount + 1; 
    int ch;
    
    wattron(categorieswin, COLOR_PAIR(1));
    wattron(categorieswin, A_REVERSE);
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
        wattroff(categorieswin, A_REVERSE);
        wattroff(categorieswin, COLOR_PAIR(1));
        clear_line(categorieswin, cursorRow, 2, getmaxx(categorieswin) - 1);
        wattron(categorieswin, COLOR_PAIR(1));
        wattron(categorieswin, A_REVERSE);
        mvwprintw(categorieswin, cursorRow, 2, "%s", title);
        wrefresh(categorieswin);
    }
    wattroff(categorieswin, A_REVERSE);
    wattroff(categorieswin, COLOR_PAIR(1));
    tasks[selectedTask].categoryCount++;
    strncpy(tasks[selectedTask].categories[tasks[selectedTask].categoryCount], title, MAX_TITLE_LEN - 1);
    selectedCategory = 1;
    draw_all_windows();
    refresh();
}

void del_category(){
    if(whereWeAre != winCategory || selectedCategory == 0) return;
    for(int i = selectedCategory; i < tasks[selectedTask].categoryCount; i++){
        strncpy(tasks[selectedTask].categories[i], tasks[selectedTask].categories[i + 1], MAX_CATEGORY_LEN + 1);
    }
    for(int i = 0; i < MAX_CATEGORY_LEN; i++){
        tasks[selectedTask].categories[tasks[selectedTask].categoryCount][i] = '\0';
    }
    tasks[selectedTask].categoryCount--;
    if(selectedCategory > tasks[selectedTask].categoryCount) selectedCategory = tasks[selectedTask].categoryCount;
    draw_all_windows();
}
