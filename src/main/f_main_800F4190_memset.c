#include "../../include/types.h"

/* PSX kernel BIOS call trampoline (function 0x2B in the A0 table).
   Address falls in a splat "data" region that isn't disassembled as code yet
   (see DOC.md - deferred full resegmentation); body hand-decoded from the
   raw words: `li $t2, 0xA0; jr $t2; li $t1, 0x2B` - standard BIOS A0-table
   dispatch for memset. Confirmed by every known call site passing
   (dst, fillValue, size). */
void *f_main_800F4190_memset(void *dst, s32 fillValue, s32 size);
