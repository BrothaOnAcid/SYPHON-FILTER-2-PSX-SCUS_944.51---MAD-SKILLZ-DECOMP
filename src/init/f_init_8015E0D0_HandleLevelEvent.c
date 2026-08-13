#include "../../include/types.h"

extern u8 *D_8011F6C4;                        /* not yet decompiled: pointer set up by f_init_8015B96C_LoadWithFlag */
extern u8 D_8011F65C;
extern u8 D_8011F5C4;
extern s32 D_8011F610;
extern s32 D_8011F558;                        /* callback slot, cleared here (see f_init_8015DF74_ApplyWeaponModeException) */
extern void *D_8011F55C;
extern u8 D_8011F6E8;
extern s16 D_8011F648;
extern s16 D_8011F62C;
extern s16 D_8011F6B0;
extern u8 D_8011F6E4;
extern u8 D_8011EEF4;
extern u8 D_8011EEF5;
extern u8 D_8011EEF6;
extern u8 D_8011F684;
extern void (*D_8011EE98)(void);
extern s16 D_8012B02C;
extern void (*D_80158954[])(s32);              /* not yet decompiled: per-stage handler table, indexed by D_8012B02C */
extern u8 D_8002DCF4;                          /* not yet decompiled */

extern void func_80089298(s32 a0);             /* not yet decompiled */
extern void func_800892A8(s32 a0);             /* not yet decompiled */
extern s32 func_8005CB60(s32 a0, s16 a1, s32 a2, void *a3); /* not yet decompiled */
extern void func_8007B0E0(void);               /* not yet decompiled */
extern void func_8008EB2C(void);               /* not yet decompiled */

void f_init_8015DC3C_DispatchByFlagTable(void);
void f_init_8015DF74_ApplyWeaponModeException(void);
void f_init_8015E850_ResetTypeDefCacheAndNotify(void);

/* Stage/level transition handler. arg0 == 8 is a special "first-time init"
   case: resets a batch of globals to their defaults and, if
   D_8011F6C4->unk29 and ->unk2A (a "count") are both set, wires up
   D_8011F55C = &D_8002DCF4 and calls func_8005CB60 with that count.

   Otherwise (any other arg0): flushes D_8011F6C4->unk2C's pending state via
   func_8007B0E0, runs f_init_8015DC3C_DispatchByFlagTable, then either
   directly invokes func_8008EB2C (arg0 == 3, or D_8011F684 set) or defers it
   by stashing it into D_8011EE98 for later. Finally looks up and calls the
   arg0-taking handler D_80158954[D_8012B02C] (if any), then runs
   f_init_8015DF74_ApplyWeaponModeException and
   f_init_8015E850_ResetTypeDefCacheAndNotify.

   NOTE: this used to be merged with func_8015E274 under one 0x2A0-byte
   nonmatching block, since nothing in this file calls it directly - it's
   only ever called from main (asm/800.s, near 0x80060320) via jal, so
   splat's local-only function-boundary detection never split it out. Fixed
   by manually splitting asm/init/0.s into two nonmatching blocks (this one
   now correctly 0x19C bytes) and adding func_8015E274 to
   symbol_addrs.init.txt. */
void f_init_8015E0D0_HandleLevelEvent(s32 arg0) {
    if (arg0 == 8) {
        func_80089298(0);
        func_800892A8(0);

        D_8011F65C = 1;
        D_8011F5C4 = 0;
        D_8011F610 = 0;
        D_8011F558 = 0;
        D_8011F55C = 0;
        D_8011F6E8 = 0;
        D_8011F648 = -1;
        D_8011F62C = -1;
        D_8011F6B0 = -1;
        D_8011F6E4 = 0;
        D_8011EEF4 = 0;
        D_8011EEF5 = 0;
        D_8011EEF6 = 0;

        if (D_8011F6C4[0x29] != 0 && *(u16 *) (D_8011F6C4 + 0x2A) != 0) {
            D_8011F55C = &D_8002DCF4;
            func_8005CB60(0, *(s16 *) (D_8011F6C4 + 0x2A), 0, &D_8002DCF4);
        }
    } else {
        if (D_8011F6C4[0x2C] != 0) {
            func_8007B0E0();
        }
        f_init_8015DC3C_DispatchByFlagTable();

        if (arg0 == 3 || D_8011F684 != 0) {
            func_8008EB2C();
        } else {
            D_8011EE98 = func_8008EB2C;
        }

        if (D_80158954[D_8012B02C] != 0) {
            D_80158954[D_8012B02C](arg0);
        }

        f_init_8015DF74_ApplyWeaponModeException();
        f_init_8015E850_ResetTypeDefCacheAndNotify();
    }
}
