#ifndef CMGR_UI_H
#define CMGR_UI_H
#include <stdbool.h>
#include "error.h"

extern void cmgr_init_ui(void);

extern void cmgr_destroy_ui(void);

extern bool cmgr_ui_active(void);

extern void cmgr_ui_write_at(int x, int y, const char *src);

extern void cmgr_ui_refresh(void);

extern void cmgr_ui_clear(void);

extern void cmgr_ui_set_underlined(bool underlined);

#endif