#include "../../include/types.h"

extern void func_80010010(void *obj, s32 id, s32 mode);

/* Scans up to 16 entries of obj's +0x80 tag-slot list for one whose low 12
   bits match id. If found and its 0x8000 "active" flag is set, clears the
   slot (tag = 0xFFF) and notifies func_80010010. */
void f_main_8001035C_ClearTaggedSlot(HandleObj80 *obj, s32 id) {
    TagSlot *p = obj->slots80;
    s32 n = 16;

    if (p == 0)
        return;

    while (n != 0) {
        u16 tag = p->tag;
        if (tag == 0xFFFF)
            break;
        p++;
        n--;
        if ((tag & 0xFFF) == id && (tag & 0x8000)) {
            (p - 1)->tag = 0xFFF;
            func_80010010(obj, id, 2);
            break;
        }
    }
}
