#include "../../include/types.h"

/* Guessed: p0 is some kind of node/object with a "parent"-shaped pointer
   chain at offset 0x20 (unconfirmed struct - kept generic/untyped here). */
extern void func_80025130(void *p0, void *chain, s32 p1);

/* Walks p0->0x20->0x20 (if p0->0x20 is non-NULL) and passes that, plus p0
   and p1, to func_80025130. */
void f_main_800251FC_CallWithParentChain(void **p0, s32 p1) {
    void **parent = (void **) p0[8]; /* +0x20 */
    void *chain = parent ? parent[8] : 0; /* parent->+0x20 */

    func_80025130(p0, chain, p1);
}
