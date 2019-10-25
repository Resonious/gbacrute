#include "sprites.h"


void kaInitSprites(ka_sprites *s) {
    // Set up shadows
    s->obj_aff_buffer = (OBJAFFINE*)s->obj_buffer;

    // Clear and hide each object
    u32 nn      = OAM_SIZE;
    SPRITE *spr = (SPRITE*)s->obj_buffer;
    while(nn--)
        spr->attr0.obj_mode = SPRITE_OBJ_MODE_HIDE;

    // Copy shadow into real OAM (will probably want a function at some point)
    u32 count           = 128;
    OBJATTR *real_oam   = OAM;
    OBJATTR *shadow_oam = s->obj_buffer;
    while (count--)
		*real_oam++ = *shadow_oam++;

    // Enable sprites (1D char mapping)
    REG_DISPCNT = OBJ_ON | OBJ_1D_MAP;
}

