#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include "cmgr.h"

int main(void) {
    // Initialize cmgr
    cmgr_Error init_error = cmgr_begin();
    if (init_error != CMGR_ERR_OK)
        cmgr_handle_error(init_error);

    // Prompt the user for a programming language
    cmgr_menu_prompt(CMGR_MENU_LANGUAGE);
    cmgr_MenuOption language = cmgr_get_selection_value(CMGR_MENU_LANGUAGE);
    
    // The key for the next prompt. Default to prompting for a C file
    uint16_t next_menu_id = CMGR_MENU_C_FILE;

    // If the user chose C++, prompt for a C++ file instead
    if (language.id == CMGR_LANGID_CPP)
        next_menu_id = CMGR_MENU_CPP_FILE;

    // Get the project type
    cmgr_menu_prompt(next_menu_id);
    cmgr_MenuOption project_type = cmgr_get_selection_value(next_menu_id);
    
    cmgr_print_file_heading(&project_type);
    cmgr_input_file_directory();


    // Todo: prompt for file location but first recommend one
    // Todo: Generate header guards

    getchar();
    cmgr_end();
}