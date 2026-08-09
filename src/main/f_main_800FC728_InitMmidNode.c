#include "../../include/types.h"

/* Not in asm/800.s: 0x800FC728 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

extern SongTree *f_main_800FC6A8_FindById(s32 id);   /* voice-slot lookup, see f_main_800FC6A8_FindById.c */
extern void f_main_800FC948_AppendActiveMmid(MmidObj *obj);

/* guess: recursively "fixes up" a freshly-loaded MMID blob: relative
   offsets stored in the raw data (obj->tracks[i], and for leaf "MID "
   nodes the +0x18 field) get turned into absolute pointers by adding the
   node's own address, and each node is linked into the active-MMID list.
   Leaf "MID " nodes additionally resolve a voice-slot id (stored at +0x10)
   via f_main_800FC6A8_FindById into +0x14, and clear their +0xC/+0x20
   (next/parent).
   Container "MMID" nodes recurse into each of their trackCount tracks and
   stamp the child's +0x20 with the parent pointer. Anything else is
   ignored. Always returns 0. */
s32 f_main_800FC728_InitMmidNode(MmidObj *obj) {
    s32 i;

    if (obj->magic == 0x2044494D) {
        /* "MID " - single leaf track */
        u8 *base = (u8 *) obj;
        s32 id = *(s32 *) (base + 0x10);
        s32 off18 = *(s32 *) (base + 0x18);
        void *slot = f_main_800FC6A8_FindById(id);

        *(void **) (base + 0x14) = slot;
        obj->next = 0;
        *(s32 *) (base + 0x20) = 0;
        off18 = (s32) PTR_U32(base + off18);
        *(s32 *) (base + 0x18) = off18;
        *(s32 *) (base + 0x1C) = off18;
        f_main_800FC948_AppendActiveMmid(obj);
    } else if (obj->magic == 0x44494D4D) {
        /* "MMID" - track container */
        obj->next = 0;
        f_main_800FC948_AppendActiveMmid(obj);

        for (i = 0; i < obj->trackCount; i++) {
            s32 rel = (s32) PTR_U32(obj->tracks[i]); /* raw relative offset, pre-fixup */
            MmidObj *child = (MmidObj *) ((u8 *) obj + rel);

            obj->tracks[i] = child;
            f_main_800FC728_InitMmidNode(child);
            *(MmidObj **) ((u8 *) child + 0x20) = obj;
        }
    }

    return 0;
}
