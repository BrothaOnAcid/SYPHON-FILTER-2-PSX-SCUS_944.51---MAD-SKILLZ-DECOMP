#include "../../include/types.h"

extern u8 g_main_8011EE50_CdStreamActive;         /* guess: nonzero when a CD stream is currently active */
extern FileHandle *g_main_8011EE4C_ActiveHandle; /* guess: the currently active/streaming file handle */

extern void func_80026910(s32 flag); /* guess: deactivate/stop the current CD stream */

/* Closes *handlePtr. If it's the currently active streaming handle, stops
   the stream first. Poisons the freed handle's size field with 0xCACACACA
   and nulls out *handlePtr. Returns 1 on bad/already-closed input. */
s32 f_main_8002662C_CloseFile(FileHandle **hp) {
    FileHandle *h;

    if (hp == 0) {
        return 1;
    }

    h = *hp;
    if (h == 0) {
        return 1;
    }

    if (g_main_8011EE50_CdStreamActive == 1 && h == g_main_8011EE4C_ActiveHandle) {
        func_80026910(1);
    }

    h->size = (s32) 0xCACACACA;
    *hp = 0;

    return 0;
}
