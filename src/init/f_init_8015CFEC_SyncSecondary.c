#include "../../include/types.h"

/* Player 1's object is referenced here as a byte blob (not a pointer
   variable) because +0x55C indexes deep into it directly from its own
   address - consistent with other code comparing pointers against
   g_main_8012A574_Player's *value*, which only works if the object's
   first field is a self-pointer equal to its own address. */
extern u8 g_main_8012A574_Player[];
extern u8 g_main_8012AAD0_Player[];
extern u8 g_main_8011F624_Flag;   /* guess: debug/cheat "don't spend ammo" flag */

/* --- libc-style externs (not yet decompiled) --- */
extern void func_80017810(s32 a0, s32 a1);
extern void func_80057E7C(s32 a0, s32 a1);

/* If player1->0x55C (a "secondary" sub-object marker) is non-zero, marks
   g_main_8011F624_Flag and processes both player1's primary and secondary
   (+0x55C) sub-objects: cross-checks their +0x8 fields via func_80017810,
   then calls func_80057E7C(obj->0xD8, (*obj->0xE0)->0xC) for each. If the
   marker is zero, clears the flag and only processes the primary. */
void f_init_8015CFEC_SyncSecondary(void) {
    u8 *p1 = g_main_8012A574_Player;
    u8 *p2 = g_main_8012AAD0_Player;
    u8 *sub;

    if (*(s32 *) (p1 + 0x55C) != 0) {
        sub = p1 + 0x55C;
        g_main_8011F624_Flag = 1;
    } else {
        sub = 0;
        g_main_8011F624_Flag = 0;
    }

    if (g_main_8011F624_Flag != 0) {
        func_80017810(*(s32 *) (p1 + 0x8), *(s32 *) (p2 + 0x8));

        func_80057E7C(*(s32 *) (p1 + 0xD8),
                      *(s32 *) (*(u8 **) (p1 + 0xE0) + 0xC));
        func_80057E7C(*(s32 *) (sub + 0xD8),
                      *(s32 *) (*(u8 **) (sub + 0xE0) + 0xC));
    } else {
        func_80057E7C(*(s32 *) (p1 + 0xD8),
                      *(s32 *) (*(u8 **) (p1 + 0xE0) + 0xC));
    }
}
