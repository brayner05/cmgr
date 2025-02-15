#include <stdarg.h>
#include <stdio.h>
#include "ui.h"

static bool ui_active = false;

void cmgr_init_ui(void) {
    initscr();
    if (cmgr_ui_colour_enabled())
        start_color();

    curs_set(0);
    noecho();
    nodelay(stdscr, true);
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

bool cmgr_ui_colour_enabled(void) {
    return has_colors();
}

uint16_t cmgr_ui_readkey(void) {
    return (uint16_t) wgetch(stdscr);
}