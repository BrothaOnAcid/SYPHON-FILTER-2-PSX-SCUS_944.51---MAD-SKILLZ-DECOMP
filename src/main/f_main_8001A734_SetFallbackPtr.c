#include "../../include/types.h"

/* Guess: fallback data pointer used by the sprite/anim renderer. Set here from
   a resource lookup (f_main_80026E00_FindResource result, see the 0x8015C070
   call site) and read at 0x8001A354 when an object's own anim pointer is
   unavailable. */
extern void *g_main_8011ED78_FallbackPtr;

void f_main_8001A734_SetFallbackPtr(void *ptr) {
    g_main_8011ED78_FallbackPtr = ptr;
}
