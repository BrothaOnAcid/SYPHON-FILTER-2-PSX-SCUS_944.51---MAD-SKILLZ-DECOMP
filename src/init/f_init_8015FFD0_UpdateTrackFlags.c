#include "../../include/types.h"

extern void *func_80045AA4(s16 id, s16 arg1); /* guess: look up/create a track object by id */
extern void func_8002CFCC(void *obj, s32 a1);

/* arg0: same weapon-holder/track shape as f_init_80161E28_UpdateWeaponTrack
   (id at +0x2, a pointer at +0x8 whose +0xC is passed through). */
void f_init_8015FFD0_UpdateTrackFlags(void *arg0, s16 arg1) {
    void *obj;
    void *inner;

    obj = func_80045AA4(*(s16 *) ((u8 *) arg0 + 0x2), arg1);

    if (*(s16 *) ((u8 *) obj + 0x2) != *(s16 *) ((u8 *) arg0 + 0x2)) {
        func_8002CFCC(obj, *(s32 *) (*(u8 **) ((u8 *) arg0 + 0x8) + 0xC));

        inner = *(void **) ((u8 *) obj + 0x18);
        *((u8 *) obj + 0x25) = 0xFF;
        *(u16 *) ((u8 *) inner + 0x8) = *(u16 *) ((u8 *) inner + 0x6);
        *((u8 *) obj + 0x24) |= 0x10;
    }
}
