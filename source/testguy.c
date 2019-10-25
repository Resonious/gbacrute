#include "testguy.h"
#include "hardware.h"
#include "graphics/komoju.h"

void kaInitTestGuy(ka_test_guy *guy, ka_sprites *sprites) {
    guy->dx = 0;
    guy->dy = 0;
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


static void readInput(ka_test_guy *guy, KEYS keys) {
    if (keys.pressed & KEY_LEFT)  guy->mx = -1;
    if (keys.pressed & KEY_RIGHT) guy->mx =  1;
    if (keys.pressed & KEY_UP)    guy->my = -1;
    if (keys.pressed & KEY_DOWN)  guy->my =  1;

    if (guy->mx < 0 && keys.released & KEY_LEFT)  guy->mx = 0;
    if (guy->mx > 0 && keys.released & KEY_RIGHT) guy->mx = 0;
    if (guy->my < 0 && keys.released & KEY_UP)    guy->my = 0;
    if (guy->my > 0 && keys.released & KEY_DOWN)  guy->my = 0;
}

static inline int signof(int x) {
    if      (x < 0) return -1;
    else if (x > 0) return 1;
    else            return 0;
}

static void moveGuy(ka_test_guy *guy) {
    // Accelerate from input
    if (guy->mx != 0) guy->ddx = guy->mx * GUY_ACCEL;
    else              guy->ddx = -signof(guy->dx);
    if (guy->my != 0) guy->ddy = guy->my * GUY_ACCEL;
    else              guy->ddy = -signof(guy->dy);

    // Speed up from acceleration
    guy->dx += guy->ddx;
    guy->dy += guy->ddy;
    if (guy->dx >  GUY_SPEED) guy->dx =  GUY_SPEED;
    if (guy->dx < -GUY_SPEED) guy->dx = -GUY_SPEED;
    if (guy->dy >  GUY_SPEED) guy->dy =  GUY_SPEED;
    if (guy->dy < -GUY_SPEED) guy->dy = -GUY_SPEED;

    // Displace from speed
    guy->spr->attr1_regular.x += guy->dx;
    guy->spr->attr0.y         += guy->dy;
}

static void animateGuy(ka_test_guy *guy) {
    if (guy->dx < 0)
        guy->spr->attr1_regular.hflip = 1;
    else if (guy->dx > 0)
        guy->spr->attr1_regular.hflip = 0;
}


void kaUpdateTestGuy(ka_test_guy *guy, ka_sprites *sprites, KEYS keys) {
    readInput(guy, keys);
    moveGuy(guy);
    animateGuy(guy);
    kaUpdateOAM(sprites, 0, 1);
}
