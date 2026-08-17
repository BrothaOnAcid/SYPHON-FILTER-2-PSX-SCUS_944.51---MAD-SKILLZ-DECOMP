#include "../../include/types.h"

/* Called right after f_main_8001382C_InitFromCore (see 0x80017868/
   0x800178A0, and init.ovl 0x801640C0). Links `other` into `self`'s
   secondary fields and self-links self->unk138 to &self->unkF8, mirroring
   the LookupRecord empty-list init pattern. */
void f_main_80015C8C_LinkListener(SoundListener *self, SoundListener *other) {
    self->unk134 = 1;
    self->unk138 = &self->unkF8;
    self->unk9C = other;
    self->unk104 = 0;
    self->unkF8 = other;
    self->unkFC = other->unk18;
}
