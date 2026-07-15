#ifndef TUI_H
#define TUI_H

#include <ncurses.h>

extern void init_tui(void);
extern void draw_tasks(void);
extern void draw_description(void);
extern void draw_categories(void);
extern void draw_subtasks(void);
extern void draw_deadline(void);

#endif