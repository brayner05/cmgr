#include <stdio.h>
#include <string.h>
#include "cmgr.h"

int main(void) {
    cmgr_Error init_error = cmgr_begin();
    if (init_error != CMGR_ERR_OK)
        cmgr_handle_error(init_error);

    cmgr_print_title();
    cmgr_println("");
    
    cmgr_menu_prompt(CMGR_MENU_LANGUAGE);
    cmgr_MenuOption language = cmgr_get_selection_value(CMGR_MENU_LANGUAGE);

    cmgr_end();

    printf("ID: %lu, Name: %s\n", language.id, language.name);
    getchar();
}