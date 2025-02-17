#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "cmgr.h"
#include "ui.h"

#define cmgr_count_options(table) (sizeof(table) / sizeof(cmgr_MenuOption))

#define cmgr_count_menus(table) (sizeof(table) / sizeof(cmgr_Menu))

#define cursor_goto(x, y) cmgr_set_cursor_position((x), (y))

#define cmgr_min(a, b) ((a) < (b) ? (a) : (b))

#define cmgr_get_terminal_width() getmaxx(stdscr)


const uint16_t cmgr_DEFAULT_SELECTION = 0;

static bool initialized = false;

static struct { int x; int y; } cursor_position = { 0, 0 };


typedef struct cmgr_PromptNode {
    cmgr_Menu *menu;
    struct cmgr_PromptNode *next;
    struct cmgr_PromptNode *previous;
} cmgr_PromptNode;


typedef struct {
    cmgr_PromptNode *head;
    cmgr_PromptNode *tail;
    size_t length;
} cmgr_PromptList;


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
        .selection_index = 0
    },

    [CMGR_MENU_C_FILE] = { 
        .name = "C Management",
        .options = c_file_options, 
        .option_count = cmgr_count_options(c_file_options),
        .selection_index = 0
    },

    [CMGR_MENU_CPP_FILE] = {
        .name = "C++ Management",
        .options = cpp_file_options,
        .option_count = cmgr_count_options(cpp_file_options),
        .selection_index = 0
    }
};


static cmgr_PromptList prompt_list = { 0 };

static cmgr_PromptNode *next_prompt = { 0 };

static cmgr_PromptNode *previous_prompt = { 0 };


static inline bool prompt_list_empty(void) {
    return prompt_list.head == NULL;
}


cmgr_Error cmgr_add_prompt(unsigned int key) {
    if (key >= cmgr_count_menus(menus))
        return CMGR_ERR_MENU_ID;

    cmgr_Menu *prompt = &menus[key];
    cmgr_PromptNode *prompt_node = (cmgr_PromptNode *) calloc(1, sizeof(cmgr_PromptNode));
    if (prompt_node == NULL)
        return CMGR_ERR_MEMORY;

    prompt_node->menu = prompt;
    prompt_node->next = NULL;
    prompt_node->previous = NULL;

    if (prompt_list_empty()) {
        prompt_list.head = prompt_list.tail = prompt_node;
        prompt_list.length++;
        next_prompt = prompt_list.head;
        return CMGR_ERR_OK;
    }

    if (next_prompt == NULL)
        next_prompt = prompt_node;

    prompt_list.tail->next = prompt_node;
    prompt_node->previous = prompt_list.tail;
    prompt_list.tail = prompt_node;
    prompt_list.length++;

    return CMGR_ERR_OK;
}


static cmgr_MenuResult cmgr_get_prompt_selection(unsigned int key) {
    cmgr_MenuResult result = { .had_error = true, .error = CMGR_ERR_INVALID_KEY };

    if (key >= CMGR_MENU_NULL)
        return result;

    const cmgr_Menu *current_menu = &menus[key];

    result.had_error = false;
    result.selection = &current_menu->options[current_menu->selection_index];

    return result;
}


static cmgr_Error init_prompt_list(void) {
    prompt_list = (cmgr_PromptList) { 0 };
    return CMGR_ERR_OK;
}


cmgr_Error cmgr_begin(void) {
    cmgr_Error errors = CMGR_ERR_OK;

    if (cmgr_ui_active())
        errors |= CMGR_ERR_NCURSES_INIT;

    cmgr_init_ui();
    if (errors == CMGR_ERR_OK)
        initialized = true;

    errors |= init_prompt_list();

    return errors;
}


void cmgr_end(void) {
    if (cmgr_ui_active())
        cmgr_destroy_ui();
}


void cmgr_reset_screen(void) {
    cmgr_ui_clear();
    cmgr_set_cursor_position(0, 0);
    cmgr_print_title();
    cmgr_println("");
}


cmgr_Error cmgr_set_cursor_position(int x, int y) {
    cmgr_assert(initialized, CMGR_ERR_CURSOR);
    cursor_position.x = x;
    cursor_position.y = y;
    move(y, x);
    return CMGR_ERR_OK;
}


static void print_menu_options(const cmgr_Menu *menu) {
    for (size_t i = 0; i < menu->option_count; ++i) {
        if (i == (size_t) menu->selection_index) {
            cmgr_ui_start_select();
            cmgr_ui_printf(
                cursor_position.x, cursor_position.y,
                "👉 %s\n", menu->options[i]);
            cmgr_ui_end_select();
        } else {
            cmgr_ui_printf(
                cursor_position.x, cursor_position.y,
                "   %s\n", menu->options[i]);
        }
        
        cmgr_set_cursor_position(cursor_position.x, cursor_position.y + 1);
    }
}


cmgr_Error cmgr_menu_prompt(cmgr_Menu *menu) {
    cmgr_reset_screen();

    uint16_t key_code;
    while ((key_code = cmgr_ui_readkey()) != CMGR_KEY_ENTER && key_code != '\n') {
        cmgr_set_cursor_position(0, 0);
        cmgr_print_title();
        cmgr_println("");
        cmgr_print_heading(menu->name);
        cmgr_println("");
        print_menu_options(menu);

        switch (key_code) {
            case CMGR_KEY_UP: 
                menu->selection_index = ((signed) menu->selection_index - 1) % menu->option_count; 
                break;

            case CMGR_KEY_DOWN:
                menu->selection_index = ((signed) menu->selection_index + 1) % menu->option_count; 
                break;

            default: break;
        }

        napms(16);
    }

    return CMGR_ERR_OK;
}


cmgr_Error cmgr_next_prompt(void) {
    if (next_prompt == NULL)
        return CMGR_ERR_MENU_ID;

    cmgr_menu_prompt(next_prompt->menu);
    previous_prompt = next_prompt;
    next_prompt = next_prompt->next;

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
    cursor_goto(0, cursor_position.y + 1);
    return CMGR_ERR_OK;
}


cmgr_Error cmgr_print(const char *str) {
    cmgr_assert(initialized, CMGR_ERR_PRINTLN);
    cmgr_ui_write_at(cursor_position.x, cursor_position.y, str);
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

static inline cmgr_MenuResult cmgr_error_result(cmgr_Error error_type) {
    cmgr_MenuResult error_result = { .had_error = true, .error = error_type };
    return error_result;
}

cmgr_MenuResult cmgr_get_last_prompt_result(void) {
    if (previous_prompt == NULL)
        return cmgr_error_result(CMGR_ERR_MENU_ID);
    
    const cmgr_Menu *prompt = previous_prompt->menu;
    const cmgr_MenuOption *selection = &prompt->options[prompt->selection_index];
    cmgr_MenuResult result = { .had_error = false, .selection = selection };

    return result;
}


cmgr_MenuResult cmgr_get_menu_selection(uint16_t menu_id) {
    return cmgr_get_prompt_selection((unsigned int) menu_id);
}


cmgr_Error cmgr_print_file_heading(const cmgr_MenuOption *heading) {
    cmgr_MenuResult result = cmgr_get_prompt_selection(CMGR_MENU_LANGUAGE);
    if (result.had_error)
        return result.error;

    const cmgr_MenuOption *language = result.selection;

    cmgr_ui_set_underlined(true);
    cmgr_ui_printf(cursor_position.x, cursor_position.y, "%s > %s", language->name, heading->name);
    cursor_goto(0, cursor_position.y + 1);
    cmgr_println("");
    cmgr_ui_set_underlined(false);

    return CMGR_ERR_OK;
}


static void cmgr_show_input(char *input_value, const size_t value_length, const int caret_position) {
    attron(A_REVERSE);
    for (size_t i = 0; i < value_length; ++i) {
        if (i == (size_t) caret_position) {
            attron(A_BLINK);
        }

        char next_char = input_value[i];
        if (next_char == '\0')
            mvaddch(cursor_position.y, cursor_position.x, ' ');
        else
            mvaddch(cursor_position.y, cursor_position.x, next_char);

        cursor_goto(cursor_position.x + 1, cursor_position.y);
        attroff(A_BLINK);
    }

    attroff(A_REVERSE);
    cmgr_ui_refresh();
}


static bool cmgr_is_valid_path_char(uint16_t key_code) {
    return (
        isalnum(key_code) || isspace(key_code) || 
        (key_code == '/') || 
        (key_code == '.') || 
        (key_code == '~') || 
        (key_code == '_') ||
        (key_code == '-')
    );
}


/**
 * @note This function could likely be refactored into a few
 * other functions, which would be much easier to read.
 */
cmgr_Error cmgr_input_file_directory(void) {
    static const int padding = 1;

    // The width of the input box itself
    const size_t preferred_input_width = (size_t) (1.5 * cmgr_get_terminal_width() / 2);
    const size_t input_width = cmgr_min(preferred_input_width, (size_t) cmgr_get_terminal_width());

    // The size of the buffer to allocate for the output directory
    const size_t input_buffer_length = input_width + 1;

    // The directory buffer 
    char *output_directory = (char *) calloc(input_buffer_length, sizeof(char));

    // By default set the output directory to the current working directory
    char *cwd = getcwd(NULL, 0);
    size_t cwd_length = strlen(cwd);
    strncpy(output_directory, cwd, cwd_length);

    // Calculate the initial position of the caret.
    // That is, the last character of the output directory.
    size_t caret_position = caret_position = cmgr_min(strlen(output_directory) - 1, SIZE_MAX);


    // Get the result of the last prompt
    const cmgr_MenuResult last_result = cmgr_get_last_prompt_result();
    if (last_result.had_error)
        return last_result.error;

    const cmgr_MenuOption *heading = last_result.selection;

    // Clear the screen, print the title, and then the language and file type
    cmgr_reset_screen();
    cmgr_print_file_heading(heading);
    
    cmgr_println("Where would you like to create the file(s)?");
    cursor_goto(cursor_position.x, cursor_position.y + padding);

    // Read the output directory from the user via the input box.
    uint16_t key_code;
    while ((key_code = cmgr_ui_readkey()) != KEY_ENTER && key_code != '\n') {
        cursor_goto(0, cursor_position.y);
        clrtoeol();
        cmgr_show_input(output_directory, input_buffer_length, caret_position);

        // Remove a character whn backspace is pressed
        if (key_code == KEY_BACKSPACE) {
            if (caret_position > 0) {
                output_directory[caret_position--] = '\0';
            }
            continue;
        }

        // If the key pressed is not a valid path character, don't add it to the buffer
        if (!cmgr_is_valid_path_char(key_code))
            continue;


        // Store a valid key character in the buffer at the current caret position.
        output_directory[caret_position] = (char) key_code;

        // Move the caret 1 character forward, unless it is already at the end of the input
        caret_position = cmgr_min(caret_position + 1, input_width);

        napms(16);
    }

    // Release the input buffer
    free(output_directory);

    return CMGR_ERR_OK;
}
