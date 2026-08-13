#include "../../include/types.h"

/* Blocked-region code (0x800EFD3C-0x80168298, see DOC.md), hand-decoded
   from raw words with toolz/decode_mips.py; uses the alternate gp base
   0x8012EC64. Called by f_init_80168190_LoadSongTree once
   f_main_800FC268_IsVhLoadDone reports the streamed VH read is complete.

   Does the SECOND relocation pass for a freshly-loaded SongTree/SBNK
   blob (the first pass, self-relative `+= rec` on slots/progs/tones, is
   in f_main_800FC13C_BeginVhLoad): every entry in `tones[]` (toneCount of
   them, 24-byte stride) gets its own +0x14 field rebased against
   `residentAddr` - this is almost certainly a per-tone sample-data offset
   turning into an absolute pointer into the resident buffer that
   f_main_80105384_AllocResident/f_main_80105550_ClaimResident filled in
   for this SongTree. `progs[]` entries (progCount of them, 8-byte stride)
   only get their own +4 field rebased self-relative (+= rec) - left
   unnamed, not decoded further. Finally links `rec` onto the tail of
   g_main_8012F408_SongTreeListHead (the same list
   f_main_800FC6A8_FindById/f_main_800FC3F0_CompileEventLists walk),
   confirming SongTree is exactly this streamed-VH object - see its doc
   comment in types.h. */

extern SongTree *g_main_8012F408_SongTreeListHead;

void f_main_800FC274_FinalizeVhLoad(SongTree *rec) {
    s32 i;

    for (i = 0; i < rec->toneCount; i++) {
        u8 *tone = rec->tones + i * 0x18;
        *(s32 *) (tone + 0x14) += rec->residentAddr;
    }

    for (i = 0; i < rec->progCount; i++) {
        u8 *prog = rec->progs + i * 8;
        *(s32 *) (prog + 4) += PTR_U32(rec);
    }

    if (g_main_8012F408_SongTreeListHead == 0) {
        g_main_8012F408_SongTreeListHead = rec;
    } else {
        SongTree *tail = g_main_8012F408_SongTreeListHead;
        while (tail->next != 0)
            tail = tail->next;
        tail->next = rec;
    }
}
