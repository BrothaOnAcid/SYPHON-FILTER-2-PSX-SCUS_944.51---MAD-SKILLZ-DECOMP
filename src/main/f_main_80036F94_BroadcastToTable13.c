#include "../../include/types.h"

extern void func_800128D8(s32 val, void *entry); /* not yet decompiled */
extern u8 g_main_80129FCC_Table13[];              /* guess: 13 entries, stride 0x20 */

/* Calls func_800128D8(*arg0, entry) for each of the 13 entries in the
   0x20-byte-stride table at g_main_80129FCC_Table13. */
void f_main_80036F94_BroadcastToTable13(s32 *arg0) {
    s32 i;
    u8 *entry = g_main_80129FCC_Table13;
    s32 val = *arg0;

    for (i = 0; i < 0xD; i++) {
        func_800128D8(val, entry);
        entry += 0x20;
    }
}
