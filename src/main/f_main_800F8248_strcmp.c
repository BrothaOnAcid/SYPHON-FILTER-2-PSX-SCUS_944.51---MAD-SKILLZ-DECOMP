#include "../../include/types.h"

/* PSX kernel BIOS call trampoline (function 0x17 in the A0 table).
   Address falls in a splat "data" region that isn't disassembled as code yet
   (see DOC.md - deferred full resegmentation); body hand-decoded from the
   raw words: `li $t2, 0xA0; jr $t2; li $t1, 0x17` - standard BIOS A0-table
   dispatch for strcmp. */
s32 f_main_800F8248_strcmp(const s8 *a, const s8 *b);
