#include <ncurses.h>
#include "../include/tui.h"
#include "../include/keys.h"
int main(){
    init_tui();
    key_input();
    endwin();
    return 0;
}