#include "../../include/types.h"

extern s32 f_init_801630EC_ResolveSlot(ResourceSlot *slot, u8 requireCount, u8 initExtra);

/* Thin wrapper: f_init_801630EC_ResolveSlot with initExtra forced on. */
s32 f_init_801631C0_ResolveSlotAndInit(ResourceSlot *slot, u8 requireCount) {
    return f_init_801630EC_ResolveSlot(slot, requireCount, 1);
}
