#include "../../include/types.h"

extern void *g_main_8011EE68_FogDesc;  /* nonzero = a FOG archive is open;
    see f_main_8002608C_OpenFog, which sets this to &D_80126058 (the FOG
    archive descriptor), not just a plain boolean */
extern FileHandle *g_main_8011EE64_Handle;

extern s32 f_main_8002662C_CloseFile(FileHandle **hp);

/* Closes g_main_8011EE64_Handle if g_main_8011EE68_FogDesc is set, then
   clears it. */
void f_main_8002605C_CloseIfOpen(void) {
    if (g_main_8011EE68_FogDesc != 0) {
        f_main_8002662C_CloseFile(&g_main_8011EE64_Handle);
        g_main_8011EE68_FogDesc = 0;
    }
}
