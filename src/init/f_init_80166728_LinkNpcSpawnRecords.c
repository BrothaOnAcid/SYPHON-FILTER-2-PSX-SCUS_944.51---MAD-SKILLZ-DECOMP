#include "../../include/types.h"

extern void *f_main_80025AD0_AllocDown(s32 size);
extern void func_80024094(void *idOut, s32 arg1); /* not yet decompiled: allocates/resolves an id into *idOut */
extern void func_80024D0C(s32 id, MATRIX *mtx);    /* not yet decompiled, guessed sig: sets id's transform */
extern void func_801666F4(s32 id, void *parent);   /* not yet decompiled, guessed sig: links id under parent */

extern AmmoUser *g_main_8012A574_Player;

/* guess: variable-length NPC "spawn" record iterated by
   f_init_80166728_LinkNpcSpawnRecords; unk0 (+0x0) is the byte stride to
   the next record (records aren't fixed-size). `rot` (+0x10, row-major)
   and `pos` (+0x22) feed a stack MATRIX; unk38 (+0x38) is a signed index
   into the batch's id array selecting this record's parent (-1 = use the
   context's fallback). unk3C (+0x3C) is written back with the id resolved
   for this record. */
typedef struct SpawnRecord {
    s32 unk0;       /* +0x00: stride to next record */
    u8 _pad04[0xC];
    s16 rot[3][3];  /* +0x10: row-major rotation, transposed into MATRIX.m */
    s16 pos[3];     /* +0x22: promoted into MATRIX.t */
    u8 _pad28[0x10];
    s16 unk38;      /* +0x38: parent index into this batch's id array, -1 = fallback */
    u8 _pad3A[0x2];
    s32 unk3C;      /* +0x3C: output - id resolved for this record */
} SpawnRecord;

/* guess: entry point object linked here and by f_init_801669CC_AttachChild.
   `count` (+0x4) is the record count, `recOff` (+0x8, x4) offsets the
   record array base (at +0x24) - both read from whichever object ends up
   as `node` below. `hdr` (+0x10) is set to the per-instance header
   allocated here when n != 0 (see f_init_801669CC_AttachChild, which
   checks it to avoid re-linking). */
typedef struct {
    u8 _pad00[0x4];
    s32 count;   /* +0x4 */
    s32 recOff;  /* +0x8 */
    u8 _padC[0x4];
    void *hdr;   /* +0x10 */
} SpawnDef;

/* guess: owner/context object. `fallback` (+0xC) is read once; when it's
   NULL, `node` below is swapped for a global default template instead of
   using `def` directly. `idArray`/`sizeArray` (+0x18/+0x24) receive the
   allocated per-instance arrays in "direct" (n == 0) mode; in "instanced"
   (n != 0) mode the same pair is stashed in a fresh header inside `table`
   instead (see f_init_80166E54_LoadNpcHogAndAllocSlots's n=5 call). */
typedef struct {
    u8 _pad00[0xC];
    void *fallback; /* +0xC */
    u8 _pad10[0x8];
    void *idArray;  /* +0x18 */
    u8 _pad1C[0x4];
    void *sizeArray; /* +0x24 */
} SpawnCtx;

/* Heavily unconfirmed. Runs the record-processing pass below once if
   n == 0, otherwise once per instance (n times), allocating a fresh id
   array (and, in instanced mode, an id-count array) each time. Per record:
   builds a MATRIX from the record's rotation/position and resolves+stores
   an id for it via func_80024094/func_80024D0C, then in a second pass
   links each record's id under its resolved parent via func_801666F4. */
void f_init_80166728_LinkNpcSpawnRecords(SpawnDef *def, SpawnCtx *ctx, s32 n) {
    void *hdr = f_main_80025AD0_AllocDown(8);
    u8 *table = (u8 *) f_main_80025AD0_AllocDown(n * 0x3C);
    void *fallback = ctx->fallback;
    SpawnDef *node = def;
    s32 inst = 0;

    if (n != 0) {
        *(s32 *) hdr = n;
        *(void **) ((u8 *) hdr + 4) = table;
        def->hdr = hdr;
    }

    if (fallback == 0) {
        /* guess: fall back to the current player's default spawn template
           when the context has no explicit link target yet. */
        void *p1 = *(void **) ((u8 *) g_main_8012A574_Player + 0x8);
        void *p2 = *(void **) ((u8 *) p1 + 0x10);
        node = *(SpawnDef **) ((u8 *) p2 + 0x20);
        func_80024094(&fallback, 0);
    }

    do {
        s32 *ids = (s32 *) f_main_80025AD0_AllocDown(node->count * 4);

        if (n != 0) {
            u8 *slot = table + inst * 0x3C;
            *(s32 *) slot = 0;
            *(s32 **) (slot + 4) = ids;
            *(void **) (slot + 8) = f_main_80025AD0_AllocDown(node->count * 8);
        } else {
            ctx->idArray = ids;
            ctx->sizeArray = f_main_80025AD0_AllocDown(node->count * 8);
        }

        if (node->count > 0) {
            SpawnRecord *rec = (SpawnRecord *) ((u8 *) node + node->recOff * 4 + 0x24);
            s32 i;
            s32 *idp = ids;

            for (i = 0; i < node->count; i++) {
                MATRIX mtx;
                s32 x, y;

                for (y = 0; y < 3; y++)
                    for (x = 0; x < 3; x++)
                        mtx.m[x][y] = rec->rot[y][x];
                mtx.t[0] = rec->pos[0];
                mtx.t[1] = rec->pos[1];
                mtx.t[2] = rec->pos[2];

                func_80024094(idp, 0);
                rec->unk3C = *idp;
                func_80024D0C(*idp, &mtx);

                idp++;
                rec = (SpawnRecord *) ((u8 *) rec + rec->unk0);
            }
        }

        if (node->count > 0) {
            SpawnRecord *rec = (SpawnRecord *) ((u8 *) node + node->recOff * 4 + 0x24);
            s32 i;
            s32 *idp = ids;

            for (i = 0; i < node->count; i++) {
                void *parent = (rec->unk38 >= 0) ? (void *) (u64) ids[rec->unk38] : fallback;

                func_801666F4(*idp, parent);

                idp++;
                rec = (SpawnRecord *) ((u8 *) rec + rec->unk0);
            }
        }

        inst++;
    } while (n != 0 ? inst < n : inst <= 0);
}
