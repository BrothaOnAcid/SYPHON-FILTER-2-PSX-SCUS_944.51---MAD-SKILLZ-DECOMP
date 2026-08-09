#include "../../include/types.h"

/* Builds the 3x3 s16 rotation matrix at `matrix` from 3 HAN 12-bit angles via
   the fixed-point sin/cos lookup table - see f_main_8001C264_BuildMatrix. */
extern s16 *f_main_8001C264_BuildMatrix(const s16 *angles, s16 *matrix);

/* HAN keyframe reader. `packed` = (frameNum << 16) | channelCount; `mask` bit
   i selects channel i - a set bit decodes a keyframe, a clear bit leaves that
   channel's angles untouched (but still advances its node/angle pointers).
   Encodings, selected by bits 7/6/5 of the first byte b0:
     b0&0x80 == 0 : 2 bytes - x += 5-bit, y += 5-bit, z unchanged
     b0&0x40 set  : 4 bytes - x/y/z += 10-bit deltas
     b0&0x20 set  : 6 bytes - absolute 16-bit x/y/z (x top nibble 5-bit signed)
     else         : 3 bytes - x/y/z += 7-bit deltas
   Returns the first unread byte. On the first frame (packed>>16 == 1) each
   processed channel additionally reads a 3-byte translation (y negated) into
   its node's HmdMatrix, then flags the matrix and calls func_8001C264. */
u8 *f_main_8001C4F4_ReadKeyframes(u8 *src, u32 packed, HanCtx *ctx, u32 mask) {
    u8 *p = src;
    s32 n = packed & 0xFF;
    s32 frame = (s32) (packed >> 16) - 1;
    s16 *a = ctx->angles;
    void **nd = ctx->nodes;
    u32 i = 0;
    u8 b0, b1, b2, b3;
    s16 x, y, z;
    s32 t;

    while (n != 0) {
        if (mask & 1) {
            x = a[0];
            y = a[1];
            z = a[2];
            b0 = p[0];
            b1 = p[1];

            if (!(b0 & 0x80)) {
                x += (s16) SEXT(b0 >> 2, 5);
                y += (s16) SEXT(((b0 & 3) << 3) | (b1 >> 5), 5);
                p += 2;
            } else if (b0 & 0x40) {
                b2 = p[2];
                b3 = p[3];
                x += (s16) SEXT(((b0 & 0x3F) << 4) | (b1 >> 4), 10);
                y += (s16) SEXT(((b1 & 0xF) << 6) | (b2 >> 2), 10);
                z += (s16) SEXT(((b2 & 3) << 8) | b3, 10);
                p += 4;
            } else if (b0 & 0x20) {
                t = (b0 & 0x10) ? (b0 & 0x1F) : (b0 | 0xE0);
                x = (s16) ((t << 8) | b1);
                y = (s16) ((p[2] << 8) | p[3]);
                z = (s16) ((p[4] << 8) | p[5]);
                p += 6;
            } else {
                b2 = p[2];
                x += (s16) SEXT(((b0 & 0x1F) << 2) | (b1 >> 6), 7);
                y += (s16) SEXT(((b1 & 0x3F) << 1) | (b2 >> 7), 7);
                z += (s16) SEXT(b2 & 0x7F, 7);
                p += 3;
            }

            a[0] = x;
            a[1] = y;
            a[2] = z;

            {
                HmdMatrix *m = (HmdMatrix *) *((void **) ((u8 *) *nd + 0x20));
                if (frame == 0) {
                    m->tx = (s8) p[0];
                    m->ty = -(s8) p[1];
                    m->tz = (s8) p[2];
                    p += 3;
                } else if (i == 0) {
                    p += 3; /* guess: per-frame header bytes skipped on non-first frames */
                }
                m->flag = 1;
                f_main_8001C264_BuildMatrix(a, m->rot);
            }
        }
        a += 4;
        nd += 1;
        i++;
        mask >>= 1;
        n--;
    }
    return p;
}
