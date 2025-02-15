#include <stdio.h>
#include "cmgr.h"

int main(void) {
    cmgr_Error init_error = cmgr_begin();
    if (init_error != CMGR_ERR_OK)
        cmgr_handle_error(init_error);

    cmgr_print_title();
    cmgr_println("");
    cmgr_print_heading("project type");
    cmgr_print_project_types();
    
    int ch;
    while ((ch = getchar()) != 0x0d) {
        
    }

    cmgr_end();
}