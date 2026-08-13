#include "../../include/types.h"

extern void *D_8011F6E0;   /* not yet decompiled: pointer into the D_8012B114 region, set by f_init_8015B96C_LoadWithFlag */
extern void *D_8011F6C4;   /* not yet decompiled */
extern u8 D_8012B114[];    /* not yet decompiled: base of the token-script region */
extern void (*D_8016881C[])(TokenCursor *); /* not yet decompiled: opcode -> handler table, see f_init_8015BA54_RunTokenScript */

extern void f_init_801594F4_ReadTokenWord(TokenCursor *cur, s16 *out); /* base arg dropped: real function only takes (cur, out), the 3rd arg passed by this caller is unused */

/* guess: same opcode-script interpreter as f_init_8015BA54_RunTokenScript,
   but with the "resolve stream + cursor" step inlined instead of going
   through f_init_8015944C_InitTokenScriptState/f_main_8002B0D0_LoadFileAlloc:
   derives an entry count from a field
   at +0x88 of the struct D_8011F6E0 points to (count*0x10 + 0x90, converted
   to a word offset from D_8012B114) and uses that both to set D_8011F6C4
   and to seed the cursor 0x40 bytes (0x10 words) further in. */
void f_init_8015B794_RunTokenScriptInline(void) {
    TokenCursor cur;
    s32 wordOff;
    s16 count;
    s16 op;
    s32 i;
    s8 unused; /* matches a dead `= 1` store in the asm right next to the
                  cursor's stack slot; never read (not a bug - matched) */

    unused = 1;
    wordOff = (s32) (((u8 *) D_8011F6E0 + (*((s32 *) ((u8 *) D_8011F6E0 + 0x88)) * 0x10 + 0x90)) - D_8012B114) >> 2;
    D_8011F6C4 = D_8012B114 + wordOff * 4;
    cur.pos = D_8012B114 + (wordOff + 0x10) * 4;
    f_init_801594F4_ReadTokenWord(&cur, &count);
    for (i = 0; i < count; i++) {
        if (*cur.pos == 0xFE) {
            do {
                cur.pos++;
            } while (*cur.pos == 0xFE);
        }
        f_init_801594F4_ReadTokenWord(&cur, &op);
        f_init_801594F4_ReadTokenWord(&cur, (s16 *) &cur);
        D_8016881C[op](&cur);
    }
}
