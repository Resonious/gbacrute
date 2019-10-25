#include "sprites.h"


void kaUpdateOAM(ka_sprites *s, int start, int count) {
    OBJATTR *shadow_oam = s->obj_buffer;
    OBJATTR *real_oam   = OAM;

    for (int start = start; start < count; ++start)
        *real_oam++ = *shadow_oam++;
}


void kaInitSprites(ka_sprites *s) {
    s->allocated_objs = 0;

    // Set up shadows
    s->obj_aff_buffer = (OBJAFFINE*)s->obj_buffer;

    // Clear and hide each object
    u32 nn      = OAM_SIZE;
    SPRITE *spr = (SPRITE*)s->obj_buffer;
    while(nn--)
        spr->attr0.obj_mode = SPRITE_OBJ_MODE_HIDE;

    // Copy shadow into real OAM
    kaUpdateOAM(s, 0, OAM_SIZE);

    // Enable sprites (1D char mapping)
    REG_DISPCNT = OBJ_ON | OBJ_1D_MAP;
}


void kaUpdateSprites(ka_sprites *s) {
    //copy_shadow_to_real(s->obj_buffer, s->allocated_objs);
}
