#include "../../include/types.h"

/* PSX kernel BIOS call trampoline (function 0x19 in the A0 table).
   Address falls in a splat "data" region that isn't disassembled as code yet
   (see DOC.md - deferred full resegmentation); body hand-decoded from the
   raw words: `li $t2, 0xA0; jr $t2; li $t1, 0x19` - standard BIOS A0-table
   dispatch for strcpy. */
s8 *f_main_800F8258_strcpy(s8 *dst, const s8 *src);
