#include <stdio.h>
#include "error.h"

void cmgr_handle_error(cmgr_Error error) {
    if (error == CMGR_ERR_OK) {
        fputs("No error has occurred.", stderr);
        return;
    }

    if (cmgr_error_is_type(error, CMGR_ERR_INIT))
        fputs("Failed to initialize cmgr.", stderr);
    
    if (cmgr_error_is_type(error, CMGR_ERR_NCURSES_INIT))
        fputs("Failed to initialize ncurses.", stderr);
}

bool cmgr_error_is_type(cmgr_Error error, cmgr_Error check) {
    return (error & check) != 0;
}