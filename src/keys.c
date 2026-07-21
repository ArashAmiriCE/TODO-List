#include <ncurses.h>
#include <string.h>
#include "../include/globals.h"
#include "../include/tui.h"
#include "../include/task.h"

void key_input(){
    noecho();
    char input;
    do{
        input = getch();
        switch (input)
        {
        case 'k':
            if(whereWeAre == winTask){
                if(selectedTask == 0) selectedTask = taskCount;
                else {
                    selectedTask = selectedTask - 1;
                    if(selectedTask == 0) selectedTask = taskCount;
                }
                draw_all_windows();
                break;
            }
            else if (whereWeAre == winSubTask){
                if(tasks[selectedTask].subtaskCount == 0) break;
                selectedSubTask -= 1;
                if(selectedSubTask == 0) selectedSubTask = tasks[selectedTask].subtaskCount;
                draw_all_windows();
                break;
            }
            else if (whereWeAre == winCategory){
                selectedCategory -= 1;
                if(selectedCategory == 0) selectedCategory = tasks[selectedTask].categoryCount;
                draw_all_windows();
                break;
            }
            else if (whereWeAre == winSort){
                selectedSort--;
                if(selectedSort < 1) selectedSort = 2;
                draw_sortdecision();
                break;
            }
            break;
        case 'j':
            if(whereWeAre == winTask){
                if (taskCount == 0) break;
                if(selectedTask == 0) selectedTask = 1;
                else {
                    selectedTask = selectedTask + 1;
                    if(selectedTask > taskCount) selectedTask = 1;
                }
                draw_all_windows();
                break;
            }
            else if (whereWeAre == winSubTask){
                if(tasks[selectedTask].subtaskCount == 0) break;
                selectedSubTask = selectedSubTask + 1;
                if(selectedSubTask > tasks[selectedTask].subtaskCount) selectedSubTask = 1;
                draw_all_windows();
                break;
            }
            else if (whereWeAre == winCategory){
                selectedCategory = selectedCategory + 1;
                if(selectedCategory > tasks[selectedTask].categoryCount) selectedCategory = 1;
                draw_all_windows();
                break;
            }
            else if (whereWeAre == winSort){
                selectedSort++;
                if(selectedSort > 2) selectedSort = 1;
                draw_sortdecision();
                break;
            }
            break;
        case 'a':
            if(whereWeAre == winTask) add_task();
            if(whereWeAre == winSubTask) add_subtask();
            if(whereWeAre == winCategory) add_category();
            break;
        case 'l':
            if (selectedTask > 0){
                whereWeAre = winSubTask;
                if(tasks[selectedTask].subtaskCount == 0){
                    wattron(subtaskswin, COLOR_PAIR(1));
                    mvwprintw(subtaskswin, 1, 2, "No subtasks yet!");
                    wattroff(subtaskswin, COLOR_PAIR(1));
                    wrefresh(subtaskswin);
                }
                else{
                    selectedSubTask = 1;
                    draw_subtasks();
                }
                break;
            }
            break;
        case 'h':
            if(whereWeAre == winSubTask){
                selectedSubTask = 0;
                whereWeAre = winTask;
                draw_subtasks();
                break;
            }
            break;
        case 'd':
            if(whereWeAre == winSubTask) del_subtask();
            else if(whereWeAre == winTask) del_task();
            else if(whereWeAre == winCategory) del_category();
            break;
        case ' ':
            if(whereWeAre == winSubTask && selectedSubTask != 0){
                if(tasks[selectedTask].subtasks[selectedSubTask].completed == false)
                    check_subtask();
                else
                    uncheck_subtask();
            }
            else if(whereWeAre == winTask && selectedTask != 0){
                if(tasks[selectedTask].completed == false) check_task();
                else uncheck_task();
            }
            break;
        case 'e':
            if(whereWeAre == winSubTask) edit_subtask();
            else if(whereWeAre == winTask) edit_task();
            break;
        case 'r':
            if(whereWeAre == winTask) edit_description();
            break;
        case 'n':
            if(whereWeAre == winTask) insert_deadline();
            break;
        case 'c':
            if (selectedTask > 0 && whereWeAre == winTask){
                whereWeAre = winCategory;
                if(tasks[selectedTask].categoryCount == 0){
                    wattron(categorieswin, COLOR_PAIR(1));
                    mvwprintw(categorieswin, 1, 2, "No categories yet!");
                    wattroff(categorieswin, COLOR_PAIR(1));
                    wrefresh(categorieswin);
                }
                else{
                    selectedCategory = 1;
                    draw_categories();
                }
                break;
            }
            else if (selectedTask > 0 && whereWeAre == winCategory){
                selectedCategory = 0;
                whereWeAre = winTask;
                draw_all_windows();
                break;
            }
            break;
        case 's':
            if(whereWeAre == winTask){
                whereWeAre = winSort;
                draw_sortdecision();
                break;
            }
        case '\n':
            if(whereWeAre == winSort && selectedSort != 0){
                if (selectedSort == 1) {
                    for (int i = 1; i < taskCount; i++) {
                        for (int j = i + 1; j <= taskCount; j++) {
                            if (strcmp(tasks[i].title, tasks[j].title) > 0) {
                                Task temp = tasks[i];
                                tasks[i] = tasks[j];
                                tasks[j] = temp;
                            }
                        }
                    }
                } 
                else if (selectedSort == 2) {
                    for (int i = 1; i < taskCount; i++) {
                        for (int j = i + 1; j <= taskCount; j++) {
                            int year1 = tasks[i].deadlineYear == 0 ? 9999 : tasks[i].deadlineYear;
                            int year2 = tasks[j].deadlineYear == 0 ? 9999 : tasks[j].deadlineYear;
                            if (year1 > year2 ||
                                (year1 == year2 && tasks[i].deadlineMonth > tasks[j].deadlineMonth) ||
                                (year1 == year2 && tasks[i].deadlineMonth == tasks[j].deadlineMonth && tasks[i].deadlineDay > tasks[j].deadlineDay)) {
                                Task temp = tasks[i];
                                tasks[i] = tasks[j];
                                tasks[j] = temp;
                            }
                        }
                    }
                }
                selectedSort = 0;
                whereWeAre = winTask;
                selectedTask = 0;
                werase(sortdecisionwin);
                draw_all_windows();
            }
            break;
        case 'w':
            save_tasks_to_file();
            draw_success();
            break;
        default:
            break;
        }
    } while((int)input != 27);
}