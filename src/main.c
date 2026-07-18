#include <ncurses.h>
#include "../include/tui.h"
#include "../include/keys.h"
#include "../include/globals.h"
int main(){
    init_tui();
    whereWeAre = winTask;
    key_input();
    endwin();
    return 0;
}