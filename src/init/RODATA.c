#include "../../include/types.h"

/* Guessed-structure read-only data for the init overlay. Bare const-char
   string literals stay inlined at call sites; only rodata whose *structure*
   has been guessed (tables, struct arrays, etc.) gets defined here, typed
   from include/types.h and referenced via extern from the decompiled .c
   files that use it. Populated gradually as encountered. */
