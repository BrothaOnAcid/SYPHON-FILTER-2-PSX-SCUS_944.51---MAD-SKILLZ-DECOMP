#include "../../include/types.h"

/* PSX kernel BIOS call trampoline (function 0x1E in the A0 table), same
   pattern as f_main_800F8248_strcmp.c. Behavior confirmed by call site
   (f_init_801627C0_LoadWldModels): searches a model name for '.' to
   truncate the extension - classic strchr semantics, same as the other BIOS
   strchr trampoline at index 0x1F (f_main_800F6FF8_strchr) - apparently two
   aliases for the same behavior at different table indices. */
s8 *f_main_800F8238_strchr(const s8 *s, s32 c);
