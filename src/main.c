#include <stdio.h>
#include <wchar.h>
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
    
    uint16_t next_menu_id = CMGR_MENU_C_FILE;

    if (language.id == CMGR_LANGID_CPP)
        next_menu_id = CMGR_MENU_CPP_FILE;

    cmgr_menu_prompt(next_menu_id);

    getchar();
    cmgr_end();
}