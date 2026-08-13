#include "../../include/types.h"

extern u8 D_80114678[]; /* not yet decompiled: 3-entry group-list table, see f_init_80165FA8_ProcessGroupList */

extern s32 func_80026D04(void **ppTable);                       /* not yet decompiled */
extern void func_80026D28(void **ppTable, s32 index, void *out); /* not yet decompiled */
extern s32 func_80025708(void *entry);                          /* not yet decompiled */
extern s32 func_8002576C(void *entry);                          /* not yet decompiled */
extern s32 func_800F8248(const char *name, const char *match);   /* not yet decompiled */
extern u8 f_init_80165F00_ScanEntryTable(s32 arg0);
extern u8 f_init_80165FA8_ProcessGroupList(s32 arg0, s16 count, void *arg2);
extern void f_init_80166100_RegisterEntries(s32 arg0);

extern u8 D_8016A408; /* not yet decompiled: "INTRFACE" resource table */
extern u8 D_80190418; /* not yet decompiled: "SPFX" resource table */

/* Called with a path/name string (e.g. "\COMMON\INTRFACE" or
   "\COMMON\SPFX"). name[8] ('I' vs 'S') quickly picks the matching resource
   table, whose entries are all touched via func_80025708/func_8002576C.
   Then the name is compared for real (func_800F8248) against the two known
   paths to decide the follow-up: INTRFACE runs f_init_80165F00_ScanEntryTable,
   SPFX runs f_init_80165FA8_ProcessGroupList (count 3, D_80114678 list) followed
   by f_init_80166100_RegisterEntries. Neither match does nothing further. */
void f_init_8015C7E8_ProcessResourceGroup(const char *name) {
    void *table;
    u8 entry[0x30];
    s32 count;
    s32 i;

    if (name[8] == 'I') {
        table = &D_8016A408;
    } else {
        table = &D_80190418;
    }

    count = func_80026D04(&table);
    for (i = 0; i < count; i++) {
        func_80026D28(&table, i, entry);
        func_80025708(entry);
        func_8002576C(entry);
    }

    if (func_800F8248(name, "\\COMMON\\INTRFACE") == 0) {
        f_init_80165F00_ScanEntryTable(PTR_U32(table));
        return;
    }

    if (func_800F8248(name, "\\COMMON\\SPFX") == 0) {
        f_init_80165FA8_ProcessGroupList(PTR_U32(table), 3, D_80114678);
        f_init_80166100_RegisterEntries(PTR_U32(table));
    }
}
