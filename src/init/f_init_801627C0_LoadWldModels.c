#include "../../include/types.h"

extern s16 g_main_8012B02C_LevelId;
extern s8 *g_main_8010C874_LevelNames[]; /* per-level dir name table */
extern s8 g_main_80136B8C_PathBuf[];     /* guess: shared scratch path buffer,
    reused across the various "\<dir>\<name>;1" builders (like the stack
    `path[]` locals elsewhere, but this one is a persistent global) */

extern u8 *g_main_8011F6E0_LevelInfo; /* guess: current level/stage descriptor
    block; only a handful of offsets are understood here:
    +0x78..+0x87: fixed 16-entry list of WLDEMD model indices this level
      references directly (0xFF-terminated)
    +0x90 + i*0x10 (i = model index): per-model dependency group, up to 14
      (0xFF-terminated) model indices whose combined streaming size is
      summed to size the clip pool below */

extern u32 g_main_8011F634_LoadStage;      /* guess: level-load progress stage */
extern s32 g_main_8011F98C_ClipSectors;    /* guess: sectors per streaming clip (from arg n) */
extern s32 g_main_8011F660_ModelCount;
extern WldModelSlot *g_main_8011F6C8_ModelSlots;     /* [ModelCount], stride 0x10 */
extern WldModelHandle *g_main_8011F680_ModelHandles; /* [ModelCount], stride 0x40 */
extern WldToc *g_main_8011F978_WldToc;
extern u8 g_main_8012093D_ModelInUseFlags[]; /* guess: per-model "in use this frame" flags */

extern s32 g_main_8011F96C_ClipSlotCount;  /* total clip slots allocated */
extern s32 g_main_8011F970_ClipsFreeA;     /* guess: free-clip counter A */
extern s32 g_main_8011F980_ClipsFreeB;     /* guess: free-clip counter B */
extern WldClipSlot *g_main_8011F968_ClipSlots;
extern s32 g_main_8011F97C_ClipStreamBase; /* running streaming-buffer cursor */
extern WldStreamCtx *g_main_8011F598_StreamCtx;

extern u8 D_8010C3B4[]; /* unexamined, passed straight through to f_init_80166C08_CreateWldRes as `tag` */

/* --- libc-style / unexamined externs --- */
extern void func_800F045C(void *pathBuf, const char *fmt, s8 *dir); /* sprintf */
extern FileHandle *func_80025F64(void *out, s8 *path); /* guess: open a stream by path; `out` is a small scratch out-param, contents unconfirmed */
extern s32 func_800F836C(FileHandle *handle); /* stream/decompression context setup, see f_main_8002608C_OpenFog */
extern s32 f_main_80026B80_ReadToc(s8 *path, s32 flags, WldToc **outToc);
extern void *f_main_80025AD0_AllocDown(s32 size);
extern void func_800F8268(s32 sector, u32 *out); /* guess: resolves/queues sector info into *out */
extern s8 *f_main_800F8238_strchr(const s8 *s, s32 c); /* PSX BIOS trampoline, A0 index 0x1E (index/strchr family) */
extern s32 func_800F9244(const s8 *s, s32 unused); /* guess: strlen-like scan over the names blob; 2nd arg appears dead (leftover register) */
extern s32 f_init_80166C08_CreateWldRes(s32 key, s32 tag, s32 flags, WldRes **out);
extern void func_800A2370(void *out, u8 id, s32 flag, u8 prevId, u8 selfId); /* 5th arg is a stack arg m2c didn't surface; out is a small local scratch buffer, contents unconfirmed */
extern void func_800A3690(u8 id);
extern void f_main_8002B354_LoadOverlay(s8 *name, u8 param);

/* Loads WLDEMD.HOG ("\<dir>\WLDEMD.HOG;1", environment models) for the
   current level (g_main_8012B02C_LevelId) and builds the runtime model/clip
   tables used by the streaming system:
   - opens the archive, parses its TOC (g_main_8011F978_WldToc) and fills one
     WldModelSlot per entry (name, sector-rounded size, index) plus a
     WldModelHandle slot that gets its resource loaded via f_init_80166C08_CreateWldRes,
     tagging "referenced by this level" / "type '8'/'9'" models with
     WldTypeDef.flags28 bits.
   - sizes a pool of streaming clip slots (g_main_8011F968_ClipSlots) to the
     worst-case simultaneous size of any one model's dependency group
     (g_main_8011F6E0_LevelInfo + 0x90 + i*0x10), then lays out their stream
     buffer offsets from g_main_8011F598_StreamCtx.
   - if `attach`, walks g_main_8011F598_StreamCtx's attach list and assigns
     clip slots to those models up front, deduping consecutive repeats and
     writing resolved data pointers back into each model's WldTypeDef.
   - clears the current overlay-name cache at the end (LoadOverlay(0, 0)). */
void f_init_801627C0_LoadWldModels(s32 n, u8 attach) {
    u8 openOut[8];   /* guess: out-param scratch for func_80025F64, unconfirmed shape */
    u8 flushOut[8];  /* guess: out-param scratch for func_800A2370, unconfirmed shape */
    FileHandle *h;
    s32 sector;
    WldToc *toc;
    s32 *bounds;
    s8 *name;
    s8 *dot;
    WldModelSlot *slot;
    WldModelHandle *hnd;
    u8 *p, *end;
    s32 off, boundEnd, rawLen;
    s8 type;
    u8 found;
    u8 i, b;
    s32 total, worst;
    s32 j, k;
    u8 prevId, run;

    g_main_8011F634_LoadStage = 5;
    g_main_8011F98C_ClipSectors = n;

    func_800F045C(g_main_80136B8C_PathBuf, "\\%s\\WLDEMD.HOG;1",
                  g_main_8010C874_LevelNames[g_main_8012B02C_LevelId]);

    h = func_80025F64(openOut, g_main_80136B8C_PathBuf);
    sector = func_800F836C(h);

    f_main_80026B80_ReadToc(g_main_80136B8C_PathBuf, 0, &toc);
    g_main_8011F978_WldToc = toc;

    g_main_8011F660_ModelCount = toc->count;
    g_main_8011F6C8_ModelSlots = f_main_80025AD0_AllocDown(toc->count * sizeof(WldModelSlot));
    g_main_8011F680_ModelHandles = f_main_80025AD0_AllocDown(toc->count * sizeof(WldModelHandle));

    for (i = 0; i < g_main_8011F660_ModelCount; i++) {
        g_main_8012093D_ModelInUseFlags[i] = 0;
    }

    bounds = (s32 *) ((u8 *) toc + toc->recordsOffset);
    name = (s8 *) ((u8 *) toc + toc->namesOffset);

    for (i = 0; i < g_main_8011F660_ModelCount; i++) {
        found = 0;
        slot = &g_main_8011F6C8_ModelSlots[i];
        off = bounds[i];
        boundEnd = (i == g_main_8011F660_ModelCount - 1)
            ? h->size - toc->dataOffset
            : bounds[i + 1];

        func_800F8268(sector + ((u32) (off + toc->dataOffset) >> 11), &slot->sectorInfo);
        rawLen = boundEnd - (off + 1);

        p = g_main_8011F6E0_LevelInfo + 0x78;
        end = p + 0x10;

        slot->name = name;
        slot->len = rawLen;
        slot->index = i;
        slot->index2 = i;
        slot->unk08 = ((rawLen / (u32) (g_main_8011F98C_ClipSectors << 11)) + 1) * g_main_8011F98C_ClipSectors;
        type = *name;

        for (b = *p; b != 0xFF; ) {
            p++;
            if (b == i) {
                found = 1;
                break;
            }
            if (p >= end)
                break;
            b = *p;
        }
        /* advance past this record's NUL-terminated name in the names blob */
        name += func_800F9244(name, (s32) PTR_U32(end)) + 1;

        dot = f_main_800F8238_strchr(slot->name, '.');
        if (dot != 0)
            *dot = 0;

        hnd = &g_main_8011F680_ModelHandles[i];
        f_init_80166C08_CreateWldRes(-1, (s32) PTR_U32(D_8010C3B4), 0x01000000, &hnd->res);
        hnd->res->flagsB |= 0x40;

        if (found) {
            hnd->res->typeDef->flags28 |= 0x20000;
        }
        if ((u32) (type - 0x38) < 2) {
            hnd->res->typeDef->flags28 |= (type == 0x39) ? 0x200000 : 0x100000;
        }
    }

    /* size the clip pool to the worst-case model dependency group */
    worst = 0;
    for (i = 0; i < g_main_8011F660_ModelCount; i++) {
        u8 *list = g_main_8011F6E0_LevelInfo + 0x90 + i * 0x10;

        total = g_main_8011F6C8_ModelSlots[i].unk08 / g_main_8011F98C_ClipSectors;
        for (j = 0; j < 0xE; j++) {
            b = list[j];
            if (b == 0xFF)
                break;
            if (b != 0xFE) {
                total += g_main_8011F6C8_ModelSlots[b].unk08 / g_main_8011F98C_ClipSectors;
            }
        }
        if (worst < total)
            worst = total;
    }

    g_main_8011F96C_ClipSlotCount = worst;
    g_main_8011F970_ClipsFreeA = worst;
    g_main_8011F980_ClipsFreeB = worst;
    g_main_8011F968_ClipSlots = f_main_80025AD0_AllocDown(worst * sizeof(WldClipSlot));
    g_main_8011F97C_ClipStreamBase = g_main_8011F598_StreamCtx->unk30 + g_main_8011F598_StreamCtx->unk08;

    for (j = 0; j < worst; j++) {
        WldClipSlot *c = &g_main_8011F968_ClipSlots[j];
        c->base = g_main_8011F97C_ClipStreamBase;
        g_main_8011F97C_ClipStreamBase += n << 11;
        c->unk04 = 0;
        c->unk08 = 0;
        c->modelIndex = -1;
        c->free = 1;
    }

    prevId = 0xFE;
    if (attach) {
        WldStreamCtx *ctx = g_main_8011F598_StreamCtx;
        run = 0;
        if (ctx->attachCount != 0) {
            for (k = 0; k < (s32) ctx->attachCount; k++) {
                u8 id = ctx->attachList[k];
                WldClipSlot *c = &g_main_8011F968_ClipSlots[k];

                c->modelIndex = id;
                g_main_8011F970_ClipsFreeA -= 1;
                c->unk08 = (void *) &g_main_8011F6C8_ModelSlots[id];
                g_main_8011F980_ClipsFreeB -= 1;
                c->unk04 = (s32) PTR_U32(g_main_8011F680_ModelHandles[id].res);

                if (id != prevId) {
                    run = 0;
                    if (prevId != 0xFE) {
                        func_800A2370(flushOut, id, 0, prevId, prevId);
                        func_800A3690(prevId);
                    }
                    prevId = id;
                    g_main_8011F680_ModelHandles[id].res->typeDef->dataPtr =
                        c->base & 0x7FFFFFFF;
                } else {
                    run += 1;
                }
                /* guess: dedups consecutive repeats of the same model id into
                   a run-indexed array of extra clip base pointers */
                g_main_8011F680_ModelHandles[id].subRes[run] = (void *) (u64) (u32) c->base;
            }
        }
        /* flush the last pending group */
        func_800A2370(flushOut, prevId, 0, prevId, prevId);
        func_800A3690(prevId);
    }

    f_main_8002B354_LoadOverlay(0, 0);
}
