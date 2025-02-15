#ifndef CMGR_H
#define CMGR_H
#include <stdbool.h>
#include <stdint.h>
#include "error.h"

enum {
    CMGR_MENU_LANGUAGE   = 0,
    CMGR_MENU_C_FILE     = 1,
    CMGR_MENU_CPP_FILE   = 2
};

extern const uint16_t cmgr_DEFAULT_SELECTION;

/**
 * Initialize all application components and
 * start the program.
 * @return Any error that occurred or `CMGR_ERR_OK` if no error occurred.
 */
extern cmgr_Error cmgr_begin(void);

extern void cmgr_end(void);

extern cmgr_Error cmgr_set_cursor_position(int x, int y);

extern cmgr_Error cmgr_println(const char *line);

extern cmgr_Error cmgr_print_heading(const char *heading);

extern cmgr_Error cmgr_print_title(void);

extern cmgr_Error cmgr_menu_prompt(uint16_t menu_id);


/**
 * Gets the id/key of the selected menu option for the menu
 * matching menu_id. This allows access to the results of menu 
 * selections even after the respective menu has been closed.
 * @param menu_id The id/key of the menu to check. If the menu has
 * not yet run, `cmgr_DEFAULT_SELECTION` will be returned.
 */
extern uint16_t cmgr_get_selection_key(uint16_t menu_id);

extern char *cmgr_get_selection_value(uint16_t menu_id);

#endif