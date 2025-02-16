#ifndef CMGR_H
#define CMGR_H
#include <stdbool.h>
#include <stdint.h>
#include "error.h"

/**
 * @file cmgr,h
 * 
 * This header file defines the interface for cmgr. It contains
 * all necessary code to initialize, run, and exit cmgr.
 */


enum {
    CMGR_MENU_LANGUAGE   = 0,   /**< A menu representing which programming language the user wants to use */
    CMGR_MENU_C_FILE     = 1,   /**< A menu representing possible actions for a C project */
    CMGR_MENU_CPP_FILE   = 2,   /**< A menu representing possible actions for a C++ project */
    CMGR_MENU_NULL              /**< A value representing the absence of a menu */
};

enum {
    CMGR_LANGID_C       = 0,    /**< Language id that specifies the C language */
    CMGR_LANGID_CPP     = 1     /**< Language id that specifies the C++ language */
};


enum {
    CMGR_TYPE_SOURCE,
    CMGR_TYPE_HEADER,
    CMGR_TYPE_MODULE,
    CMGR_TYPE_CLASS,
    CMGR_TYPE_STRUCT
};


/**
 * @typedef cmgr_OptionId
 * 
 * 
 * `cmgr_OptionId` is an alias for `size_t`, used for readabilty
 * when working with `cmgr_MenuOption`.
 * 
 * @see cmgr_MenuOption
 */
typedef size_t cmgr_OptionId;


/**
 * @struct cmgr_MenuOption
 * 
 * @brief Metadata about any menu option.
 * 
 */
typedef struct {
    const char *name;       /**< The name/title of the option (used to display in menu) */
    const cmgr_OptionId id; /**< The unique identifier of the option (used as the index in an option table) */
} cmgr_MenuOption;


/**
 * @struct cmgr_MenuOptionOrError
 * 
 * @brief A type encapsulating a `cmgr_Error` or a `cmgr_MenuOption`.
 * If `had_error` is set to true, then only `error` exists. Otherwise 
 * only `selection` exists.
 * 
 */
typedef struct cmgr_MenuResult {
    bool had_error;
    union {
        cmgr_MenuOption selection;
        cmgr_Error error;
    };
} cmgr_MenuOptionOrError;


/**
 * @typedef cmgr_MenuResult
 * 
 * A shorter alias for `cmgr_MenuOptionOrError`.
 * 
 */
typedef struct cmgr_MenuResult cmgr_MenuResult;


/**
 * @extern
 */
extern const uint16_t cmgr_DEFAULT_SELECTION;

/**
 * Initialize all application components and
 * start the program.
 * 
 * @return Any error that occurred or `CMGR_ERR_OK` if no error occurred.
 */
extern cmgr_Error cmgr_begin(void);

/**
 * Shuts down all cmgr code and all of cmgr's dependencies.
 */
extern void cmgr_end(void);


/**
 * Sets the position of the cursor in the terminal.
 * TODO: Consider making this function static because it is
 * implementation detail.
 */
extern cmgr_Error cmgr_set_cursor_position(int x, int y);


/**
 * Prints `str` at the current cursor position. Note that
 * the cursor position is not changed when calling `cmgr_print`.
 * 
 * @param str The string to print.
 * 
 * @return `CMGR_ERR_PRINTLN` if an error occurs (For example, if cmgr has not been initialized).
 * Otherwise, this function return `CMGR_ERR_OK`, signifying that no error occured.
 */
extern cmgr_Error cmgr_print(const char *str);

/**
 * Prints `line` followed by the new line character `'\\n'`/`0x0A`.
 * Similar to `cmgr_print` except that `cmgr_println`
 * moves the cursor down 1 line.
 * 
 * @param line the line to print
 * 
 * @return `CMGR_ERR_PRINTLN` on failure, `CMGR_ERR_OK` on success. For more
 * information, see `cmgr_print`.
 * 
 * @see cmgr_print
 */
extern cmgr_Error cmgr_println(const char *line);

extern cmgr_Error cmgr_print_heading(const char *heading);

extern cmgr_Error cmgr_print_title(void);

extern cmgr_Error cmgr_menu_prompt(uint16_t menu_id);


/**
 * Gets the id/key of the selected menu option for the menu
 * matching menu_id. This allows access to the results of menu 
 * selections even after the respective menu has been closed.
 * @param menu_id The id/key of the menu to check. If the menu has
 * not yet run, `cmgr_DEFAULT_SELECTION` will be returned.
 */
extern uint16_t cmgr_get_selection_key(uint16_t menu_id);

extern cmgr_MenuResult cmgr_get_selection_value(uint16_t menu_id);

extern void cmgr_reset_screen(void);

extern cmgr_Error cmgr_print_file_heading(const cmgr_MenuOption *file_type);

extern cmgr_Error cmgr_input_file_directory(void);

#endif