#ifndef TYPES_H
#define TYPES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef float f32;

/* PSX pointers are 32-bit; on our (64-bit) host, a direct pointer->u32 cast
   warns (-Wpointer-to-int-cast). Route through u64 to silence it - this is
   purely a decompilation-host artifact, harmless on the real 32-bit target. */
#define PTR_U32(p) ((u32) (u64) (p))

/* Sign-extend a `bits`-wide (1..32) value pulled from a u32 */
#define SEXT(v, bits) ((s32) ((u32) (v) << (32 - (bits))) >> (32 - (bits)))

/* Fixed-point multiply (12-bit fractional): matches the MIPS multu/mflo+sra
   sequence - low 32 bits of the product, then arithmetic shift right 12. */
#define FIX12(a, b) ((s32) ((u32) (a) * (u32) (b)) >> 12)

/* Guessed per-node transform matrix produced by func_8001C264 (3x3 s16
   rotation at +0x0) and written by f_main_8001C4F4_ReadKeyframes (s32
   translation at +0x14, +0x2C "rebuild" flag). */
typedef struct {
    s16 rot[9];    /* +0x00: 3x3 rotation matrix */
    s32 tx;        /* +0x14 */
    s32 ty;        /* +0x18 */
    s32 tz;        /* +0x1C */
    u8 _pad20[0xC]; /* +0x20..+0x2B: unknown */
    u32 flag;      /* +0x2C: guess: matrix-rebuild flag */
} HmdMatrix;

/* Guessed HAN animation decoder context (f_main_8001C4F4_ReadKeyframes):
   parallel per-channel arrays - node pointers (+0x18, stride 4) and 3-s16
   angle accumulators (+0x24, stride 8). Only these two fields are confirmed. */
typedef struct {
    u8 _pad00[0x18];
    void **nodes;   /* +0x18: per-channel node pointers */
    u8 _pad1C[0x8];
    s16 *angles;    /* +0x24: per-channel 3-s16 angle state (stride 8) */
} HanCtx;

/* Guessed viewport/rect-like struct (see f_init_8015B920_ApplyLevelParam,
   f_init_80164A84_ResetEntries); +0x4C/0x4E/0x50 (u16/s16) confirmed. */
typedef struct {
    u8 _pad00[0x4C];
    u16 unk4C;   /* +0x4C */
    s16 unk4E;   /* +0x4E */
    s16 unk50;   /* +0x50 */
} ViewportRect;

/* PSYQ libgte VECTOR: 3 x 32-bit fixed point components */
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} VECTOR;

/* Guessed from f_main_800263C4_GetFileSize (only field usage seen so far).
   Passed around by the file-loading code (func_80026234/80026414/8002662C)
   as an opaque "handle" - actual size/layout unconfirmed. */
typedef struct {
    s32 unk00;  /* guess: BIOS file descriptor / handle id */
    s32 size;   /* guess: raw file size in bytes */
    s32 mode;   /* guess: <0 = exact byte size, >=0 = round size up to a 2048-byte CD sector */
    s32 unk0C;  /* +0xC: guess: read-cursor copy of unk00, set by f_main_800265A8_PrepareRead */
    s32 unk10;  /* +0x10: guess: read-cursor copy of size, set by f_main_800265A8_PrepareRead */
} FileHandle;

/* Guessed from f_main_80026E00_FindResource. Matches knowledge.txt's ".HOG"
   description (resource archive with a filename TOC): a named-entry lookup
   table, all fields are byte offsets relative to the HogToc itself. */
typedef struct {
    s32 unk00;         /* guess: unused by lookup, maybe a magic/id */
    s32 count;         /* number of named entries */
    s32 valuesOffset;  /* offset to an s32[count] array of per-entry values */
    s32 namesOffset;   /* offset to a blob of concatenated NUL-terminated filenames, in entry order */
    s32 dataOffset;    /* offset added to array[i] to get entry i's resource data pointer */
} HogToc;

/* Guessed wrapper: offset 0 holds the actual HogToc pointer (NULL if the
   archive isn't loaded yet). */
typedef struct {
    HogToc *toc;
} HogArchive;

/* Guessed from f_main_80025D3C_RemoveNode/f_main_80025DE8_FreeList: generic
   intrusive doubly-linked list node, used pervasively (27+ call sites). */
typedef struct ListNode {
    void *owner;             /* guess: payload/owner pointer, untouched by list ops */
    struct ListNode *prev;   /* +0x4 */
    struct ListNode *next;   /* +0x8 */
} ListNode;

/* Guessed: a list head is just a pointer to the first node. */
typedef struct {
    ListNode *first;
} ListHead;

/* Guessed: current-angle block (AnimOwner +0x0C). Angles are s16 at +0x4,
   +0xA and +0x10 (stride 6). */
typedef struct {
    u32 unk00;       /* +0x00 */
    s16 angX;        /* +0x04 */
    s16 unk06;       /* +0x06 */
    u16 unk08;       /* +0x08 */
    s16 angY;        /* +0x0A */
    u16 unk0C;       /* +0x0C */
    u16 unk0E;       /* +0x0E */
    s16 angZ;        /* +0x10 */
    u16 unk12;       /* +0x12 */
    s32 unk14;       /* +0x14: guess: position/direction x, read by f_main_80065F90 */
    s32 unk18;       /* +0x18: guess: position/direction y (negated by f_main_80065F90) */
    s32 unk1C;       /* +0x1C: guess: position/direction z */
    void *unk20;     /* +0x20: guess: optional per-frame callback trigger, see f_main_80068DA4 */
} AngBlock;

/* Guessed character/object that owns animation "action" entries (chained on
   `entries` +0x20). Update is gated on `anim18`/`extra24` being non-null; the
   wrapper func_8001A740 also skips when bit 3 of `flags08` is set or the +0x10
   def's +0x28 flags carry the 0x400000/0x2000000 bits. Only the +0x10/+0x18/
   +0x20/+0x24/+0x8 fields are confirmed (by f_main_8001A7BC_UpdateAnimEntries,
   func_8001A740 and the 0x80012470/0x800434C0 call sites). */
typedef struct {
    u8 _pad00[0x8];
    u8 flags08;          /* +0x08: bit 3 = skip anim update */
    u8 _pad09;
    u8 flags0A;          /* +0x0A: guess: bit 2 gates f_main_80069054's +0x70/74/78 accumulation */
    u8 _pad0B;
    AngBlock *unk0C;     /* +0x0C: current-angle block */
    void *def;           /* +0x10: def block; +0x28 flags gate the wrapper */
    u8 _pad14[0x4];
    void *anim18;        /* +0x18: non-null required to update */
    u8 _pad1C[0x4];
    ListNode *entries;   /* +0x20: AnimEntry chain */
    void *extra24;       /* +0x24: non-null required to update */
} AnimOwner;

/* Guessed animation "action" entry owned by an AnimOwner. HAN data pointer is
   `anim` (+0x10); `id` (+0x28) selects the update pass in
   f_main_8001A7BC_UpdateAnimEntries (0 or 0x4000 = first pass, 0x8000 = second).
   The remaining fields are only read by func_8001A8A8 (per-entry anim reader,
   see f_main_8001C4F4_ReadKeyframes). */
typedef struct {
    u8 _pad00[0x4];
    s16 unk04;           /* +0x04: param passed to func_80015FF0 */
    s16 unk06;           /* +0x06 */
    u32 flags08;         /* +0x08: 0x10000000 = follow +0x0C pointer */
    void *sub;           /* +0x0C: sub-anim pointer (followed when flags08 bit set) */
    void *anim;          /* +0x10: HAN anim data pointer */
    u32 unk14;           /* +0x14: current anim id byte (word-stored) */
    u8 _pad18[0x4];
    void (*callback)(void *owner, u32 a1, u32 a2); /* +0x1C: anim-event callback */
    u32 flags20;         /* +0x20: 0x4000000 / 0x10000000 bits tested */
    u32 unk24;           /* +0x24: passed to callback as a2 */
    u32 id;              /* +0x28 */
    u32 count;           /* +0x2C: loop counter (decremented by func_8001A8A8) */
} AnimEntry;

/* Guessed actor node in the g_main_8011F160_ActorList linked list (chained
   through core->next, +0x18C). */
typedef struct ActorAction ActorAction;
typedef struct ActorCore ActorCore;

typedef struct ActorNode {
    u8 _pad00[0x8];
    AnimOwner *anim;        /* +0x08: animation/action owner */
    ActorCore *core;        /* +0x0C */
    s32 unk10;               /* +0x10: guess: optional per-frame callback trigger, see f_main_800691FC */
} ActorNode;

/* Guessed: active action/state of an actor (ActorCore +0x158). */
typedef struct ActorAction {
    u8 _pad00[0x8];
    s32 pendX;      /* +0x08: guess: pending direction vector, see f_main_800695CC_SetActorDir */
    s32 pendY;      /* +0x0C */
    s32 pendZ;      /* +0x10 */
    s32 pendW;      /* +0x14: guess: only set on the copy path, see f_main_800695CC_SetActorDir */
    s32 unk18;      /* +0x18: recoil direction vector (written by f_main_8006C730) */
    s32 unk1C;      /* +0x1C */
    s32 unk20;      /* +0x20 */
    s32 unk24;      /* +0x24 */
    s32 dirX;       /* +0x28: guess: active direction vector, copied from pendX/Y/Z/W */
    s32 dirY;       /* +0x2C */
    s32 dirZ;       /* +0x30 */
    s32 dirW;       /* +0x34 */
    s32 unk38;      /* +0x38: compared to 6 (reload state?); also written directly by f_main_800695CC_SetActorDir */
    s32 unk3C;      /* +0x3C: guess: previous value of unk38 */
    u32 flags40;    /* +0x40: bit 0x10 = recoil pending */
} ActorAction;

/* Guessed per-actor block. `next` (+0x18C) links the actor list; nodes are
   added by func_80066B54 (also sets +0x104 bit 0x1000) and removed by
   func_80066BE0. */
typedef struct ActorCore {
    s32 rawX;                /* +0x00: guess: raw position/orientation x, read directly by f_main_80065F90 */
    s32 rawY;                /* +0x04 */
    s32 rawZ;                /* +0x08 */
    s32 rawW;                /* +0x0C */
    u8 _pad10[0x20 - 0x10];
    s32 unk20;                /* +0x20: guess: bounds-check coordinate, see f_main_80066270_IsInBounds */
    s32 unk24;                /* +0x24 */
    s32 unk28;                /* +0x28 */
    u8 _pad2C[0x50 - 0x2C];
    s32 posX;               /* +0x50: guess: world position, integrated by f_main_8006852C_IntegratePositions */
    s32 posY;               /* +0x54 */
    s32 posZ;                /* +0x58 */
    u8 _pad5C[0x60 - 0x5C];
    s32 velX;                /* +0x60: guess: velocity added into posX/Y/Z */
    s32 velY;                /* +0x64 */
    s32 velZ;                /* +0x68 */
    u8 _pad6C[0x70 - 0x6C];
    s32 unk70;                /* +0x70: guess: accumulator, += g_main_8010DD34_unk, see f_main_80069054 */
    s32 unk74;                /* +0x74: guess: accumulator, += g_main_8010DD38_unk */
    s32 unk78;                /* +0x78: guess: accumulator, += g_main_8010DD3C_unk */
    u8 _pad7C[0xC0 - 0x7C];
    s32 unkC0;              /* +0xC0: recoil rotation, written by f_main_8006C730 */
    s32 unkC4;              /* +0xC4 */
    s32 unkC8;              /* +0xC8 */
    s32 unkCC;              /* +0xCC */
    s32 pos2X;               /* +0xD0: guess: secondary position (e.g. attach point), see f_main_8006852C_IntegratePositions */
    s32 pos2Y;               /* +0xD4 */
    s32 pos2Z;               /* +0xD8 */
    u8 _padDC[0xE0 - 0xDC];
    s32 vel2X;                /* +0xE0: guess: velocity added into pos2X/Y/Z */
    s32 vel2Y;                /* +0xE4 */
    s32 vel2Z;                /* +0xE8 */
    u8 _padEC[0x102 - 0xEC];
    u8 unk102;                /* +0x102: guess: state byte, 5 = "skip" for f_main_80068EB8 */
    u8 unk103;                /* +0x103: guess: expiry counter threshold, see f_main_8006924C. 0 = disabled */
    u32 flags104;           /* +0x104: 0x1000 = active, 0x800 cleared each frame; low byte
                                doubles as an expiry counter vs. unk103, see f_main_8006924C */
    u8 _pad108[0x50 - 0x4];
    u32 flags154;           /* +0x154: guess: 0x4000000 gates f_main_80050240 */
    ActorAction *act;       /* +0x158: active action */
    s32 unk15C;              /* +0x15C: guess: gate flag, see f_main_80069054 */
    s32 unk160;              /* +0x160: guess: optional per-frame callback trigger, see f_main_80068E64 */
    u8 _pad164[0x184 - 0x164];
    s32 unk184;               /* +0x184: guess: gate flag, see f_main_80069054 */
    u8 _pad188[0x18C - 0x188];
    ActorNode *next;        /* +0x18C */
} ActorCore;

/* Guessed result struct produced by f_main_80031838_GetSlotState, consumed by
   func_8002EFF0 (copies words 0..0xC into a weapon slot entry at +0x74..0x80)
   and by func_80032C38. Only the first 3 words are written by the producer;
   consumers also read the 4th (unk0C), which the producer leaves untouched. */
typedef struct {
    u32 unk00;  /* guess: always zero */
    u32 val;    /* guess: slot state/damage-like value (0, 0x46, or a table value) */
    u32 unk08;  /* guess: always zero */
    u32 unk0C;  /* guess: untouched by producer (uninitialized) */
} SlotState;

/* Guessed weapon ammo-type definition: 0x24-byte records in
   g_main_8012F630_AmmoDefs, indexed by ammo type. Only +0x00..+0x08 are
   confirmed (by f_main_800644AC_HandleAmmo); the individual symbols
   D_8012F631/633/636/637/638 are labels at base+1/+3/+6/+7/+8. */
typedef struct {
    u8 reserve;    /* +0x00: default reserve-ammo refill amount */
    u8 spend;      /* +0x01: != 0 = firing spends ammo (decrements current) */
    u8 unk02;      /* +0x02 */
    u8 flags03;    /* +0x03: low 3 bits tested */
    u8 unk04;      /* +0x04 */
    u8 unk05;      /* +0x05 */
    s8 unk06;      /* +0x06: >= 0 -> spawn a projectile/effect, else skip */
    s8 unk07;      /* +0x07: -2 = special "unlimited" handling */
    s16 unk08;     /* +0x08 */
    u8 unk0A[0x1A]; /* +0x0A..+0x23: unconfirmed */
} AmmoDef;

/* Ammo slot: maximum at +0x00, current at +0x02. */
typedef struct {
    u16 max;       /* +0x00 */
    u16 current;   /* +0x02 */
} AmmoSlot;

/* Guessed weapon definition table g_main_8011EEF8_WeaponDefs (0x4C-byte
   records indexed by weapon id); +0x24 selects the ammo type. +0x28 carries
   a per-id "type" used by the music sequencer too. */
typedef struct {
    u8 _pad00[0x24];
    u16 ammoType;   /* +0x24: ammo type index (low 6 bits used) */
    s16 unk28;      /* +0x28: per-id type (music module: track type) */
    u8 unk2A;       /* +0x2A: mode flag, 0x24 = special-case in f_init_80161E28_UpdateWeaponTrack */
    u8 _pad2B[0x30 - 0x2B];
    s32 unk30;      /* +0x30: index into g_main_8011EEFC_ObjArray, -1 = none */
    u8 _pad34[0x4A - 0x34];
    s16 unk4A;      /* +0x4A: written by f_init_8015E9C0_SyncTrackParams (truncated s32 result) */
} WeaponDef;

/* Guessed per-weapon block (AmmoUser +0x1C -> +0x08). */
typedef struct {
    u8 _pad00[0x30];
    u32 unk30;      /* +0x30: reload/anim value (0x3E or 0x55) */
    u8 unk34;       /* +0x34 */
    u8 _pad35[0x5];
    u8 unk3A;       /* +0x3A: reserve-ammo counter (refilled from AmmoDef.reserve) */
} PerWeapon;

/* Guessed weapon-holder/character object used by f_main_800644AC_HandleAmmo.
   `core` (+0xC) is the actor core whose +0x158 action's +0x38 drives the
   reload state; `flags` (+0x10) is a flags bitmap whose low 2 bits of the
   pointer select sub-indices and whose byte +0xC holds a mode flag; `hud`
   (+0x20) is the ammo/hud block (ammo slots at +0x44, stride 4). */
typedef struct {
    u8 _pad00[0x2];
    s16 weaponId;   /* +0x02: current weapon id */
    u8 _pad04[0x8];
    ActorCore *core; /* +0x0C: actor core (act at +0x158) */
    u8 *flags;      /* +0x10: flags bitmap */
    u8 _pad14[0x4];
    void *unk18;    /* +0x18: -> +0x8 = s16 counter, clamped to 0x7FFF when > 0 */
    void *unk1C;    /* +0x1C: -> +0x8 = PerWeapon, +0xC = projectile word,
                        +0x4 = flags word, +0x10 = s16 */
    void *hud;      /* +0x20: ammo/hud block */
} AmmoUser;

/* Music-sequencer flags block g_main_8011F374_SongFlags (8011F374).
   `changed` is set when a new song id is lower than the current one. */
typedef struct {
    u8 changed;     /* +0x00 (8011F374) */
    u8 _pad01;
    u16 count;      /* +0x02 (8011F376): number of registered tracks */
} SongFlags;

/* Guessed song/sequence request object passed to f_main_800C2CD4_RegisterTrack.
   `owner` (+0x08) holds a pointer whose +0x10 is the track object; +0x27 is a
   flags byte with bit 1 (0x02) set by the sequencer. */
typedef struct {
    u8 _pad00[0x2];
    s16 songId;     /* +0x02 */
    u8 _pad04[0x4];
    void *owner;    /* +0x08: -> +0x10 = TrackObj */
    u8 _pad0C[0x1B];
    u8 flags27;     /* +0x27: bit 0x02 = registered */
} SongReq;

/* Guessed registered-track object (SongReq.owner->0x10). */
typedef struct {
    u8 _pad00[0x28];
    u32 flags28;    /* +0x28: 0x80000 = slotted, 0x40000 = registered */
} TrackObj;

/* Guessed 0x48-byte channel record of the table at 0x801412C0
   (g_main_801412C0_Rec48, 24 entries, one per SPU voice). */
typedef void (*ChannelCb)(s32 channel, u32 arg, s32 cmd);
typedef struct {
    u32 unk00;      /* +0x00: channel state (1 = active) */
    u32 unk04;      /* +0x04 */
    u32 unk08;      /* +0x08: countdown timer (ticked by func_800FD38C) */
    u8 _pad0C[0x0];
    s16 unk0C;      /* +0x0C: raw pan-ish value, see f_main_800FBFF0_ReapplyPan */
    s16 unk0E;      /* +0x0E: raw pan-ish value */
    s16 unk10;      /* +0x10: threshold value, see f_main_800FD610_MuteIdleChannels */
    u8 _pad12[0xE];
    u32 unk20;      /* +0x20: flags, bit 1 = "temporarily muted" (see
                        f_main_800FD610_MuteIdleChannels /
                        f_main_800FD6A4_RestoreTaggedChannels) */
    u8 _pad24[0x4];
    u32 unk28;      /* +0x28: voice id (0x7F = special) */
    u8 _pad2C[0x4];
    u32 unk30;      /* +0x30: logical channel/param index, see f_main_800FBFF0_ReapplyPan */
    u8 _pad34[0xC];
    u32 unk40;      /* +0x40 */
    ChannelCb unk44; /* +0x44: channel callback (cmd 2/3) */
} Rec48;

/* Guessed node type chained onto AudioSeqState.activeHead (+0x24); only the
   +0xA4 "next" link is confirmed (by f_main_800FDC04_AppendActiveNode). Not
   the same object as Rec48 (which is only 0x48 bytes). */
typedef struct ActiveNode {
    u8 _pad00[0xA4];
    struct ActiveNode *next;   /* +0xA4 */
} ActiveNode;

/* Guessed 0x24-byte schedule node (static pool at 0x80141AA0, 32 entries),
   ticked by f_main_80104E50_TickSchedule. */
typedef struct SeqNode SeqNode;
typedef s32 (*SeqCb)(SeqNode *node, s32 value);
struct SeqNode {
    SeqNode *prev;      /* +0x00 */
    SeqNode *next;      /* +0x04 */
    u8 _pad08[0x8];
    s32 value;          /* +0x10 */
    u16 flags;          /* +0x14: non-zero = linked */
    u16 period;         /* +0x16: timer reload value */
    s16 offset;         /* +0x18: value increment per tick */
    u16 timer;          /* +0x1A */
    s16 state;          /* +0x1C: 1 = repeating */
    SeqCb fn;           /* +0x20: per-tick callback */
};

/* PSYQ audio-sequencer state (g_main_8012F41C_AudioSeq). The blocked-region
   library code uses its OWN gp base 0x8012EC64 (set at 0x800F8614), so its
   gp-relative accesses land at 0x8012F4xx (NOT the main gp 0x8011EC64). */
typedef struct {
    u32 unk00;          /* +0x00 (8012F41C) */
    u8 _pad04[0x4];
    u32 unk08;          /* +0x08 (8012F424): stop-all shortcut flag */
    void (*cb2)(void);  /* +0x0C (8012F428) */
    u32 mask;           /* +0x10 (8012F42C): per-channel in-use mask */
    u32 pending;        /* +0x14 (8012F430): reaper pending bitmask */
    u32 flagsA;         /* +0x18 (8012F434): channel state bitmask A */
    u32 flagsB;         /* +0x1C (8012F438): channel state bitmask B */
    u32 flagsC;         /* +0x20 (8012F43C): channel state bitmask C */
    void *activeHead;   /* +0x24 (8012F440): singly-linked list head, nodes
                            chained through their own +0xA4 "next" field
                            (see f_main_800FDC04_AppendActiveNode) */
    u8 _pad28[0x10];    /* 0x8012F444..0x8012F453 */
    s32 activeMode;     /* +0x38 (8012F454): currently active audio mode
                            (0 = none), see f_main_8010008C_SetAudioMode */
    u8 _pad3C[0x4];     /* 0x8012F458..0x8012F45B */
    s16 volL;           /* +0x40 (8012F45C): target volume L */
    s16 volR;           /* +0x42 (8012F45E): target volume R */
    u32 volFlag;        /* +0x44 (8012F460): volume-change pending */
    u32 count;          /* +0x48 (8012F464): poll counter */
    void (*cb0)(void);  /* +0x4C (8012F468): poll tail callback 0 */
    void (*cb1)(void);  /* +0x50 (8012F46C): poll tail callback 1 */
    u32 unk54;          /* +0x54 (8012F470) */
    u32 busy;           /* +0x58 (8012F474): reentrancy guard */
    SeqNode *seqHead;   /* +0x5C (8012F478): schedule list head */
    u32 seqLock;        /* +0x60 (8012F47C): schedule tick guard */
    u32 seqPause;       /* +0x64 (8012F480): schedule tick guard */
} AudioSeqState;

/* Guessed song/sequence tree node (0x24 bytes, array-indexed, stride
   confirmed by index*36 addressing in f_main_800F9F68_PlaySongNode and
   siblings func_800FA404/func_800FA560). `state` (0-14) selects behavior
   via a jump table; `childMask` (+0x1C) doubles as "has queued child"
   (nonzero test) and a per-child-index bitmask (OR'd with 1<<i). pitch/vol
   accept -1 ("reset to default") / -2 ("leave as-is") sentinels from
   callers, else are set directly. Field roles beyond that are unconfirmed
   guesses. */
typedef struct SongSlot {
    u32 state;          /* +0x00 */
    void *handler;      /* +0x04: nonzero = slot in use */
    u8 _pad08[0x5];
    u8 childCount;      /* +0x0D */
    u8 unk0E;           /* +0x0E */
    u8 _pad0F;
    u8 unk10;           /* +0x10 */
    u8 _pad11;
    u8 pitch;           /* +0x12: override (sentinel -1/-2) */
    u8 pitchDefault;    /* +0x13: backup for pitch == -1 */
    u16 vol;            /* +0x14: override (sentinel -1/-2) */
    u16 volDefault;     /* +0x16: backup for vol == -1 */
    u8 unk18;           /* +0x18 */
    u8 _pad19[3];
    u32 childMask;      /* +0x1C */
    u8 _pad20[4];
} SongSlot;

/* Guessed owner of a SongSlot array (`slots`, +0x20); `count` (+0x18)
   bounds the valid index range. Also lives on the global list at
   g_main_8012F408_unk (chained via `next`, matched by `tag` - see
   f_main_800FC6A8_FindById / f_main_800FC9C4_NotifySlotsByTag). */
typedef struct SongTree {
    u8 _pad00[0xC];
    s32 tag;             /* +0x0C: match key for f_main_800FC6A8_FindById */
    struct SongTree *next; /* +0x14: g_main_8012F408_unk list link */
    s16 count;          /* +0x18 */
    u8 _pad1A[4];
    SongSlot *slots;    /* +0x20 */
} SongTree;

/* Guessed sound-effect rate-limit rule (g_main_80134E2C_RateLimits[5]),
   consulted by f_main_8008D21C_PlaySound: if `id` falls in [idMin, idMax]
   for the matching `category`, and fewer than `cooldown` ticks (against
   g_main_8011F668_unk) have passed since the last play, the request is
   dropped. */
typedef struct {
    u8 category;    /* +0x00 */
    u8 idMin;        /* +0x01 */
    u8 idMax;        /* +0x02 */
    u8 _pad03;
    u32 lastTime;    /* +0x04 */
    u32 cooldown;    /* +0x08 */
} RateLimitRule;

/* Guessed in-flight positional-sound tracking slot
   (g_main_80134D4C_SoundQueue[8]); `id`/`subId` identify the sound,
   `ctx` is the caller's context pointer, `pos` is (at most 3 words) copied
   from the caller's position argument - the 4th source word is
   immediately clobbered by `active`, so it's effectively discarded. */
typedef struct {
    s32 id;          /* +0x00: -1 = free slot */
    s32 subId;       /* +0x04 */
    void *ctx;       /* +0x08 */
    s32 pos[3];      /* +0x0C */
    s32 active;      /* +0x18 */
} SoundQueueEntry;

/* Guessed byte-stream reader (init.ovl); `cursor` (+0x04) is a read
   pointer, advanced one byte at a time by f_init_801594F4_ReadU16 and
   sibling functions (e.g. func_80159530). */
typedef struct {
    u8 _pad00[0x4];
    u8 *cursor;    /* +0x04 */
} ByteStream;

/* MMID (multi-MIDI container) object, confirmed by the "MMID" magic
   (0x44494D4D) at +0x00 in f_main_800FC814_DestroyMmid. `trackCount`
   (+0x07) bounds the pointer array at +0x10 (stride 4, each entry another
   MmidObj/track to recursively destroy). `next` (+0x0C) links it into the
   library's global active-MMID list (g_main_8012F40C_ActiveMmid; mirrored
   main-side at g_main_8011F40C_ActiveMmid per user-provided knowledge). */
typedef struct MmidObj {
    u32 magic;              /* +0x00: 'MMID' */
    u8 _pad04[0x3];
    u8 trackCount;          /* +0x07 */
    struct MmidObj *next;   /* +0x0C */
    struct MmidObj *tracks[1]; /* +0x10: trackCount entries */
} MmidObj;

/* Guessed handle-like object passed to f_main_80010418_GetField80. Callers
   build the value by OR-ing a raw field with 0x80000000 (turning it into a
   valid pointer); -1 is treated as "no handle". Only +0x80 is confirmed. */
typedef struct {
    u8 _pad00[0x80];
    s32 field80;    /* +0x80 */
} HandleObj80;

/* guess: 3-entry table at D_80134D1C, reset by f_main_8008E14C_ResetAudioSlots.
   Shape (id/subId/type=-1, flag=0xFF, level=0x50) resembles the
   SoundQueueEntry/RateLimitRule "free slot" reset pattern elsewhere in the
   audio code. */
typedef struct {
    s32 unk0;    /* +0x00: -1 = free */
    s16 unk4;    /* +0x04: -1 */
    s16 unk6;    /* +0x06: -1 */
    s16 unk8;    /* +0x08: -1 */
    u8 unkC;     /* +0x0C: 0xFF */
    u8 unkD;     /* +0x0D: 0x50 */
    u8 _padE[0x2];
} AudioSlotEntry;

/* Guessed record returned via func_8002D3A8's outFlag param (init.ovl
   lookup-by-id helper, see f_init_8015E6E8_CheckEntry). Used in
   f_init_801630EC_ResolveSlot both as a truthy "found" flag and,
   directly, as a pointer to this record: `count` gates a
   "needs >= 2 entries" check, `next` gets self-linked to +0x8 (empty
   list init pattern). */
typedef struct {
    s32 count;    /* +0x00 */
    void *next;   /* +0x04: set to (u8 *)self + 8 */
} LookupRecord;

/* Guessed nested object at ResourceSlot.sub (+0xC), only touched by the
   "extraInit" path of f_init_801630EC_ResolveSlot. Also the object
   resolved (two hops: self->+0xC->+0x160) by func_8008F844 (main module,
   undecompiled) and manipulated by f_init_80163680_SetSlotIndex /
   f_init_801636D0_InsertAtIndex - `unk128`/`unk12C`/`unk130` get written
   by nearby init.ovl code (caller of InsertAtIndex) from a third source
   object, `unk134` is a list/slot index, and `next` links instances into
   a singly-linked list (per-index insert via InsertAtIndex). */
typedef struct ResourceSub {
    u8 _pad000[0x124];
    s32 unk124;   /* +0x124: cleared */
    s32 unk128;   /* +0x128: cleared in ResolveSlot; also set from a source object elsewhere */
    s32 unk12C;   /* +0x12C */
    s32 unk130;   /* +0x130 */
    s32 unk134;   /* +0x134: list/slot index, set by f_init_80163680_SetSlotIndex */
    u8 _pad138[0x8];
    s32 unk140;   /* +0x140: set to func_8002D3A8's outValue */
    u8 _pad144[0x28];
    struct ResourceSub *next; /* +0x16C: singly-linked list pointer, see f_init_801636D0_InsertAtIndex */
} ResourceSub;

/* Guessed object passed to f_init_801630EC_ResolveSlot and its two thin
   wrappers (f_init_801631C0/E4). `id` feeds func_8002D3A8's lookup;
   `sub` is NULL until resolved (checked first as a "already done"
   short-circuit). */
typedef struct {
    u8 _pad00[0x2];
    s16 id;         /* +0x02 */
    u8 _pad04[0x6];
    ResourceSub *sub; /* +0x0C */
} ResourceSlot;

/* Guessed 8-entry array at g_main_8012C94C_ResetEntries, stride 8 bytes;
   only the leading s16 is touched (reset to -1) by
   f_init_80164A84_ResetEntries. */
typedef struct {
    s16 unk0;   /* +0x00: reset to -1 */
    u8 _pad2[6];
} ResetEntry;

/* Guessed source object passed to f_init_80165D48_BuildTable: header
   fields copied verbatim into g_main_80114918.. globals, plus a count
   (+0x24) and a scale value (+0x20, reused sign-extended*2) fed to
   func_80100260 (main module, undecompiled). */
typedef struct {
    u8 _pad00[0x1C];
    u16 unk1C;   /* +0x1C */
    u16 unk1E;   /* +0x1E */
    u16 unk20;   /* +0x20: also used (sign-extended, x2) as a scale value */
    u16 unk22;   /* +0x22 */
    s32 unk24;   /* +0x24: count */
} TableSrc;

/* Guessed 0x10-byte constant "preset" record (g_init_80158BEC_Preset):
   4 raw s32 fields, snapshotted twice into a stack pair (before/after
   values) by f_init_80167638_BuildBlend and passed to func_80167510
   along with an interpolation ratio - exact meaning (color? transform?)
   unconfirmed. */
typedef struct {
    s32 unk0;   /* +0x00 */
    s32 unk4;   /* +0x04 */
    s32 unk8;   /* +0x08 */
    s32 unkC;   /* +0x0C */
} Preset4;

/* Guessed 0x38-byte slot record, g_main_80121F40_SoundSlots (60 entries,
   walked by f_main_8001C758_ApplyReverbToActiveSlots). +0x20 is an
   "active" pointer/flag; +0x28 is passed as the destination arg to
   func_800205E4. */
typedef struct {
    u8 _pad00[0x20];
    void *active;   /* +0x20 */
    u8 _pad24[0x4];
    u8 unk28[0x10]; /* +0x28: passed as func_800205E4's dst arg */
} SoundSlot;

/* Guessed 0x14-byte per-category record, g_main_80120514_SoundParams.
   Only the leading word is read/written by the 0x1C82x sound-source code;
   the rest is unconfirmed padding. */
typedef struct {
    s32 val;        /* +0x00 */
    u8 _pad4[0x10];
} SoundParamSlot;

typedef struct SoundSrcNode SoundSrcNode;
typedef struct SoundSrcInfo SoundSrcInfo;
typedef struct SoundSrcEntry SoundSrcEntry;

/* Guessed per-source "info" block (SoundSrcEntry.info->+0x10). flags28 is
   a bitmask tested by f_main_8001C828_ProcessSoundSources for category
   dispatch (0x01000000 active, 0x200000/0x40000/0x20000 sub-flags). */
struct SoundSrcInfo {
    u8 _pad00[0x20];
    void *unk20;    /* +0x20: -> +0x2 = category/type id (compared to 0xF0) */
    void *unk24;    /* +0x24 */
    u32 flags28;    /* +0x28 */
};

/* Guessed active sound-source entry (SoundSrcNode.entry, +0x0). */
struct SoundSrcEntry {
    void *core;         /* +0x0 */
    u8 _pad4[0x4];
    u8 flags8;           /* +0x8: bit 0x8 = skip */
    u8 unk9;
    u8 flagsA;            /* +0xA: bits 0x20, 0x40 */
    u8 flagsB;             /* +0xB: bit 0x40 */
    void *unkC;             /* +0xC: name/tag pointer, compared to D_8010C3B4 */
    u8 _pad10[0x4];
    SoundSrcInfo *info;      /* +0x10 */
    u8 _pad14[0x4];
    void *unk18;              /* +0x18: back-pointer, compared to the listener */
};

/* Guessed intrusive linked-list node walked by
   f_main_8001C828_ProcessSoundSources (arg1). `next` is +0x8. */
struct SoundSrcNode {
    SoundSrcEntry *entry; /* +0x0 */
    u8 _pad4[0x4];
    SoundSrcNode *next;   /* +0x8 */
};

/* Guessed listener/owner object (f_main_8001C828_ProcessSoundSources'
   arg0). `core` (+0x0) is the underlying game-object with a short
   rotation-ish triple at +0x4/+0xA/+0x10 and a s32 world position at
   +0x14/+0x18/+0x1C (both unconfirmed). */
typedef struct {
    u8 _pad00[0x4];
    s16 rotX;       /* +0x4 */
    u8 _padA[0x4];
    s16 rotZ;       /* +0xA */
    u8 _pad10pad[0x4];
    s16 rotY;       /* +0x10 */
    u8 _pad12[0x2];
    s32 posX;       /* +0x14 */
    s32 posY;       /* +0x18 */
    s32 posZ;       /* +0x1C */
} ListenerCore;

typedef struct {
    void *core;      /* +0x0: -> ListenerCore */
    u16 unk4;        /* +0x4: compared to 0x2B (level threshold) */
    u16 unk6;        /* +0x6: bit 0x10 tested */
    u8 unk8;         /* +0x8 */
    u8 unk9;         /* +0x9 */
    u8 _padA[0x18 - 0xA];
    s32 unk18;       /* +0x18: zeroed by f_main_8001382C_InitFromCore */
    s32 snapshot[8]; /* +0x1C..+0x3C: raw copy of core[0x0..0x20) */
    u8 _pad3C[0x68 - 0x3C];
    u8 unk68[0x8];   /* +0x68: passed by-ref to func_800F4200 */
    u8 _pad70[0x9C - 0x70];
    void *unk9C;     /* +0x9C: -> word tested for 0 */
    u8 _pad11A0[0x13D - 0xA0];
    u8 slots[0x1A4 - 0x13D]; /* +0x13D: per-slot bytes, count = g_main_8011F660_SlotCount */
    void *sub1A4;    /* +0x1A4: -> +0x10 -> +0x28 flags (0x20000) */
} SoundListener;

/* Guessed target/source object (f_main_8001382C_InitFromCore's arg1). 4
   s16 fields + 1 s32 field are temporarily negated (mirrored) around the
   func_8001084C call in that function and then restored - likely a
   position/rotation record being flipped into a mirrored coordinate
   space for that call only. */
typedef struct {
    u8 _pad0[0x2];
    s16 unk2;    /* +0x2 */
    u8 _pad4[0x2];
    s16 unk6;    /* +0x6 */
    u8 _pad8[0x2];
    s16 unkA;    /* +0xA */
    u8 _padC[0x2];
    s16 unkE;    /* +0xE */
    u8 _pad10[0x8];
    s32 unk18;   /* +0x18 */
} MirrorObj;

/* Small 2-halfword struct used for the "current 3D pan target" globals
   (D_801205E8/EC and D_80120614/618 - both share this layout). */
typedef struct {
    s16 lo;
    s16 hi;
} PanPair;

/* Guessed "flags4 & visibility-bitmap" list node, walked off
   g_main_8011EC90_unk in f_main_8001C828_ProcessSoundSources. Only +0x4
   (flags word) and +0x40 (bitmap base, accessed 32-bit-aligned) are used;
   exact meaning (an OT/visibility bitmask test) is unconfirmed. */
typedef struct FlagNode FlagNode;
struct FlagNode {
    u8 _pad0[4];
    u32 flags4;   /* +0x4 */
    u8 _pad8[0x38];
};
typedef struct FlagListNode FlagListNode;
struct FlagListNode {
    FlagNode *item;      /* +0x0 */
    u8 _pad4[4];
    FlagListNode *next;  /* +0x8 */
};

/* Guessed target/category struct pointed to by SoundSrcInfo.unk20/unk24;
   +0x2 is a category/type id compared against 0xF0. */
typedef struct {
    u8 _pad0[2];
    s16 unk2;
} TargetInfo;

/* Guessed WLDEMD.HOG-style resource TOC, filled by func_80026B80 for
   f_init_801627C0_LoadWldModels. Same convention as .HOG's own data-offset
   table (see toolz/hog_thing.py): `offsets[i]`/`offsets[i+1]` bound entry
   i's data - except the very last entry, whose end is derived from the
   stream's total size instead of reading past the array. */
typedef struct {
    s32 unk00;         /* +0x00: unconfirmed */
    s32 count;         /* +0x04: number of records */
    s32 recordsOffset; /* +0x08: offset to s32 offsets[count] (boundaries, see above) */
    s32 namesOffset;   /* +0x0C: offset to a NUL-terminated names blob, in entry order */
    s32 dataOffset;    /* +0x10: base offset added to `offsets[i]` to reach entry i's data */
} WldToc;

/* Guessed per-model slot filled by f_init_801627C0_LoadWldModels for each
   WLDEMD.HOG entry (array g_main_8011F6C8_ModelSlots, stride 0x10). */
typedef struct {
    s8 *name;       /* +0x00: entry name pointer (into the WldToc names blob) */
    u32 sectorInfo; /* +0x04: guess: filled by func_800F8268(sector, &this->sectorInfo) */
    s8 unk08;       /* +0x08: guess: sector-rounded size (multiple of g_main_8011F98C_ClipSectors) */
    u8 index;       /* +0x09: this slot's own index */
    u8 index2;      /* +0x0A: duplicate of +0x09 */
    u8 _pad0B;
    u16 len;        /* +0x0C: entry length in bytes minus 1 */
} WldModelSlot;

/* Guessed resource object loaded via func_80166C08, pointed to by
   WldModelHandle.res. */
typedef struct WldTypeDef WldTypeDef;
typedef struct {
    u8 _pad00[0x8];
    u8 flags8;       /* +0x08: bit 0x08 toggled by f_main_800AFF0C_SetModelFlag8 */
    u8 _pad09[0x2];
    u8 flagsB;       /* +0x0B: bit 0x40 set once loaded */
    u8 _pad0C[0x4];
    WldTypeDef *typeDef; /* +0x10 */
} WldRes;

/* Guessed per-model-type definition (WldRes.typeDef). */
struct WldTypeDef {
    u8 _pad00[0x20];
    s32 dataPtr;     /* +0x20: guess: resolved streaming data pointer (high bit masked off) */
    u32 flags28;     /* +0x28: bits 0x20000/0x100000/0x200000 set by the loader, by model "type" byte */
};

/* Guessed per-model resource handle slot (array g_main_8011F680_ModelHandles,
   stride 0x40). res is filled by func_80166C08 (guess: create/load a named
   resource); subRes is a guessed run-length-indexed array of extra pointers
   written in the attach loop, exact meaning unconfirmed. */
typedef struct {
    WldRes *res;      /* +0x00 */
    void *subRes[15]; /* +0x04..+0x3C */
} WldModelHandle;

/* Guessed streaming clip slot (array g_main_8011F968_ClipSlots, stride 0x14). */
typedef struct {
    s32 base;        /* +0x00: streaming buffer base address for this clip */
    s32 unk04;       /* +0x04: attach loop stores WldModelHandle.res here */
    void *unk08;     /* +0x08: attach loop stores a WldModelSlot* here */
    s32 modelIndex;  /* +0x0C: -1 = free/unassigned */
    s32 free;        /* +0x10: 1 = free (guess) */
} WldClipSlot;

/* Guessed level streaming context, pointed to by global g_main_8011F598_StreamCtx. */
typedef struct {
    u8 _pad00[0x8];
    s32 unk08;       /* +0x08: guess: streaming buffer base */
    u8 _pad0C[0x24];
    s32 unk30;       /* +0x30: guess: streaming buffer offset, added to unk08 */
    u32 attachCount; /* +0x34: number of entries in the attach list below */
    u8 attachList[1]; /* +0x38: flexible array, length attachCount; each byte
                          is a WldModelSlot index (0xFE = "self/skip" marker) */
} WldStreamCtx;

/* Guessed streamed-VAB/VH load record, returned via
   f_main_80026E00_FindResource's out-param and consumed by
   f_main_800FC13C_BeginVhLoad. +0x08 bit0 = "pointer fields relocated",
   bit2 = alt-init path; +0x20/+0x24/+0x28 start as offsets and get fixed
   up to absolute pointers (base-relative, `+= this`) the first time
   through; +0x2C is a handle produced by the alt-init call; +0x30 is the
   raw (still HOG-relative) source pointer. */
typedef struct {
    u8 _pad00[0x8];
    u32 flags08;   /* +0x08 */
    u8 _pad0C[0x20 - 0xC];
    s32 unk20;     /* +0x20 */
    s32 unk24;     /* +0x24 */
    s32 unk28;     /* +0x28 */
    s32 unk2C;     /* +0x2C */
    s32 unk30;     /* +0x30 */
} VhLoadRecord;

/* Guessed generic 16-byte position record (x/z only confirmed, by
   f_main_80027354_FastDist2D; y/w are read/written by callers but never
   touched by that function). */
typedef struct {
    s32 x;   /* +0x00 */
    s32 y;   /* +0x04 */
    s32 z;   /* +0x08 */
    s32 w;   /* +0x0C */
} Vec4;

/* Guessed fixed-size slot pool record, base D_8012340C (5 slots, stride
   0x10), allocated by f_main_8002137C_AllocPoolA. */
typedef struct {
    u8 _pad00[0x4];
    s32 marker; /* +0x04: 0xCACACACA = free/unused, else in-use */
    u8 _pad08[0x10 - 0x8];
} PoolARecord;

/* Guessed fixed-size slot pool record, base D_80125FF4 (5 slots, stride
   0x14), allocated by f_main_80026114_AllocPoolB. */
typedef struct {
    u8 _pad00[0x4];
    s32 marker; /* +0x04: 0xCACACACA = free/unused, else in-use */
    u8 _pad08[0x14 - 0x8];
} PoolBRecord;

/* Guessed 0x2C-byte slot record reset by f_main_80056AF8_ResetSlots.
   +0x04/+0x06 default to 0x190 (400 - guess: timer/range), +0x14..0x16
   default to 0xFF,0xFF,0xFF (guess: RGB, full white), +0x1C/+0x1E default
   to 0x1000 (guess: 12-bit fixed-point 1.0 scale). */
typedef struct {
    s32 unk00;
    s16 unk04;
    s16 unk06;
    s16 unk08;
    s16 unk0A;
    s16 unk0C;
    s8 unk0E;
    s8 unk0F;
    s16 unk10;
    s16 unk12;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 _pad17;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s32 unk20;
    s32 unk24;
    s32 unk28;
} ResetSlot;

/* Guessed fixed 4-entry lookup table g_main_80138680_unk; keyA/keyB use
   -1 as a wildcard, see f_main_800AA658_LookupValue. */
typedef struct {
    s32 count;
    s16 keyA[4];
    s16 keyB[4];
    s32 value[4];
} LookupTable4;

/* Guessed table entry, base g_main_8011F5B8_unk (stride 0x10): +0x4 is a
   count, +0x8 a pointer-to-pointer array of that many entries, see
   f_main_80080428_FindIdleSlot. */
typedef struct {
    u8 _pad00[0x4];
    u8 count;    /* +0x4 */
    u8 _pad05[0x3];
    void **items; /* +0x8: array of `count` pointers */
} IdleTable;

/* Guessed record pointed to by IdleTable.items[i]; unk8/9/A are checked
   for "busy" (>0). */
typedef struct {
    u8 _pad00[0x8];
    s8 unk8;
    s8 unk9;
    s8 unkA;
} IdleRecord;

/* Guessed 0x3C-byte history-buffer record, see f_main_8006AC48_RecordHistory. */
typedef struct {
    s32 words[15];
} HistoryRecord;

/* Guessed 0x48-byte scratch context built by f_main_8006AD54_InvokeWithCtx
   and handed to a caller-supplied callback. Only +0x04..+0x28 are ever
   written by that function; the rest is untouched (left for the callback
   itself). */
typedef struct {
    u8 _pad00[0x4];
    s32 flag04;   /* +0x04: always zeroed */
    s32 data[8];  /* +0x08..+0x24: copied from `src` when non-NULL */
    s32 flags28;  /* +0x28: bit 0 = "src was provided"; other bits are
                     whatever was already on the stack (uninitialized in
                     the original asm - matched here, not a bug) */
    u8 _pad2C[0x48 - 0x2C];
} CallbackCtx;

/* Guessed compacted-table record (stride 0x3C), consumed/produced by
   f_main_8006C824_AllocActorSlot/f_main_8006C864_FreeActorSlot. unk30 is
   the record's own compacted-table index (written into
   g_main_8011F7F0_SlotMap[id] on alloc); unk31 is the record's own slot
   id, used as a back-pointer when a swap-remove relocates it. */
typedef struct {
    u8 _pad00[0x30];
    u8 unk30; /* +0x30: own compacted-table index */
    u8 unk31; /* +0x31: own slot id */
    u8 _pad32[0x3C - 0x32];
} SlotRecord;

#endif
