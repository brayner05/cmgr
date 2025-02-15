#include <ncurses.h>
#include "ui.h"

static bool ui_active = false;

void cmgr_init_ui(void) {
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, true);
    ui_active = true;
}

void cmgr_destroy_ui(void) {
    endwin();
    ui_active = false;
}

bool cmgr_ui_active(void) {
    return ui_active;
}

void cmgr_ui_write_at(int x, int y, const char *src) {
    mvprintw(y, x, src);
}

void cmgr_ui_refresh(void) {
    refresh();
}

void cmgr_ui_clear(void) {
    clear();
}

void cmgr_ui_set_underlined(bool underlined) {
    if (underlined) {
        attron(A_UNDERLINE);
        return;
    }
    attroff(A_UNDERLINE);
}