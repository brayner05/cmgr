#ifndef CMGR_H
#define CMGR_H
#include <stdbool.h>
#include <stdint.h>
#include "error.h"

typedef struct {
    char *name;
    uint16_t option_index;
} cmgr_Option;

/**
 * Initialize all application components and
 * start the program.
 * @return Any error that occurred or `CMGR_ERR_OK` if no error occurred.
 */
extern cmgr_Error cmgr_begin(void);

extern void cmgr_end(void);

extern cmgr_Error cmgr_print_project_types(void);

extern cmgr_Error cmgr_set_cursor_position(int x, int y);

extern cmgr_Error cmgr_println(const char *line);

extern cmgr_Error cmgr_print_heading(const char *heading);

extern cmgr_Error cmgr_print_title(void);

#endif