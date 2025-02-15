#include <stddef.h>
#include "cmgr.h"
#include "ui.h"

#define cmgr_assert(condition, error_type) do { if (!(condition)) return error_type; } while (0)

typedef struct {
    char *name;
    char **options;
    size_t option_count;
} Menu;

typedef char* cmgr_OptionsList[];
static bool initialized = false;
static struct { int x; int y; } cursor_position = { 0, 0 };


static const Menu menus[] = {
    [CMGR_MENU_LANGUAGE] = { 
        .name = "Choose Language", 
        .options = (cmgr_OptionsList) { "C", "C++" }, 
        .option_count = 2 
    },
    [CMGR_MENU_C_FILE] = { 
        .name = "C Management",
        .options = (cmgr_OptionsList) { "C Source File", "C Header File", "C Source & Header File" }, 
        .option_count = 3
    },
    [CMGR_MENU_CPP_FILE] = {
        .name = "C++ Management",
        .options = (cmgr_OptionsList) { 
            "C++ Source File", 
            "C++ Header File", 
            "C++ Source & Header File", 
            "C++ Class", 
            "C++ Struct" 
        },
        .option_count = 5
    }
};

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
    cmgr_assert(initialized, CMGR_ERR_CURSOR);
    cursor_position.x = x;
    cursor_position.y = y;
    return CMGR_ERR_OK;
}


cmgr_Error cmgr_menu_prompt(uint16_t menu_id) {
    cmgr_assert(menu_id < sizeof(menus) / sizeof(Menu), CMGR_ERR_MENU_ID);
    const Menu *menu = &menus[menu_id];

    cmgr_print_heading(menu->name);
    for (size_t i = 0; i < menu->option_count; ++i) {
        cmgr_println(menu->options[i]);
    }

    return CMGR_ERR_OK;
}


cmgr_Error cmgr_print_heading(const char *heading) {
    cmgr_ui_set_underlined(true);
    cmgr_println(heading);
    cmgr_ui_set_underlined(false);
    return CMGR_ERR_OK;
}


cmgr_Error cmgr_println(const char *line) {
    cmgr_assert(initialized, CMGR_ERR_PRINTLN);
    cmgr_ui_write_at(cursor_position.x, cursor_position.y, line);
    cmgr_set_cursor_position(cursor_position.x, cursor_position.y + 1);
    cmgr_ui_refresh();
    return CMGR_ERR_OK;
}


cmgr_Error cmgr_print_title(void) {
    cmgr_Error error = CMGR_ERR_OK;
    error |= cmgr_println("  ___ _ __ ___   __ _ _ __  ");
    error |= cmgr_println(" / __| '_ ` _ \\ / _` | '__|");
    error |= cmgr_println("| (__| | | | | | (_| | |   ");
    error |= cmgr_println(" \\___|_| |_| |_|\\__, |_|  ");
    error |= cmgr_println("                |___/      ");
    return error;
}
