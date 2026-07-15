#include <ncurses.h>
#include "../include/globals.h"

void init_tui(){
    initscr();
    if (!has_colors()) {
        endwin();
        fprintf(stderr, "Your terminal doesn't support colors!");
        exit(EXIT_FAILURE);
    }
    init_todo_color();
    int xmax, ymax;
    getmaxyx(stdscr, ymax, xmax);
    taskswin = newwin(ymax/2 + ymax/4, xmax/2, 0, 0);
    descriptionwin = newwin(ymax/2, xmax/2 , 0, xmax/2 );
    categorieswin = newwin(ymax/4, xmax/2 , ymax/2 , xmax/2 );
    subtaskswin = newwin(ymax/4, xmax/2, ymax/2 + ymax/4 , 0);
    deadlinewin = newwin(ymax/4, xmax/2 , ymax/2 + ymax/4 , xmax/2 );
    wattron(taskswin, COLOR_PAIR(1));
    refresh();
    box(taskswin,0,0);
    mvwprintw(taskswin, 0, 1, "Tasks");
    wattroff(taskswin, COLOR_PAIR(1));
    wattron(descriptionwin, COLOR_PAIR(1));
    box(descriptionwin,0,0);
    mvwprintw(descriptionwin, 0, 1, "Description");
    wattroff(descriptionwin, COLOR_PAIR(1));
    wattron(categorieswin, COLOR_PAIR(1));
    box(categorieswin,0,0);
    mvwprintw(categorieswin, 0, 1, "Categories");
    wattroff(categorieswin, COLOR_PAIR(1));
    wattron(subtaskswin, COLOR_PAIR(1));
    box(subtaskswin,0,0);
    mvwprintw(subtaskswin, 0, 1, "Sub-Tasks");
    wattroff(subtaskswin, COLOR_PAIR(1));
    wattron(deadlinewin, COLOR_PAIR(1));
    box(deadlinewin,0,0);
    mvwprintw(deadlinewin, 0, 1, "Deadline");
    wattroff(deadlinewin,COLOR_PAIR(1));
    wrefresh(taskswin);
    wrefresh(descriptionwin);
    wrefresh(categorieswin);
    wrefresh(subtaskswin);
    wrefresh(deadlinewin);
}