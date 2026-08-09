#include "../../include/types.h"

/* Music/audio-sequencer state (block 0x8011F370..., gp-relative). */
extern s32 g_main_8011F370_SongTime;    /* zeroed when a song changes */
extern SongFlags g_main_8011F374_SongFlags;  /* song-changed flag + track count */
extern s16 g_main_8011F378_unk;
extern u16 g_main_8011F37A_TrackIndex;  /* parallel-array counter */
extern s16 g_main_8011F37E_unk;
extern u8 g_main_8011F380_unk;
extern s32 g_main_8011F388_CurrentSongId;
extern u16 g_main_8011F310_SongId;
extern void *g_main_8013CBA0_TrackList[];      /* registered track objects */
extern void *g_main_8013CB70_TrackOwnerList[]; /* parallel: song owner objects */
extern void *g_main_80117078_TrackSlots[];     /* indexed by track type 1..3 */
extern WeaponDef *g_main_8011EEF8_WeaponDefs;  /* per-id definitions, 0x4C-byte records */

/* --- not yet decompiled --- */
extern void func_8002C770(void);   /* music callback */
extern void func_80013610(void (*func)(void), s32 arg, s16 id); /* event-register helper */

/* Music/audio-sequencer entry: register a track object for `song`.
   If the song id is lower than the current one the sequencer state is reset.
   When no tracks are registered yet the song callback is armed via
   func_80013610. A track whose per-id type (WeaponDef.unk28) is 1..3 is
   stored in the type slot list; otherwise it is appended to the track list
   (with a parallel owner list). Bit 0x40000 on the track marks it as
   registered. */
void f_main_800C2CD4_RegisterTrack(SongReq *song) {
    s16 id = song->songId;
    s16 type;
    TrackObj *track;
    s32 i;

    track = (TrackObj *) *(void **) ((u8 *) song->owner + 0x10);
    type = g_main_8011EEF8_WeaponDefs[id].unk28;

    if (id < g_main_8011F388_CurrentSongId)
        g_main_8011F374_SongFlags.changed = 1;

    g_main_8011F388_CurrentSongId = id;

    if (g_main_8011F374_SongFlags.changed != 0) {
        g_main_8011F378_unk = 0;
        g_main_8011F37A_TrackIndex = 0;
        g_main_8011F37E_unk = 0;
        g_main_8011F374_SongFlags.count = 0;
        g_main_8011F370_SongTime = 0;
        g_main_8011F374_SongFlags.changed = 0;
        g_main_8011F380_unk = 0;
    }

    i = 0;
    while (i < g_main_8011F374_SongFlags.count) {
        if (g_main_8013CBA0_TrackList[i] == track)
            return;
        i++;
    }
    if (i == 0) {
        g_main_8011F310_SongId = (u16) id;
        func_80013610(&func_8002C770, 4, id);
    }

    if (type != 0 && type < 4) {
        g_main_80117078_TrackSlots[type] = track;
        track->flags28 |= 0x80000;
    } else {
        g_main_8013CBA0_TrackList[g_main_8011F374_SongFlags.count] = track;
        g_main_8011F374_SongFlags.count++;
        g_main_8013CB70_TrackOwnerList[g_main_8011F37A_TrackIndex] = song->owner;
        g_main_8011F37A_TrackIndex++;
    }

    song->flags27 |= 0x02;
    track->flags28 |= 0x40000;
}
