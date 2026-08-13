#include "../../include/types.h"

/* Guessed layout: three tables used by the token-script interpreter
   (f_init_8015BA54_RunTokenScript / f_init_8015B794_RunTokenScriptInline),
   zeroed here before a script runs. Addresses are contiguous in bss with
   no other references found, so sizes are inferred purely from the loop
   trip counts in the asm. */
s32 g_init_801385D4_TokenState1[0x2B];   /* 43 words, ends right before g_init_80138554 */
s32 g_init_80138554_TokenState2[0x20];   /* 32 words */
s32 g_init_80138304_TokenState3[0x24][4]; /* 36 entries of 4 words (0x10 bytes) each */

u8 g_init_8012B0BC_TokenFlags1[0x2B]; /* 43 bytes, contiguous with g_init_8012B0E7 */
u8 g_init_8012B0E7_TokenFlags2[0x2B]; /* 43 bytes */

void f_init_8015944C_InitTokenScriptState(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 0x2B; i++) {
        g_init_801385D4_TokenState1[i] = 0;
    }

    for (i = 0; i < 0x20; i++) {
        g_init_80138554_TokenState2[i] = 0;
    }

    for (i = 0; i < 0x24; i++) {
        for (j = 0; j < 4; j++) {
            g_init_80138304_TokenState3[i][j] = 0;
        }
    }

    for (i = 0; i < 0x2B; i++) {
        g_init_8012B0BC_TokenFlags1[i] = 0xFF;
        g_init_8012B0E7_TokenFlags2[i] = 0xFF;
    }
}
