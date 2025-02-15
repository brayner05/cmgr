#include <stdarg.h>
#include <ncurses.h>
#include "ui.h"

static bool ui_active = false;

void cmgr_init_ui(void) {
    initscr();
    curs_set(1);
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
    cmgr_ui_refresh();
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

void cmgr_ui_printf(int x, int y, const char *fmt, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    move(y, x);
    vw_printw(stdscr, fmt, arg_ptr);
    va_end(arg_ptr);
    cmgr_ui_refresh();
}