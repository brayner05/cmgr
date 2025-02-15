#include <stdio.h>
#include "cmgr.h"

int main(void) {
    cmgr_Error init_error = cmgr_begin();
    if (init_error != CMGR_ERR_OK)
        cmgr_handle_error(init_error);

    cmgr_print_title();
    cmgr_println("");
    
    cmgr_menu_prompt(CMGR_MENU_LANGUAGE);
    
    int ch;
    while ((ch = getchar()) != 0x0d) {
        
    }

    cmgr_end();
}