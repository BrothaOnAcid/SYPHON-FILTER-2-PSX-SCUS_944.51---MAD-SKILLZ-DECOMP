#include "../../include/types.h"

/* 0x48-byte record table (RAM, .bss-style), indexed by this helper. */
extern Rec48 g_main_801412C0_Rec48[];

/* Return a pointer to the `index`-th 0x48-byte record of the table at
   0x801412C0 (i.e. `&g_main_801412C0_Rec48[index]`). Computed as
   0x801412C0 + index*72. */
Rec48 *f_main_800FD584_Rec48Base(s32 index) {
    return &g_main_801412C0_Rec48[index];
}
