#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs;

typedef struct {
    s32 unk0;
    u16 unk4;
    u8 _pad6[0x2];
    u16 unk8;
} TrackParamResult;

extern void func_80025004(s32 a0, s32 a1, TrackParamResult *out);

/* arg0/arg1: same weapon-holder/track shape as f_init_80161E28_UpdateWeaponTrack
   (id at +0x2, a pointer at +0x8 whose +0xC is passed through), and arg0's
   +0x4 field is a packed pair of u16 halves. */
void f_init_8015E9C0_SyncTrackParams(void *arg0, void *arg1) {
    TrackParamResult res;
    u32 *packed;

    func_80025004(*(s32 *) (*(u8 **) ((u8 *) arg0 + 0x8) + 0xC),
                  *(s32 *) (*(u8 **) ((u8 *) arg1 + 0x8) + 0xC),
                  &res);

    g_main_8011EEF8_WeaponDefs[*(s16 *) ((u8 *) arg0 + 0x2)].unk4A = (s16) res.unk0;

    packed = (u32 *) ((u8 *) arg0 + 0x4);
    *packed = (u32) (res.unk8 << 16) | res.unk4;
}
