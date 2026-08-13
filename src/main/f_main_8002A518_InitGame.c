#include "../../include/types.h"

extern s32 g_main_8011F668_unk;
extern s32 g_main_8011EEC4_ResidentTableFlag;
extern s32 g_main_8011F564_ObjCount;      /* guess: count for the D_8011EEFC pointer array below */
extern void **g_main_8011EEFC_ObjArray;   /* guess: array of object pointers, stride 0x10 */
extern s32 g_main_8011F63C_unk;
extern s32 g_main_8011EE8C_LastInitMode;
extern void *g_main_8011F538_ResidentEntry; /* single resident-file-table entry, see f_main_8002B3D8_GetResidentFile */
extern s8 g_main_8011F608_OpenStreamCount;
extern s8 g_main_8011F628_unk;
extern s32 g_main_80126358_unk;
extern s32 g_main_8011EEA8_InitResult;
extern s32 g_main_8011F210_unk[]; /* guess: table of 2 stream names, stride 0x10 */

extern s8 g_main_8010904C_MovieOvlName[];   /* "MOVIE.OVL" */
extern s8 g_main_80109030_InitOvlName[];    /* "INIT.OVL" */
extern void *g_main_80158878_ArenaEnd;      /* see f_main_8002B4C4_LoadFile - also doubles as the fixed load address for INIT.OVL */
extern void *g_main_801B92A8_unk;
extern s32 g_main_8012635C_unk[];           /* zeroed in a strided (0x1C) loop down to offset 0 */
extern s8 g_main_8012A574_unk[];

extern s32 f_main_80025F58_SetFlag(s32 val);
extern void func_80025A88(void *addr);
extern void func_80019840(s32);
extern void func_80021538(s32, s32, s32);
extern void func_80012704(s32 flag);
extern void func_80022828(void);
extern void f_main_8002B354_LoadOverlay(s8 *name, u8 mode);
extern void func_800226A0(void);
extern void func_80021850(void);
extern s32 func_80026234(s32 name, void *outHandle); /* guess: same shape as f_main_8002B4C4_LoadFile's open helper, different arg0 type here */
extern void func_8002662C(void *handlePtr);
extern void func_80021404(void);
extern void func_800A4A10(void);
extern void func_800A4A70(void);
extern void func_800F86B0(void);
extern u32 f_main_8002B4C4_LoadFile(s8 *name, void *arg1, s32 mode);
extern void func_8002A050(void);
extern void func_80041C94(void);
extern void f_init_8016749C_Reset(void); /* in INIT.OVL */
extern void func_80066B44(void);
extern void func_8002C7CC(void);
extern void f_init_80167F64_ClearSlots(void); /* in INIT.OVL */
extern void func_8004F36C(void);
extern void *f_main_800F4190_memset(void *dst, s32 fillValue, s32 size);
extern void func_8002C7F8(void);
extern void func_8015D0CC(s32 mode);
extern void func_800A27C4(s32 flag);
extern void f_main_8002BC44_PushState(s32 flag);

/* Game/level bring-up. `mode` (low byte of arg0) selects init path:
     3 - reinit while keeping some object refs alive (clears an "active" flag
         via a small unk28/unk20/unk10 chain over g_main_8011EEFC_ObjArray)
     1 - fresh init: sets up the resident-file table pointing at
         g_main_801B92A8_unk, loads MOVIE.OVL as an overlay, opens up to 2
         streams from the g_main_8011F210 table
     other - alternate init path (no overlay load)
   Always loads INIT.OVL directly into the fixed address
   g_main_80158878_ArenaEnd (mode 0 = load straight into the given buffer,
   no arena bump-allocation - see f_main_8002B4C4_LoadFile). */
void f_main_8002A518_InitGame(s32 arg0) {
    s32 handle[2]; /* opaque FileHandle-shaped buffer, see func_80026234 */
    u8 mode;
    s32 i;
    s32 n;
    s32 *entry;

    g_main_8011F668_unk = 0;
    g_main_8011EEC4_ResidentTableFlag = 0;
    f_main_80025F58_SetFlag(0);
    func_80025A88((void *) 0x801FF000);
    f_main_80025F58_SetFlag(0);

    mode = arg0 & 0xFF;

    if (mode == 3) {
        for (i = 0; i < g_main_8011F564_ObjCount; i++) {
            void *a = ((void **) g_main_8011EEFC_ObjArray)[i * 4];
            void *b = a ? ((void **) a)[2] : 0;   /* +0x8 */
            void *c = b ? ((void **) b)[4] : 0;   /* +0x10 */
            if (c && (((s32 *) c)[10] & 0x400000)) {         /* +0x28 */
                *(s32 *) (((void **) c)[8]) = 0;             /* (+0x20)->unk10 = 0 */
            }
        }
    }

    func_80019840(0);

    if (mode == 1) {
        g_main_8011F63C_unk = 0;
        func_80021538(0x180, 0xF0, 0);
        func_80012704(0);
        func_80022828();

        g_main_8011EEC4_ResidentTableFlag = (s32) PTR_U32(&g_main_8011F538_ResidentEntry);
        g_main_8011F538_ResidentEntry = &g_main_801B92A8_unk;
        if (g_main_8011EE8C_LastInitMode != mode) {
            g_main_8011EE8C_LastInitMode = mode;
        }

        f_main_8002B354_LoadOverlay(g_main_8010904C_MovieOvlName, 0);

        func_800226A0();
        func_80021850();

        entry = g_main_8011F210_unk;
        for (n = 0; n < 2; n++) {
            if (func_80026234(*entry, handle) == 0) {
                g_main_8011F608_OpenStreamCount = n;
                func_8002662C(handle);
                break;
            }
            entry += 4;
        }
    } else {
        func_80012704(0x180);
        func_80021404();
        func_80021404();
        func_800A4A10();
        func_800A4A70();
        func_800F86B0();
    }

    f_main_8002B4C4_LoadFile(g_main_80109030_InitOvlName, g_main_80158878_ArenaEnd, 0);

    func_8002A050();
    func_80041C94();
    f_init_8016749C_Reset();
    func_80066B44();
    func_8002C7CC();
    f_init_80167F64_ClearSlots();
    func_8004F36C();

    if (mode != 3) {
        f_main_800F4190_memset(g_main_8012A574_unk, 0, 0xAB8);
    }

    for (i = 0xAD4; i >= 0; i -= 0x1C) {
        g_main_8012635C_unk[i / 4] = 0;
    }
    g_main_80126358_unk = 0;

    func_8002C7F8();
    g_main_8011F628_unk = 0;
    func_8015D0CC(mode);

    if (mode == 1) {
        f_main_8002BC44_PushState(4);
    } else {
        func_800A27C4(1);
    }

    g_main_8011EEA8_InitResult = -1;
}
