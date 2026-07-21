#include <string.h>
#include <stdlib.h>
#include "cJSON.h"
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

void save_tasks_to_file() {
    cJSON *root = cJSON_CreateArray();
    if (root == NULL) {
        return;
    }
    for (int i = 1; i <= taskCount; i++) {
        Task *t = &tasks[i];
        cJSON *task_obj = cJSON_CreateObject();
        if (task_obj == NULL) {
            cJSON_Delete(root);
            return;
        }
        cJSON_AddStringToObject(task_obj, "title", t->title);
        cJSON_AddStringToObject(task_obj, "description", t->description);
        cJSON_AddNumberToObject(task_obj, "priority", t->priority);
        cJSON_AddBoolToObject(task_obj, "completed", t->completed);
        cJSON_AddNumberToObject(task_obj, "deadlineDay", t->deadlineDay);
        cJSON_AddNumberToObject(task_obj, "deadlineMonth", t->deadlineMonth);
        cJSON_AddNumberToObject(task_obj, "deadlineYear", t->deadlineYear);
        cJSON_AddNumberToObject(task_obj, "categoryCount", t->categoryCount);
        cJSON *categories_arr = cJSON_CreateArray();
        for (int j = 0; j < t->categoryCount; j++) {
            cJSON_AddItemToArray(categories_arr, cJSON_CreateString(t->categories[j+1])); 
        }
        cJSON_AddItemToObject(task_obj, "categories", categories_arr);
        cJSON *subtasks_arr = cJSON_CreateArray();
        for (int j = 1; j <= t->subtaskCount; j++) {
            cJSON *sub_obj = cJSON_CreateObject();
            cJSON_AddStringToObject(sub_obj, "title", t->subtasks[j].title);
            cJSON_AddBoolToObject(sub_obj, "completed", t->subtasks[j].completed);
            cJSON_AddItemToArray(subtasks_arr, sub_obj);
        }
        cJSON_AddItemToObject(task_obj, "subtasks", subtasks_arr);

        cJSON_AddItemToArray(root, task_obj);
    }
    char *json_string = cJSON_Print(root);
    if (json_string == NULL) {
        cJSON_Delete(root);
        return;
    }
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        free(json_string);
        cJSON_Delete(root);
        return;
    }
    fprintf(file, "%s", json_string);
    fclose(file);
    free(json_string);
    cJSON_Delete(root);
}

void load_tasks_from_file() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        return;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (file_size <= 0) {
        fclose(file);
        return;
    }
    char *content = (char *)malloc(file_size + 1);
    if (content == NULL) {
        fclose(file);
        return;
    }
    fread(content, 1, file_size, file);
    content[file_size] = '\0';
    fclose(file);
    cJSON *root = cJSON_Parse(content);
    free(content);
    if (root == NULL) {
        return;
    }
    if (!cJSON_IsArray(root)) {
        cJSON_Delete(root);
        return;
    }
    int count = cJSON_GetArraySize(root);
    if (count > MAX_TASKS) count = MAX_TASKS;
    taskCount = 0;
    for (int i = 0; i < count; i++) {
        cJSON *task_obj = cJSON_GetArrayItem(root, i);
        if (task_obj == NULL) continue;
        Task *t = &tasks[i+1];
        cJSON *title = cJSON_GetObjectItem(task_obj, "title");
        if (title && cJSON_IsString(title)) {
            strncpy(t->title, title->valuestring, MAX_TITLE_LEN - 1);
            t->title[MAX_TITLE_LEN - 1] = '\0';
        } else {
            t->title[0] = '\0';
        }

        cJSON *desc = cJSON_GetObjectItem(task_obj, "description");
        if (desc && cJSON_IsString(desc)) {
            strncpy(t->description, desc->valuestring, MAX_DESCRIPTION_LEN - 1);
            t->description[MAX_DESCRIPTION_LEN - 1] = '\0';
        } else {
            t->description[0] = '\0';
        }

        cJSON *prio = cJSON_GetObjectItem(task_obj, "priority");
        t->priority = (prio && cJSON_IsNumber(prio)) ? prio->valueint : 0;

        cJSON *comp = cJSON_GetObjectItem(task_obj, "completed");
        t->completed = (comp && cJSON_IsBool(comp)) ? comp->valueint : false;

        cJSON *day = cJSON_GetObjectItem(task_obj, "deadlineDay");
        t->deadlineDay = (day && cJSON_IsNumber(day)) ? day->valueint : 0;
        cJSON *mon = cJSON_GetObjectItem(task_obj, "deadlineMonth");
        t->deadlineMonth = (mon && cJSON_IsNumber(mon)) ? mon->valueint : 0;
        cJSON *year = cJSON_GetObjectItem(task_obj, "deadlineYear");
        t->deadlineYear = (year && cJSON_IsNumber(year)) ? year->valueint : 0;
        cJSON *cats = cJSON_GetObjectItem(task_obj, "categories");
        t->categoryCount = 0;
        if (cats && cJSON_IsArray(cats)) {
            int cat_count = cJSON_GetArraySize(cats);
            if (cat_count > MAX_CATEGORIES) cat_count = MAX_CATEGORIES;
            for (int j = 0; j < cat_count; j++) {
                cJSON *cat = cJSON_GetArrayItem(cats, j);
                if (cat && cJSON_IsString(cat)) {
                    strncpy(t->categories[j+1], cat->valuestring, MAX_CATEGORY_LEN - 1);
                    t->categories[j+1][MAX_CATEGORY_LEN - 1] = '\0';
                    t->categoryCount++;
                }
            }
        }
        cJSON *subs = cJSON_GetObjectItem(task_obj, "subtasks");
        t->subtaskCount = 0;
        if (subs && cJSON_IsArray(subs)) {
            int sub_count = cJSON_GetArraySize(subs);
            if (sub_count > MAX_SUBTASKS) sub_count = MAX_SUBTASKS;
            for (int j = 0; j < sub_count; j++) {
                cJSON *sub = cJSON_GetArrayItem(subs, j);
                if (sub == NULL) continue;
                cJSON *sub_title = cJSON_GetObjectItem(sub, "title");
                cJSON *sub_comp = cJSON_GetObjectItem(sub, "completed");
                if (sub_title && cJSON_IsString(sub_title)) {
                    strncpy(t->subtasks[j+1].title, sub_title->valuestring, MAX_TITLE_LEN - 1);
                    t->subtasks[j+1].title[MAX_TITLE_LEN - 1] = '\0';
                } else {
                    t->subtasks[j+1].title[0] = '\0';
                }
                t->subtasks[j+1].completed = (sub_comp && cJSON_IsBool(sub_comp)) ? sub_comp->valueint : false;
                t->subtaskCount++;
            }
        }

        taskCount++;
    }
    cJSON_Delete(root);
    selectedTask = 0;
    selectedSubTask = 0;
    selectedCategory = 0;
    selectedSort = 0;
}
