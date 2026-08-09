#include "../../include/types.h"

extern s16 g_init_80169798_unk[]; /* guess: 156-byte zeroed output buffer, filled by func_800F1598 */

/* Suspicious: func_800F1598 is in the blocked 0x800Fxxxx PSYQ library
   region (see DOC.md) - not yet examined, name/signature guessed from
   call-site shape (4 shorts in, dest buffer out; looks GPU/texture related
   given the values used here). */
extern s16 func_800F1598(s16 *info, void *out);

/* Sets up a 4-field short struct (guessed: x/y/w/h or similar) and passes
   it with a fixed destination buffer to func_800F1598. Return value unused. */
void f_init_80168120_LoadTexInfo(void) {
    s16 info[4];

    info[0] = 0x3E0;
    info[1] = 0x120;
    info[2] = 0x10;
    info[3] = 0x20;
    func_800F1598(info, g_init_80169798_unk);
}
