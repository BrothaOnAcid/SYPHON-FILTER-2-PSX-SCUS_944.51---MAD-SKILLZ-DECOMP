#include "../../include/types.h"

/* Per-frame animation dispatch - see f_main_8001A7BC_UpdateAnimEntries. */
extern void f_main_8001A7BC_UpdateAnimEntries(AnimOwner *owner);

/* Gated wrapper around the anim update. Runs it only while the def's +0x28
   flags carry 0x400000 (anims enabled) and clear 0x2000000 (not blocked), the
   node/angle arrays (+0x18/+0x24) are present, and bit 3 of +0x8 is clear.
   Called per-entity from the actor update loop func_80068C38 (a0 = entity). */
void f_main_8001A740_UpdateAnims(AnimOwner *owner) {
    u32 defFlags;

    defFlags = *(u32 *) ((u8 *) owner->def + 0x28);
    if ((defFlags & 0x400000) == 0)
        return;
    if (owner->anim18 == 0 || owner->extra24 == 0)
        return;
    if ((owner->flags08 & 0x8) != 0)
        return;
    if ((defFlags & 0x2000000) != 0)
        return;
    f_main_8001A7BC_UpdateAnimEntries(owner);
}
