#include "../../include/types.h"

/* Blocked-region code (0x800EFD3C-0x80168298, see DOC.md), hand-decoded
   from raw words with toolz/decode_mips.py; uses the same alternate gp
   base (0x8012EC64) as g_main_8012F41C_AudioSeq. */
extern s32 g_main_8012F400_LoadBusy; /* guess: reentrancy guard, gp+0x79C */
extern s32 g_main_8012F404_LoadArg;  /* guess: stashed `a4`, gp+0x7A0 */

extern s32 func_80105384(s32 src);              /* guess: alt-path VAB init, returns a handle */
extern s32 func_80105550(s32 handle, s32 src);   /* guess: normal-path VAB init */
extern s32 func_8010597C(s32 handle);            /* guess: validates/echoes back a handle */
extern void func_801059DC(void *node);           /* guess: registers a load node */
extern u32 func_80105A0C(s32 findResult, u32 residentSize); /* guess: kicks off the stream read, returns bytes queued */

/* guess: kicks off a streamed VH/VAB load for `rec` (the resource record
   FindResource wrote its out-pointer into). On the first call (`a3==0`)
   this initializes the VAB handle (two variants gated by rec->flags08 bit
   2) and, once validated, relocates rec's three offset fields to absolute
   pointers (self-relative, `+= rec`) exactly once (bit 0 latch). It then
   always stashes `a4`, registers a load node, and starts the stream read;
   success latches g_main_8012F400_LoadBusy so a second call returns -4
   until the in-flight load is polled to completion
   (f_main_800FC268/f_main_800FC274, not yet decompiled). */
s32 f_main_800FC13C_BeginVhLoad(VhLoadRecord *rec, s32 findResult, u32 residentSize, s32 a3, s32 a4) {
    s32 h;

    if (g_main_8012F400_LoadBusy != 0)
        return -4;

    if (a3 == 0) {
        if (rec->flags08 & 4) {
            h = func_80105384(rec->unk30);
            rec->unk2C = h;
        } else {
            h = func_80105550(rec->unk2C, rec->unk30);
        }

        if (h == 0)
            return -0xF;

        if (func_8010597C(rec->unk2C) != rec->unk2C)
            return -0xD;

        if (!(rec->flags08 & 1)) {
            rec->unk20 += (s32) PTR_U32(rec);
            rec->unk24 += (s32) PTR_U32(rec);
            rec->unk28 += (s32) PTR_U32(rec);
            rec->flags08 |= 1;
        }
    }

    g_main_8012F404_LoadArg = a4;
    func_801059DC((void *) 0x800FC994); /* guess: static load-node record right after this function */
    if (func_80105A0C(findResult, residentSize) >= residentSize) {
        g_main_8012F400_LoadBusy = 1;
        return 0;
    }
    return -0xE;
}
