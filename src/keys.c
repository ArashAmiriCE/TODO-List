#include <ncurses.h>
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
            break;
        case 'a':
            add_task();
            break;
        case 'd':
            if(whereWeAre == winTask) del_task();
            break;
        case ' ':
            if(whereWeAre == winTask && selectedTask != 0){
                if(tasks[selectedTask].completed == false) check_task();
                else uncheck_task();
            }
            break;
        case 'e':
            if(whereWeAre == winTask) edit_task();
            break;
        default:
            break;
        }
    } while((int)input != 27);
}