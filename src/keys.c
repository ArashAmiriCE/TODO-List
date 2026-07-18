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
            if(selectedSubTask == 0 && selectedCategory == 0){
                if(selectedTask == 0) selectedTask = taskCount;
                else {
                    selectedTask = selectedTask - 1;
                    if(selectedTask == 0) selectedTask = taskCount;
                }
                draw_all_windows();
                break;
            }
            else if (selectedSubTask != 0 && selectedCategory != 0){
                //will be error message soon!
                break;
            }
            else if (selectedSubTask != 0){
                selectedSubTask -= 1;
                if(selectedSubTask == 0) selectedSubTask = tasks[selectedTask].subtaskCount;
                draw_all_windows();
                break;
            }
            else if (selectedCategory != 0){
                selectedCategory -= 1;
                if(selectedCategory == 0) selectedCategory = tasks[selectedTask].categoryCount;
                draw_all_windows();
                break;
            }
            break;
        case 'j':
            if(selectedSubTask == 0 && selectedCategory == 0){
                if (taskCount == 0) break;
                if(selectedTask == 0) selectedTask = 1;
                else {
                    selectedTask = selectedTask + 1;
                    if(selectedTask > taskCount) selectedTask = 1;
                }
                draw_all_windows();
                break;
            }
            else if (selectedSubTask != 0 && selectedCategory != 0){
                //will be error message soon!
                break;
            }
            else if (selectedSubTask != 0){
                selectedSubTask = selectedSubTask + 1;
                if(selectedSubTask > tasks[selectedTask].subtaskCount) selectedSubTask = 1;
                draw_all_windows();
                break;
            }
            else if (selectedCategory != 0){
                selectedCategory = selectedCategory + 1;
                if(selectedCategory > tasks[selectedTask].categoryCount) selectedCategory = 1;
                draw_all_windows();
                break;
            }
            break;
        case 'a':
            add_task();
            break;
        default:
            break;
        }
    } while((int)input != 27);
}