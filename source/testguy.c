#include "testguy.h"
#include "hardware.h"
#include "graphics/komoju.h"

void kaInitTestGuy(ka_test_guy *guy, ka_sprites *sprites) {
    guy->x = 0;
    guy->y = 0;
    guy->spr = (SPRITE *)sprites->obj_buffer;
    
    // Copy over sprite and pallet
    dmaCopy(&tile_mem[4][0], (void *)komojuTiles, komojuTilesLen / 4);
    dmaCopy(SPRITE_PALETTE, (void *)komojuPal, komojuPalLen / 4);

    // Enable the object
    guy->spr->attr0.obj_mode = SPRITE_OBJ_MODE_REGULAR;
    guy->spr->attr0.gfx_mode = SPRITE_GFX_MODE_REGULAR;

    // 64x64 (komoju is not quite this size...!)
    guy->spr->attr0.shape        = 0b00;
    guy->spr->attr1_regular.size = 0b11;
}


void kaUpdateTestGuy(ka_test_guy *guy) {

}
