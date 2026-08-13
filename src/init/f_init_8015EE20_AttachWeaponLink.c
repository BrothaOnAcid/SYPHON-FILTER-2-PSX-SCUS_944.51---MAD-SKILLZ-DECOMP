#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */

extern void func_80017624(void *inner);              /* not yet decompiled */
extern void func_80024000(void *innerPlus8);          /* not yet decompiled */
extern void func_800241D8(s32 a, s32 b);               /* not yet decompiled */
extern void *func_80025AD0(s32 size);                  /* not yet decompiled: bump allocator, see f_main_80025AD0_AllocDown */
extern void func_80046950(void *obj, s32 flag);         /* not yet decompiled */

/* guess: allocates a small "link" wrapper { void *inner; u32 flags; } plus
   an 0x50-byte inner block, wires the inner block via func_80017624/
   func_80024000/func_800241D8, and cross-links arg0 (weapon-holder object,
   weaponId at +0x2, flag byte at +0x0) with arg1 (another weapon-holder,
   u16 at +0x2, pointer at +0x8): stashes the wrapper into arg0's WeaponDef
   at +0x24 (NOTE: this overlaps the separately-confirmed WeaponDef.ammoType
   u16 at that same offset - the two guesses conflict and the real layout
   there needs re-checking), sets "attached" flag bits on both arg0/arg1,
   and copies arg1's +0x2 field into the wrapper's flags' low half. */
void f_init_8015EE20_AttachWeaponLink(void *arg0, void *arg1) {
    struct { void *inner; u32 flags; } *link;
    void *inner;

    link = (void *) func_80025AD0(8);
    inner = func_80025AD0(0x50);
    link->inner = inner;
    func_80017624(inner);

    *(u32 *) ((u8 *) inner + 4) |= 2;

    *(u32 *) ((u8 *) g_main_8011EEF8_WeaponDefs + *(s16 *) ((u8 *) arg0 + 2) * 0x4C + 0x24) = (u32) (u64) link;

    *(u8 *) arg0 |= 2;
    func_80046950(arg0, 2);

    func_80024000((u8 *) inner + 8);
    func_800241D8(*(s32 *) ((u8 *) inner + 8), *(s32 *) (*(u8 **) ((u8 *) arg1 + 8) + 0xC));

    link->flags &= 0xFFFEFFFF;
    *(u8 *) arg1 |= 2;
    *(u16 *) &link->flags = *(u16 *) ((u8 *) arg1 + 2);
}
