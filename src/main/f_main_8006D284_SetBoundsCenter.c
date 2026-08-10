#include "../../include/types.h"

extern s32 g_main_8011F780_BoundsDirty; /* guess: dirty flag consumed elsewhere */
extern Vec4 g_main_8012FC0C_BoundsSize; /* guess: last-set bounds size (x/y/z used) */
extern s32 g_main_8012FC7C_BoundsOriginX;
extern s32 g_main_8012FC80_BoundsOriginY;
extern s32 g_main_8012FC84_BoundsOriginZ;

/* guess: records a new bounding-volume size and derives its origin from a
   center position (origin = center - size / 2), marking the bounds dirty
   for whatever consumes them later. */
void f_main_8006D284_SetBoundsCenter(Vec4 *center, Vec4 *size) {
    g_main_8011F780_BoundsDirty = 1;

    g_main_8012FC0C_BoundsSize.x = size->x;
    g_main_8012FC0C_BoundsSize.y = size->y;
    g_main_8012FC0C_BoundsSize.z = size->z;
    g_main_8012FC0C_BoundsSize.w = size->w;

    g_main_8012FC7C_BoundsOriginX = center->x - (g_main_8012FC0C_BoundsSize.x >> 1);
    g_main_8012FC80_BoundsOriginY = center->y - (g_main_8012FC0C_BoundsSize.y >> 1);
    g_main_8012FC84_BoundsOriginZ = center->z - (g_main_8012FC0C_BoundsSize.z >> 1);
}
