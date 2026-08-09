#include "../../include/types.h"

/* PSX BIOS trampolines, confirmed via hand-decoded A0-table indices - see
   f_main_800F8248_strcmp.c / f_main_800F8258_strcpy.c */
extern s32 f_main_800F8248_strcmp(const s8 *a, const s8 *b);
extern s8 *f_main_800F8258_strcpy(s8 *dst, const s8 *src);

extern u32 f_main_8002B4C4_LoadFile(s8 *name, void *arg1, s32 mode);

extern s8 g_main_8010C60C_CurrentOverlayName[]; /* name of the currently loaded overlay */
extern u8 g_main_80142150_ScratchBuffer[]; /* generic scratch read buffer, see f_main_8002B4C4_LoadFile */

void f_main_8002B354_LoadOverlay(s8 *name, u8 param) {
    if (name == 0) {
        g_main_8010C60C_CurrentOverlayName[0] = 0;
        return;
    }

    if (f_main_800F8248_strcmp(name, g_main_8010C60C_CurrentOverlayName) != 0) {
        f_main_8002B4C4_LoadFile(name, g_main_80142150_ScratchBuffer, param);
        f_main_800F8258_strcpy(g_main_8010C60C_CurrentOverlayName, name);
    }
}
