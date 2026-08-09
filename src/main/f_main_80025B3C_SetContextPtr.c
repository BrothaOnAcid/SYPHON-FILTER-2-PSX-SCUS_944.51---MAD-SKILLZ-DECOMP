#include "../../include/types.h"

/* guess: generic "current context/handle" pointer, set before file-load
   helpers run and read back by callers of func_80026234/func_8002662C */
extern void *g_main_8011EE2C_ContextPtr;

void f_main_80025B3C_SetContextPtr(void *ptr) {
    g_main_8011EE2C_ContextPtr = ptr;
}
