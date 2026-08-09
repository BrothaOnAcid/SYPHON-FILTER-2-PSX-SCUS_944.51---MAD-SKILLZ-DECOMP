#include "../../include/types.h"

/* PSYQ audio-sequencer globals; this function is in the blocked region
   (0x800EFD3C-0x80168298) that uses its own gp base 0x8012EC64, not main's
   gp 0x8011EC64. */
extern s32 (*g_main_8012F3CC_PlayFn)(void *a, s32 b, s32 c);  /* guess: used
    both as fn(tree, idx, 0) and fn(&slot, ov1, ov2) below - probably a
    loosely typed callback; third arg is unused/zero on the first call. */

/* --- libc-style externs (not yet decompiled) --- */
extern void f_main_800FA404_DispatchSongSlot(SongTree *tree, s32 idx);
extern s32 f_main_800FA720_FindRecordsInRange(u8 *handler, s32 index, s32 value, void **outList);
extern s32 f_main_800FCEA8_AllocateChannel(u8 voiceId, void *arg, ChannelCb cb);
extern void f_main_800FA7AC_ClearChildMaskBit(s32 channel, SongSlot *slot, s32 cmd);
extern void f_main_800FBC30_ComputeStereoPan(s32 a0, s32 a1, s32 a2, s32 a3, s32 e, s32 f, s32 g, s16 *buf);
extern void f_main_800FD020_ActivateChannelRecord(s32 index, void *src, s32 flag);
extern s32 f_main_800FCE28_SetStopCallback(void (*cb)(void));  /* here used
    with cb=NULL, purely to claim/query the one-shot flag; its return value
    (0 or 1) doubles as the trip count of the loop below */
extern void f_main_800FCE58_FlushStopCallback(void);

/* Recursive song/sequence-node dispatcher, keyed by SongSlot.state (0-14,
   via a jump table read directly out of the binary at 0x8010BDD8, since
   this region's rodata/text split is still unresolved): state 0 and 7-13
   are invalid (return -1); 2/3 gate on whether the slot already has a
   queued child (childMask); 4/5 delegate entirely to
   g_main_8012F3CC_PlayFn; 14 scans forward through up to childCount
   sibling slots for the first with childMask == 0 and recurses into it.
   States 1, 6, and >=15 fall through to the shared "apply overrides and
   trigger" path.

   ov1/ov2 are pitch/vol overrides: -1 = reset to the slot's *Default
   backup, -2 = leave unchanged, anything else = set directly.

   The trigger path (guarded by the global one-shot latch in
   f_main_800FCE28_SetStopCallback, so it only actually runs the first time
   it's reached) fills a small list of entries via
   f_main_800FA720_FindRecordsInRange, then for
   each: resolves an id via f_main_800FCEA8_AllocateChannel, builds an
   8-field argument block
   (some fields sourced from a per-childCount lookup table at
   handler+0x24, stride 8) for f_main_800FBC30_ComputeStereoPan, fills a
   second scratch
   buffer for f_main_800FD020_ActivateChannelRecord, and marks the resulting id's bit in
   childMask. Field roles inside that block are still largely unconfirmed
   guesses - kept as raw offsets rather than invented names. */
s32 f_main_800F9F68_PlaySongNode(SongTree *tree, s32 idx, s32 ov1, s32 ov2) {
    SongSlot *cur;
    s32 state;
    s32 i;
    s32 fp;
    void *list[16];   /* guessed size for the scratch list f_main_800FA720_FindRecordsInRange fills */
    u8 buf2[0x34];    /* guessed size for the scratch buffer passed to
                          f_main_800FBC30_ComputeStereoPan/f_main_800FD020_ActivateChannelRecord -
                          bumped from 0x28 to 0x34 since the latter copies
                          0x34 bytes out of it */

    if (tree == 0)
        return -1;
    if (idx >= tree->count)
        return -1;
    if (idx < 0)
        return -1;

    cur = &tree->slots[idx];
    if (cur->handler == 0)
        return -1;

    state = cur->state;

    if (state >= 0 && state < 15) {
        switch (state) {
        case 0:
        case 7: case 8: case 9: case 10: case 11: case 12: case 13:
            return -1;

        case 2:
            if (cur->childMask != 0)
                f_main_800FA404_DispatchSongSlot(tree, idx);
            break;

        case 3:
            if (cur->childMask != 0)
                return -1;
            break;

        case 4:
        case 5:
            if (g_main_8012F3CC_PlayFn == 0)
                return -1;
            if (g_main_8012F3CC_PlayFn(tree, idx, 0) != 0)
                return -1;
            if (g_main_8012F3CC_PlayFn(cur, ov1, ov2) == 1)
                return idx;
            return -1;

        case 14:
            if (cur->childCount == 0)
                return -1;
            for (i = 0; i < cur->childCount; i++) {
                if (tree->slots[idx + 1 + i].childMask == 0)
                    return f_main_800F9F68_PlaySongNode(tree, idx + i + 1, ov1, ov2);
            }
            return -1;

        default: /* 1, 6 */
            break;
        }
    }

    /* Shared "apply overrides" path (state 1/6/>=15, or state 2/3 after
       their guard above). Re-fetch state since state 14 skips this
       entirely by returning above, but everything else - including states
       >= 15 that never entered the switch - reaches here. */
    if (cur->state != 14) {
        if (ov1 != -1) {
            if (ov1 != -2)
                cur->pitch = (u8) ov1;
        } else {
            cur->pitch = cur->pitchDefault;
        }

        if (ov2 != -1) {
            if (ov2 != -2)
                cur->vol = (u16) ov2;
        } else {
            cur->vol = cur->volDefault;
        }
    }

    /* Trigger path, effectively runs at most once total (global latch). */
    fp = f_main_800FCE28_SetStopCallback(0);
    f_main_800FA720_FindRecordsInRange((u8 *) cur->handler, cur->childCount, cur->unk0E, list);

    for (i = 0; i < fp; i++) {
        u8 *entry = (u8 *) list[i];
        u8 *table;
        u8 *rec;
        s32 result;

        /* guess: arg1 in the real asm is `*(cur+0x20) + <a per-child sub-table
           pointer derived earlier in this loop>`, not simply `cur` - the exact
           expression needs revisiting; `cur` is a placeholder that preserves
           the call shape (voiceId, arg, callback) without claiming false
           precision on arg1. cmd-callback pointer is exact (a fixed constant
           in the real asm). */
        result = f_main_800FCEA8_AllocateChannel(*entry, cur, (ChannelCb) f_main_800FA7AC_ClearChildMaskBit);
        if (result == -1)
            continue;

        entry[0x10] = (u8) result;

        table = (u8 *) cur->handler + 0x24;
        rec = table + cur->childCount * 8;

        buf2[0x00] = 0;
        buf2[0x01] = 0;
        buf2[0x02] = 0;
        buf2[0x03] = 0;
        f_main_800FBC30_ComputeStereoPan(cur->unk0E, cur->pitch, cur->vol, rec[1],
                      *(u16 *) (rec + 2), *(s16 *) (entry + 4), entry[0],
                      (s16 *) buf2);

        *(u16 *) (buf2 + 0x04) = 0x10;
        *(u16 *) (buf2 + 0x06) = cur->unk0E;
        *(u16 *) (buf2 + 0x0C) = (u16) (cur->unk10 | (cur->unk0E << 8));
        *(u16 *) (buf2 + 0x0E) = (u16) (entry[3] | (entry[2] << 8));
        *(u32 *) (buf2 + 0x10) = *(u32 *) (entry + 0x14);
        *(u16 *) (buf2 + 0x18) = *(u16 *) (entry + 0xA);
        *(u16 *) (buf2 + 0x1A) = *(u16 *) (entry + 0xC);
        *(u32 *) (buf2 + 0x20) = 0;
        *(u32 *) (buf2 + 0x1C) = entry[0];
        *(u32 *) (buf2 + 0x24) = cur->unk18;
        *(u16 *) (buf2 + 0x14) = *(u16 *) (entry + 0xE) & 1;

        f_main_800FD020_ActivateChannelRecord(result, buf2, *(u16 *) (entry + 0xE) & 1);

        cur->childMask |= (1u << result);
    }

    f_main_800FCE58_FlushStopCallback();

    cur = &tree->slots[idx];
    if (cur->childMask != 0)
        return idx;
    return -1;
}
