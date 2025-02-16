#include <stdarg.h>
#include <stdio.h>
#include <locale.h>
#include "ui.h"

static bool ui_active = false;
static const cmgr_ColourId cmgr_SELECTION = 1;

void cmgr_init_ui(void) {
    setlocale(LC_ALL, "");
    initscr();
    if (cmgr_ui_colour_enabled()) {
        start_color();
        init_pair(cmgr_SELECTION, COLOR_CYAN, COLOR_BLACK);
    }

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

cmgr_Error cmgr_ui_start_select(void) {
    cmgr_assert(cmgr_ui_colour_enabled(), CMGR_ERR_COLOUR_DISABLED);
    attron(COLOR_PAIR(cmgr_SELECTION));
    return CMGR_ERR_OK;
}

cmgr_Error cmgr_ui_end_select(void) {
    cmgr_assert(cmgr_ui_colour_enabled(), CMGR_ERR_COLOUR_DISABLED);
    attroff(COLOR_PAIR(cmgr_SELECTION));
    return CMGR_ERR_OK;
}