#include "../../include/types.h"

/* Guessed: generic doubly-linked node - prev at +0x0, next at +0x4 (same
   shape as SeqNode's link fields, but this is a different list: the head
   lives at obj->unk8->unk28, not AudioSeqState.seqHead). Prepends `node`
   to that list and always returns 1. */
s32 f_main_8009AF88_PrependLinkNode(u8 *obj, u8 *node) {
    u8 *container = *(u8 **) (obj + 8);
    u8 *oldFirst;

    *(u8 **) (node + 0) = 0;
    oldFirst = *(u8 **) (container + 0x28);
    *(u8 **) (node + 4) = oldFirst;
    *(u8 **) (container + 0x28) = node;
    if (oldFirst != 0) {
        *(u8 **) (oldFirst + 0) = node;
    }
    return 1;
}
