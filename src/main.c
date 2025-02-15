#include <stdio.h>
#include "cmgr.h"

int main(void) {
    cmgr_Error init_error = cmgr_begin();
    if (init_error != CMGR_ERR_OK)
        cmgr_handle_error(init_error);

    cmgr_print_title();
    cmgr_println("");
    
    cmgr_menu_prompt(CMGR_MENU_LANGUAGE);
    cmgr_end();

    uint16_t id = cmgr_get_selection_key(CMGR_MENU_LANGUAGE);
    char *language = cmgr_get_selection_value(CMGR_MENU_LANGUAGE);
    printf("Current selection: %u, %s\n", id, language);

    getchar();
}