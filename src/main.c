#include <ncurses.h>
#include "../include/tui.h"
int main(){
    init_tui();
    getch();
    endwin();
    return 0;
}