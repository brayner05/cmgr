#ifndef CMGR_ERROR_H
#define CMGR_ERROR_H
#include <stdbool.h>

typedef enum {
    CMGR_ERR_OK             = (0x00),
    CMGR_ERR_NCURSES_INIT   = (0x01 << 0),
    CMGR_ERR_INIT           = (0x01 << 1),
    CMGR_ERR_CURSOR         = (0x01 << 2),
    CMGR_ERR_PRINTLN        = (0x01 << 3),
    CMGR_ERR_MENU_ID        = (0x01 << 4)
} cmgr_Error;

extern void cmgr_handle_error(cmgr_Error error);

extern bool cmgr_error_is_type(cmgr_Error error, cmgr_Error check);

#endif