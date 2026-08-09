#include "../../include/types.h"

extern u8 g_main_8010C2D8_DefaultVtable[]; /* guess: default handler/vtable stub */

extern void func_80016BC0(void);
extern void func_80013610(void (*func)(void), s32 arg1, void *obj); /* event-register helper */

/* arg0->unk1C is a pointer-to-pointer; if the pointee is still NULL, this
   installs the default handler, sets flag bit 0xA|=0x10, optionally
   registers func_80016BC0 as a callback (unless arg1 == 0x29A), and stamps
   arg0->unk10->unk1C with a fixed constant. */
void f_main_80016BE0_InitCallback(void *arg0, s32 arg1) {
    void **slot = *(void ***) ((u8 *) arg0 + 0x1C);

    if (*slot == 0) {
        *slot = g_main_8010C2D8_DefaultVtable;
        *((u8 *) arg0 + 0xA) |= 0x10;

        if (arg1 != 0x29A)
            func_80013610(func_80016BC0, arg1, arg0);

        *(s32 *) (*(u8 **) ((u8 *) arg0 + 0x10) + 0x1C) = 0x304050;
    }
}
