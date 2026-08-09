#include "../../include/types.h"

extern void *g_main_8011EE28_ArenaPtr; /* guess: current overlay-arena bump pointer / last-loaded-file pointer */

void f_main_80025AB4_SetArenaPtr(void *ptr) {
    g_main_8011EE28_ArenaPtr = ptr;
}
