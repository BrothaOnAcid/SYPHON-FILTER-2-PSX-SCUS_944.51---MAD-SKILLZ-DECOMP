#include "../../include/types.h"

extern u8 D_80121F60[];
extern s32 D_8011ED04;

/* Zeroes the leading word of each 0x38-byte entry in a 60-entry table,
   then clears a related scalar. */
void f_main_80016D7C_ClearSlotTable(void) {
    s32 i;

    for (i = 0; i < 60; i++) {
        *(s32 *) (D_80121F60 + i * 0x38) = 0;
    }

    D_8011ED04 = 0;
}
