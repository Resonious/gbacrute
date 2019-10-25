#ifndef SPRITES_H
#define SPRITES_H

#include <gba.h>
#include "hardware.h"

typedef struct ka_sprites {
    // Shadow of the OAM
    OBJATTR obj_buffer[OAM_SIZE];
    OBJAFFINE *obj_aff_buffer;
    int allocated_objs;
} ka_sprites;

void kaInitSprites(ka_sprites *);
void kaUpdateSprites(ka_sprites *);
void kaUpdateOAM(ka_sprites *, int, int);

#endif
