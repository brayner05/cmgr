#include <stddef.h>
#include "cmgr.h"
#include "ui.h"

#define cmgr_assert(condition, error_type) do { if (!(condition)) return error_type; } while (0)
#define cmgr_count_options(table) (sizeof(table) / sizeof(cmgr_MenuOption))

typedef struct {
    const char *name;
    const cmgr_MenuOption *options;
    size_t option_count;
    uint16_t selected_option;
} cmgr_Menu;


const uint16_t cmgr_DEFAULT_SELECTION = 0;
static bool initialized = false;
static struct { int x; int y; } cursor_position = { 0, 0 };


static const cmgr_MenuOption languages[] = {
    [CMGR_LANGID_C] = { .id = CMGR_LANGID_C, .name = "C" },
    [CMGR_LANGID_CPP] = { .id = CMGR_LANGID_CPP, .name = "C++" }
};

static const cmgr_MenuOption c_file_options[] = { 
    [CMGR_TYPE_SOURCE]  = { .id = CMGR_TYPE_SOURCE, .name = "C Source File (.c)" },
    [CMGR_TYPE_HEADER]  = { .id = CMGR_TYPE_HEADER, .name = "C Header File (.h)" },
    [CMGR_TYPE_MODULE]  = { .id = CMGR_TYPE_MODULE, .name = "C Source & Header File" },
};

static const cmgr_MenuOption cpp_file_options[] = {
    [CMGR_TYPE_SOURCE]  = { .id = CMGR_TYPE_SOURCE, .name = "Source File (.cpp, .cxx, .c++)" },
    [CMGR_TYPE_HEADER]  = { .id = CMGR_TYPE_HEADER, .name = "Header File (.hpp, .h, .hxx)" },
    [CMGR_TYPE_MODULE]  = { .id = CMGR_TYPE_MODULE, .name = "Source & Header File" },
    [CMGR_TYPE_CLASS]   = { .id = CMGR_TYPE_CLASS,  .name = "C++ Class" },
    [CMGR_TYPE_STRUCT]  = { .id = CMGR_TYPE_STRUCT, .name = "C++ Struct" }
};


static cmgr_Menu menus[] = {
    [CMGR_MENU_LANGUAGE] = { 
        .name = "Choose Language", 
        .options = languages, 
        .option_count = cmgr_count_options(languages),
        .selected_option = cmgr_DEFAULT_SELECTION
    },

    [CMGR_MENU_C_FILE] = { 
        .name = "C Management",
        .options = c_file_options, 
        .option_count = cmgr_count_options(c_file_options),
        .selected_option = cmgr_DEFAULT_SELECTION
    },

    [CMGR_MENU_CPP_FILE] = {
        .name = "C++ Management",
        .options = cpp_file_options,
        .option_count = cmgr_count_options(cpp_file_options),
        .selected_option = cmgr_DEFAULT_SELECTION
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

static void print_menu_options(const cmgr_Menu *menu) {
    for (size_t i = 0; i < menu->option_count; ++i) {
        const char *format_str = NULL;

        if (i == menu->selected_option)
            format_str = "> %s\n";
        else
            format_str = "  %s\n";
        
        if (cmgr_ui_colour_enabled()) {}

        cmgr_ui_printf(
            cursor_position.x, cursor_position.y,
            format_str, menu->options[i]);

        cmgr_set_cursor_position(cursor_position.x, cursor_position.y + 1);
    }
}

cmgr_Error cmgr_menu_prompt(uint16_t menu_id) {
    cmgr_assert(menu_id < sizeof(menus) / sizeof(cmgr_Menu), CMGR_ERR_MENU_ID);
    cmgr_Menu *menu = &menus[menu_id];

    cmgr_print_heading(menu->name);
    print_menu_options(menu);

    uint16_t key_code;
    while ((key_code = cmgr_ui_readkey()) != CMGR_KEY_ENTER && key_code != '\n') {
        cmgr_set_cursor_position(0, 0);
        cmgr_print_title();
        cmgr_println("");
        cmgr_print_heading(menu->name);
        print_menu_options(menu);

        switch (key_code) {
            case CMGR_KEY_UP: 
                menu->selected_option = ((signed) menu->selected_option - 1) % menu->option_count; 
                break;

            case CMGR_KEY_DOWN:
                menu->selected_option = ((signed) menu->selected_option + 1) % menu->option_count; 
                break;

            default: break;
        }

        napms(16);
    }

    cmgr_ui_clear();
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

uint16_t cmgr_get_selection_key(uint16_t menu_id) {
    return menus[menu_id].selected_option;
}

cmgr_MenuOption cmgr_get_selection_value(uint16_t menu_id) {
    // cmgr_assert(menu_id < CMGR_MENU_NULL, CMGR_ERR_INVALID_KEY);
    const cmgr_Menu *menu = &menus[menu_id];
    const uint16_t selection_key = menu->selected_option;
    return menu->options[selection_key];
}