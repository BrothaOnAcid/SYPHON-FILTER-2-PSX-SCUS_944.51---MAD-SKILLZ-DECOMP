#include "../../include/types.h"

/* Parallel stride-0x24 arrays: D_8012F624[i] is a name-string pointer (or
   NULL), D_8012F628[i] is the resulting WldTypeDef* (offset 0 of each
   0x24-byte slot; the rest is untouched here). 34 slots, D_8012F628 to
   D_8012FAF0. */
extern u8 D_8012F624[];
extern u8 D_8012F628[];

extern s32 D_8011F60C;   /* real type HogArchive* (see f_main_8002D4B4_FindResourceOrZero); kept s32-shaped
                            here to match func_80026E00's s32 ctx param */
extern s32 D_8011F71C;
extern u8 D_8011F724;
extern WldTypeDef *D_8011F720;

/* Stride-4 array of "cleared to -1" flags/handles (30 entries, D_8012F5AC
   to D_8012F5AC+0x78), parallel to a stride-4 array of WldRes* out-slots
   starting at D_8012F534. */
extern s32 D_8012F5AC[];
extern WldRes *D_8012F534[];

extern s32 func_80026E00(s32 ctx, u8 *str, s32 *out);           /* not yet decompiled */
extern WldTypeDef *f_init_80166D14_AllocLinked(s32 arg0, s32 arg1);
extern s32 func_800F8258(u8 *out, u8 *str);                      /* not yet decompiled */
extern s32 f_main_8002D4B4(const s8 *name, s32 *out); /* real out param is void**; declared s32* here since
                                                          this file only treats the resolved value as a key */
extern s32 f_init_80166C08_CreateWldRes(s32 key, s32 tag, s32 flags, WldRes **out);

/* Fills the 34 D_8012F624/D_8012F628 name/typeDef slots: for each named
   slot (non-NULL pointer, non-empty string), parses the string via
   func_80026E00 into an intern key and creates a WldTypeDef via
   f_init_80166D14_AllocLinked (flags 0x04010000). A second func_80026E00
   pass (fed through func_800F8258 with a trailing 'X' marker byte) parses
   a second value which is stashed into the typeDef's unk24 field; any
   parse failure along the way just clears the slot to NULL.

   Then resolves two named resources ("D_80158BA4"/"D_80158BE0" - actual
   text not decompiled) the same way, the second one's typeDef going into
   the global D_8011F720 and getting its own unk24 set from its second
   parsed value.

   Finally walks 30 D_8012F5AC/D_8012F534 slots: if the (constant, reused)
   first parsed value is set, creates a WldRes into D_8012F534[i] (flags
   0x04010000) and clears its unk18/sets unk09=0x11; either way D_8012F5AC[i]
   is reset to -1. Wraps up by resetting D_8011F71C to -1 and flagging
   D_8011F724. */
void f_init_80162E44_LoadTypeSlots(void) {
    s32 scratch;
    u8 fmtBuf[0x10];
    s32 i;

    for (i = 0; &D_8012F628[i * 0x24] < D_8012F628 + 0x4C8; i++) {
        u8 *name = *(u8 **) &D_8012F624[i * 0x24];
        WldTypeDef **slot = (WldTypeDef **) &D_8012F628[i * 0x24];

        if (name == 0 || *name == 0) {
            *slot = 0;
            continue;
        }

        if (func_80026E00(D_8011F60C, name, &scratch) != 0) {
            *slot = 0;
            continue;
        }

        *slot = f_init_80166D14_AllocLinked(scratch, 0x04010000);

        name = *(u8 **) &D_8012F624[i * 0x24];
        func_800F8258(fmtBuf, name);
        fmtBuf[0] = 0x58;

        if (func_80026E00(D_8011F60C, fmtBuf, &scratch) == 0) {
            (*slot)->unk24 = scratch;
        }
    }

    f_main_8002D4B4((const s8 *) "D_80158BA4", &scratch);
    D_8011F720 = f_init_80166D14_AllocLinked(scratch, 0x04010000);
    f_main_8002D4B4((const s8 *) "D_80158BE0", &scratch);
    D_8011F720->unk24 = scratch;

    for (i = 0; i < 30; i++) {
        if (scratch != 0) {
            WldRes *res;

            f_init_80166C08_CreateWldRes(scratch, 0, 0x04010000, &D_8012F534[i]);
            res = D_8012F534[i];
            res->unk18 = 0;
            res->unk09 = 0x11;
        }
        D_8012F5AC[i] = -1;
    }

    D_8011F71C = -1;
    D_8011F724 = 1;
}
