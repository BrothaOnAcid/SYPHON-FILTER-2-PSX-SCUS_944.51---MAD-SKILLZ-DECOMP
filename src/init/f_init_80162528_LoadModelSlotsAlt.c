#include "../../include/types.h"

extern u16 D_8011F1E0;   /* not yet decompiled: running count of loaded slots across both passes */
extern s16 D_8012B02C;
extern s32 D_8010C874[];
extern u8 D_80136B4C[];  /* not yet decompiled: 2 * 0x20-byte scratch name buffers, cleared each pass */
extern u8 D_80136BB4[];  /* not yet decompiled: built path buffer, passed to func_800F045C/func_80025F64 */
extern WldModelSlot D_8013672C[]; /* not yet decompiled: 2 * 0x21-entry model-slot table, stride 0x10 */
extern u8 D_80158BB0[];  /* not yet decompiled: sprintf format string */
extern u8 D_80158BC4[];  /* not yet decompiled: extension/template string, pass 2 */
extern u8 D_80158BC8[];  /* not yet decompiled: extension/template string, pass 1 */

extern void func_800A3B90(void);                    /* not yet decompiled */
extern void func_800F045C(void *dst, u8 *fmt, s32 a, u8 *b); /* not yet decompiled */
extern s32 func_80025F64(void *scratch, void *path);  /* not yet decompiled */
extern s32 func_800F836C(s32 handle);                  /* not yet decompiled */
extern void func_800F8268(s32 sector, void *out);        /* not yet decompiled */

/* Two passes (offset 0 and 0x20 into D_80136B4C/D_8013672C's outer
   stride-0x20/0x210 regions): clears a 32-byte name buffer, builds a path
   via func_800F045C (same key both passes - D_8010C874[D_8012B02C] - but a
   different extension template: D_80158BC8 first pass, D_80158BC4 second),
   and resolves it via func_80025F64 (into a small local scratch buffer, not
   the name buffer). On success, bumps D_8011F1E0 and fills
   33 WldModelSlot entries (same shape as f_init_801627C0_LoadWldModels'
   g_main_8011F6C8_ModelSlots): name cleared to NULL, sectorInfo filled via
   func_800F8268 (sector starting at func_800F836C(handle)+1, +0x10 per
   entry), index/index2 = the entry's own 0..0x20 counter. */
void f_init_80162528_LoadModelSlotsAlt(void) {
    s32 pass;

    D_8011F1E0 = 0;
    func_800A3B90();

    for (pass = 0; pass < 0x40; pass += 0x20) {
        u8 *nameBuf = D_80136B4C + pass;
        WldModelSlot *slots = (WldModelSlot *) ((u8 *) D_8013672C + (pass / 0x20) * 0x210);
        u8 *ext = (pass == 0) ? D_80158BC8 : D_80158BC4;
        u8 scratch[0x40];
        s32 handle;
        s32 i;

        for (i = 0; i < 0x20; i++) {
            nameBuf[i] = 0;
        }

        func_800F045C(D_80136BB4, D_80158BB0, D_8010C874[D_8012B02C], ext);
        handle = func_80025F64(scratch, D_80136BB4);

        if (handle != 0) {
            s32 sector = func_800F836C(handle) + 1;

            D_8011F1E0++;

            for (i = 0; i < 0x21; i++) {
                WldModelSlot *slot = &slots[i];

                slot->name = 0;
                func_800F8268(sector, &slot->sectorInfo);
                slot->index = (u8) i;
                slot->index2 = (u8) i;

                sector += 0x10;
            }
        }
    }
}
