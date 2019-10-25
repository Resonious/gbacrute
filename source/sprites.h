#ifndef SPRITES_H
#define SPRITES_H

#include <gba.h>
#include "hardware.h"

typedef struct ka_sprites {
    // Shadow of the OAM
    OBJATTR obj_buffer[OAM_SIZE];
    OBJAFFINE *obj_aff_buffer;
} ka_sprites;

void kaInitSprites(ka_sprites *s);
#endif
