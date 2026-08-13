#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */

/* NOTE: splat's size estimate for this "function" (0x41C bytes, gap to the
   next label) is wrong - this address only covers a genuine ~0x6C-byte
   function that ends in a real `jr $ra` at 0x80160608. The remaining bytes
   up to the next label are actually (at least) two more unlabeled
   functions concatenated in the same nonmatching block (one starting at
   0x80160610 with its own prologue, another at 0x80160804) - a case of the
   "gap to next known symbol" size heuristic misfiring because spimdisasm
   didn't emit glabels for them (see DOC.md). Left undecompiled/unlabeled
   for now; only the real 8016059C function is covered here. */

/* Sets flags0 bits 0x20/0x1 on arg0, then seeds arg0's weapon's unk4A
   (default 0x14 if unset, see also f_init_8015E9C0_SyncTrackParams which
   writes it too) and mirrors its low 7 bits into arg0->unk1 (keeping
   unk1's own bit 0x80). */
void f_init_8016059C_InitWeaponTrackFlags(ObjRecord *arg0) {
    WeaponDef *def = &g_main_8011EEF8_WeaponDefs[arg0->weaponId];

    arg0->flags0 |= 0x21;

    if (def->unk4A != 0) {
        arg0->unk1 = (arg0->unk1 & 0x80) | (def->unk4A & 0x7F);
    } else {
        def->unk4A = 0x14;
        arg0->unk1 = (arg0->unk1 & 0x80) | 0x14;
    }
}
