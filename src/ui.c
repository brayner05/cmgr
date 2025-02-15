#include <ncurses.h>
#include "ui.h"

static bool ui_active = false;

void cmgr_init_ui(void) {
    initscr();
    curs_set(0);
    ui_active = true;
}

void cmgr_destroy_ui(void) {
    endwin();
    ui_active = false;
}

bool cmgr_ui_active(void) {
    return ui_active;
}