#include <ncurses.h>
#include "../include/globals.h"
#include "../include/tui.h"

void init_tui(){
    initscr();
    // if (!has_colors()) {
    //     endwin();
    //     fprintf(stderr, "Your terminal doesn't support colors!");
    //     exit(EXIT_FAILURE);
    // }
    init_todo_color();
    int xmax, ymax;
    getmaxyx(stdscr, ymax, xmax);
    taskswin = newwin(ymax/2 + ymax/4, xmax/2, 0, 0);
    descriptionwin = newwin(ymax/2, xmax/2 , 0, xmax/2 );
    categorieswin = newwin(ymax/4, xmax/2 , ymax/2 , xmax/2 );
    subtaskswin = newwin(ymax/4, xmax/2, ymax/2 + ymax/4 , 0);
    deadlinewin = newwin(ymax/4, xmax/2 , ymax/2 + ymax/4 , xmax/2 );
    refresh();
    draw_tasks();
    draw_description();
    draw_categories();
    draw_subtasks();
    draw_deadline();
}

void draw_tasks(void){
    werase(taskswin);
    wattron(taskswin, COLOR_PAIR(1));
    box(taskswin, 0, 0);
    mvwprintw(taskswin, 0, 1, " Tasks ");

    for(int i = 1; i <= taskCount; i++)
    {
        if(i == selectedTask) wattron(taskswin, A_REVERSE);
        mvwprintw(taskswin,
                  i,
                  2,
                  "%d. [%c] %s",
                  i,
                  tasks[i].completed ? 'x' : ' ',
                  tasks[i].title);

        if(i == selectedTask)
            wattroff(taskswin, A_REVERSE);
    }
    wattroff(taskswin, COLOR_PAIR(1));
    wrefresh(taskswin);
}

void draw_description(void){
    werase(descriptionwin);
    wattron(descriptionwin, COLOR_PAIR(1));
    box(descriptionwin, 0, 0);
    mvwprintw(descriptionwin, 0, 1, " Description ");
    if(selectedTask != 0){
        mvwprintw(descriptionwin, 1,1,tasks[selectedTask].description);
    }
    wattroff(descriptionwin, COLOR_PAIR(1));
    wrefresh(descriptionwin);
    return;
}

void draw_subtasks(void){
    werase(subtaskswin);
    wattron(subtaskswin, COLOR_PAIR(1));
    box(subtaskswin, 0, 0);
    mvwprintw(subtaskswin, 0, 1, " Subtasks ");
    if (selectedTask != 0){
        for (int i = 1; i <= tasks[selectedTask].subtaskCount; i++){
            if(i == selectedSubTask) wattron(subtaskswin, A_REVERSE);
            mvwprintw(subtaskswin,
                  i,
                  2,
                  "%d. [%c] %s",
                  i,
                  tasks[selectedTask].subtasks[i].completed ? 'x' : ' ',
                  tasks[selectedTask].subtasks[i].title);

            if(i == selectedSubTask) wattroff(subtaskswin, A_REVERSE);
        }
    }
    wattroff(subtaskswin, COLOR_PAIR(1));
    wrefresh(subtaskswin);
}

void draw_categories(){
    werase(categorieswin);
    wattron(categorieswin, COLOR_PAIR(1));
    box(categorieswin, 0, 0);
    mvwprintw(categorieswin, 0, 1, " Categories ");
    if (selectedTask != 0){
        for (int i = 1; i <= tasks[selectedTask].categoryCount; i++){
            if(i == selectedCategory) wattron(categorieswin, A_REVERSE);
            mvwprintw(categorieswin,
                  i,
                  2,
                  " %s",
                  tasks[selectedTask].categories[i]);

            if(i == selectedCategory) wattroff(categorieswin, A_REVERSE);
        }
    }
    wattroff(categorieswin, COLOR_PAIR(1));
    wrefresh(categorieswin);
}

void draw_deadline(){
    werase(deadlinewin);
    wattron(deadlinewin, COLOR_PAIR(1));
    box(deadlinewin, 0, 0);
    mvwprintw(deadlinewin, 0, 1, " Deadline ");
    if (selectedTask != 0){
        if (tasks[selectedTask].deadlineDay == 0 || tasks[selectedTask].deadlineMonth == 0 || tasks[selectedTask].deadlineYear == 0){
            mvwprintw(deadlinewin, 1, 1, "No deadline!");
            wattroff(deadlinewin, COLOR_PAIR(1));
            wrefresh(deadlinewin);
            return;
        }
        else{
            int yearDigits = 0;
            int yearCopy = tasks[selectedTask].deadlineYear;
            while(yearCopy != 0){
                yearCopy /= 10;
                yearDigits++;
            }
            wmove(deadlinewin, 1, 1);
            for(int i = 0; i < 4 - yearDigits; i++) wprintw(deadlinewin, "0");
            wprintw(deadlinewin, "%d/", tasks[selectedTask].deadlineYear);
            if (tasks[selectedTask].deadlineMonth / 10 == 0) wprintw(deadlinewin, "0");
            wprintw(deadlinewin, "%d/", tasks[selectedTask].deadlineMonth);
            if (tasks[selectedTask].deadlineDay / 10 == 0) wprintw(deadlinewin, "0");
            wprintw(deadlinewin, "%d", tasks[selectedTask].deadlineDay);
        }
    }
    wattroff(deadlinewin, COLOR_PAIR(1));
    wrefresh(deadlinewin);
}