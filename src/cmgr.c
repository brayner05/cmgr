#include <stddef.h>
#include "cmgr.h"
#include "ui.h"

static struct { int x; int y; } cursor_position = { 0, 0 };

static bool initialized = false;

static const char *project_types[] = {
    "C",
    "C++"
};

// static const char *c_options[] = {
//     "Source File",
//     "Source & Header File"
// };

// static const char *cpp_options[] = {
//     "C++ Class",
//     "C++ Struct"
// };

cmgr_Error cmgr_begin(void) {
    cmgr_Error errors = CMGR_ERR_OK;

    if (cmgr_ui_active())
        errors |= CMGR_ERR_NCURSES_INIT;

    cmgr_init_ui();
    if (errors == CMGR_ERR_OK)
        initialized = true;

    return errors;
}


void cmgr_end(void) {
    if (cmgr_ui_active())
        cmgr_destroy_ui();
}

cmgr_Error cmgr_set_cursor_position(int x, int y) {
    if (!initialized)
        return CMGR_ERR_CURSOR;

    cursor_position.x = x;
    cursor_position.y = y;

    return CMGR_ERR_OK;
}

cmgr_Error cmgr_print_project_types(void) {
    if (!initialized)
        return CMGR_ERR_PRINTLN;

    for (size_t i = 0; i < 2; ++i) {
        cmgr_println(project_types[i]);
        cmgr_set_cursor_position(cursor_position.x, cursor_position.y);
    }

    cmgr_ui_refresh();
    return CMGR_ERR_OK;
}

cmgr_Error cmgr_print_heading(const char *heading) {
    cmgr_ui_set_underlined(true);
    cmgr_println(heading);
    cmgr_ui_set_underlined(false);
    return CMGR_ERR_OK;
}

cmgr_Error cmgr_println(const char *line) {
    if (!initialized)
        return CMGR_ERR_PRINTLN;

    cmgr_ui_write_at(cursor_position.x, cursor_position.y, line);
    cmgr_set_cursor_position(cursor_position.x, cursor_position.y + 1);
    cmgr_ui_refresh();

    return CMGR_ERR_OK;
}