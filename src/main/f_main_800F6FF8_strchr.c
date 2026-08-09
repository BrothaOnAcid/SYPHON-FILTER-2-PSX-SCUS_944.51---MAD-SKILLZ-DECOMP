#include "../../include/types.h"

/* PSX BIOS call trampoline (function 0x1F in the A0 table), same pattern as
   f_main_800F8248_strcmp.c. Behavior confirmed by call site
   (f_main_8002B3D8_GetResidentFile): searches for a '\\' to skip a path
   prefix, and for a ';' to strip a CD version suffix ("NAME.BIN;1") -
   classic strchr semantics. */
s8 *f_main_800F6FF8_strchr(const s8 *s, s32 c);
