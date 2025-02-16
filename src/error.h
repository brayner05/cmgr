#ifndef CMGR_ERROR_H
#define CMGR_ERROR_H
#include <stdbool.h>

#define cmgr_assert(condition, error_type) do { if (!(condition)) return error_type; } while (0)

typedef enum {
    CMGR_ERR_OK                 = (0x00),
    CMGR_ERR_NCURSES_INIT       = (0x01 << 0),
    CMGR_ERR_INIT               = (0x01 << 1),
    CMGR_ERR_CURSOR             = (0x01 << 2),
    CMGR_ERR_PRINTLN            = (0x01 << 3),
    CMGR_ERR_MENU_ID            = (0x01 << 4),
    CMGR_ERR_INVALID_KEY        = (0x01 << 5),
    CMGR_ERR_COLOUR_DISABLED    = (0x01 << 6)
} cmgr_Error;

/**
 * 
 * Deprecated - DO NOT USE: Was initially used as a method of handling errors that 
 * occur at runtime, but proved to not be very helpful as every new error 
 * required a new condition to be passed to `cmgr_handle_error`.
 * 
 * @deprecated
 */
extern void cmgr_handle_error(cmgr_Error error) __attribute__((deprecated));

extern bool cmgr_error_is_type(cmgr_Error error, cmgr_Error check);

#endif