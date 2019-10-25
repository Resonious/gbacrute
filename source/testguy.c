#include "testguy.h"
#include "hardware.h"
#include "graphics/komoju.h"

void kaInitTestGuy(ka_test_guy *guy, ka_sprites *sprites) {
    guy->x = 0;
    guy->y = 0;
    guy->spr = (SPRITE *)sprites->obj_buffer;
    sprites->allocated_objs += 1;
    
    // Copy over sprite and pallet
    //dmaCopy(&tile_mem[4][0], (void *)komojuTiles, komojuTilesLen / 4);
    //dmaCopy(SPRITE_PALETTE, (void *)komojuPal, komojuPalLen / 4);
    int *tiles = (int *)&tile_mem[4][0];
    int *palette = (int *)SPRITE_PALETTE;
    for (int i = 0; i < komojuTilesLen/4; ++i)
        tiles[i] = komojuTiles[i];
    for (int i = 0; i < komojuPalLen/4; ++i)
        palette[i] = ((int*)komojuPal)[i];

    // Enable the object
    guy->spr->attr0.obj_mode   = SPRITE_OBJ_MODE_REGULAR;
    guy->spr->attr0.gfx_mode   = SPRITE_GFX_MODE_REGULAR;
    guy->spr->attr0.color_mode = SPRITE_COLOR_MODE_8BPP;

    // Tile and palette (?)
    guy->spr->attr2.tile_idx     = 0;
    guy->spr->attr2.palbank      = 0;

    // 64x64
    guy->spr->attr0.shape        = 0b00;
    guy->spr->attr1_regular.size = 0b11;

    // Position
    guy->spr->attr0.y = sizeof(SPRITE);
    guy->spr->attr1_regular.x = 100;

    // Commit changes
    kaUpdateOAM(sprites, 0, 1);
}


void kaUpdateTestGuy(ka_test_guy *guy, ka_sprites *sprites, KEYS keys) {
    bool moved = false;

    if (keys.pressed & KEY_LEFT) {
        guy->spr->attr1_regular.x -= 10;
        moved = true;
    }
    if (keys.pressed & KEY_RIGHT) {
        guy->spr->attr1_regular.x += 10;
        moved = true;
    }
    if (keys.pressed & KEY_UP) {
        guy->spr->attr0.y -= 10;
        moved = true;
    }
    if (keys.pressed & KEY_DOWN) {
        guy->spr->attr0.y += 10;
        moved = true;
    }

    if (moved)
        kaUpdateOAM(sprites, 0, 1);
}
