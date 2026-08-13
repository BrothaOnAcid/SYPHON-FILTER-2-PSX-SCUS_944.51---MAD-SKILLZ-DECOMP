#include "../../include/types.h"

/* Blocked-region code (0x800EFD3C-0x80168298, see DOC.md), hand-decoded
   from raw words with toolz/decode_mips.py; uses the same alternate gp
   base (0x8012EC64) as g_main_8012F41C_AudioSeq.

   `rec` is a SongTree/SBNK blob loaded whole into RAM (see SongTree's doc
   comment in types.h for the on-disk layout this function relocates). */
extern s32 g_main_8012F400_LoadBusy; /* guess: reentrancy guard, gp+0x79C */
extern s32 g_main_8012F404_LoadArg;  /* guess: stashed `a4`, gp+0x7A0 */

extern s32 f_main_80105384_AllocResident(s32 size);
extern s32 func_80105550(s32 addr, s32 size);    /* guess: f_main_80105550_ClaimResident -
                                                     coalescing-free-style scan claiming an
                                                     already-known [addr,addr+size) range out
                                                     of the resident free list; not decompiled
                                                     with confidence, left extern */
extern s32 func_8010597C(s32 addr);              /* guess: validates/echoes back a handle */
extern void func_801059DC(void *node);           /* guess: registers a load node */
extern u32 func_80105A0C(s32 findResult, u32 residentSize); /* guess: kicks off the stream read, returns bytes queued */

/* Kicks off a streamed VH/SBNK load for `rec` (the resource record
   FindResource wrote its out-pointer into). On the first call (`a3==0`)
   this obtains a resident buffer for `rec`'s data - either freshly
   allocated (f_main_80105384_AllocResident, when flags08 bit2 is set) or
   claimed at an already-known address (func_80105550, bit2 clear) -
   storing the result in rec->residentAddr. Once validated, relocates
   rec's `slots`/`progs`/`tones` fields from file-relative offsets to
   absolute pointers exactly once (bit0 latch). It then always stashes
   `a4`, registers a load node, and starts the stream read; success
   latches g_main_8012F400_LoadBusy so a second call returns -4 until the
   in-flight load is polled to completion (f_main_800FC268_IsVhLoadDone /
   f_main_800FC274_FinalizeVhLoad). */
s32 f_main_800FC13C_BeginVhLoad(SongTree *rec, s32 findResult, u32 residentSize, s32 a3, s32 a4) {
    s32 h;

    if (g_main_8012F400_LoadBusy != 0)
        return -4;

    if (a3 == 0) {
        if (rec->flags08 & 4) {
            h = f_main_80105384_AllocResident(rec->unk30);
            rec->residentAddr = h;
        } else {
            h = func_80105550(rec->residentAddr, rec->unk30);
        }

        if (h == 0)
            return -0xF;

        if (func_8010597C(rec->residentAddr) != rec->residentAddr)
            return -0xD;

        if (!(rec->flags08 & 1)) {
            rec->slots = (SongSlot *) ((u8 *) rec->slots + PTR_U32(rec));
            rec->progs += PTR_U32(rec);
            rec->tones += PTR_U32(rec);
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
