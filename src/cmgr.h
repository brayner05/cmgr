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

#endif