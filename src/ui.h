#ifndef CMGR_UI_H
#define CMGR_UI_H
#include <stdbool.h>
#include <ncurses.h>
#include "error.h"

typedef enum {
    CMGR_KEY_UP     = KEY_UP,
    CMGR_KEY_LEFT   = KEY_LEFT,
    CMGR_KEY_DOWN   = KEY_DOWN,
    CMGR_KEY_RIGHT  = KEY_RIGHT,
    CMGR_KEY_ENTER  = KEY_ENTER
} cmgr_Key;

typedef uint8_t cmgr_Colour;

typedef uint16_t cmgr_ColourId;

extern void cmgr_init_ui(void);

extern void cmgr_destroy_ui(void);

extern bool cmgr_ui_active(void);

extern void cmgr_ui_write_at(int x, int y, const char *src);

extern void cmgr_ui_refresh(void);

extern void cmgr_ui_clear(void);

extern void cmgr_ui_set_underlined(bool underlined);

extern void cmgr_ui_printf(int x, int y, const char *fmt, ...);

extern bool cmgr_ui_colour_enabled(void);

extern cmgr_Error cmgr_ui_start_select(void);

extern cmgr_Error cmgr_ui_end_select(void);

extern uint16_t cmgr_ui_readkey(void);

#endif