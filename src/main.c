#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include "cmgr.h"


int main(void) {
    static cmgr_Error error = CMGR_ERR_OK;

    // Initialize cmgr
    cmgr_Error init_error = cmgr_begin();
    if (init_error != CMGR_ERR_OK) {
        error = CMGR_ERR_INIT;
        goto cleanup;
    }

    // Set the first prompt
    cmgr_add_prompt(CMGR_MENU_LANGUAGE);
    cmgr_next_prompt();

    // Prompt the user for a programming language
    cmgr_MenuResult language_result = cmgr_get_last_prompt_result();
    if (language_result.had_error) {
        error = language_result.error;
        goto cleanup;
    } 

    const cmgr_MenuOption *language = language_result.selection;

    // The key for the next prompt. Default to prompting for a C file
    if (language->id == CMGR_LANGID_CPP) {
        cmgr_add_prompt(CMGR_MENU_CPP_FILE);
    }
    else
        cmgr_add_prompt(CMGR_MENU_C_FILE);    

    cmgr_next_prompt();

    cmgr_MenuResult file_type_result = cmgr_get_last_prompt_result();
    if (file_type_result.had_error) {
        error = file_type_result.error;
        goto cleanup;
    }


    // Get the project type
    cmgr_MenuResult project_type_result = cmgr_get_last_prompt_result();
    if (project_type_result.had_error) {
        error = project_type_result.error;
        goto cleanup;
    }

    const cmgr_MenuOption *project_type = project_type_result.selection;
    puts(project_type->name);
    
    cmgr_print_file_heading(project_type);
    // cmgr_input_file_directory();

    // Todo: prompt for file location but first recommend one
    // Todo: Generate header guards
    getchar();

cleanup:
    cmgr_end();
    return (int) error;
}