#ifndef HARDWARE_H
#define HARDWARE_H
#include <gba.h>

#define PACKED __attribute__((__packed__))

//////////////////////////////////////////////
//                  Sprites
//////////////////////////////////////////////

//
// Sprite structs
//

#define OAM_SIZE 128

// SPRITE is a struct-based alternative to OBJATTR.
typedef struct SPRITE_ATTR0 {
    int y          : 8;
    int obj_mode   : 2;
    int gfx_mode   : 2;
    int mosaic     : 1;
    int color_mode : 1;
    int shape      : 2;
} PACKED SPRITE_ATTR0;

#define SPRITE_COLOR_MODE_4BPP (0)
#define SPRITE_COLOR_MODE_8BPP (1)

#define SPRITE_OBJ_MODE_REGULAR       (0b00)
#define SPRITE_OBJ_MODE_AFFINE        (0b01)
#define SPRITE_OBJ_MODE_HIDE          (0b10)
#define SPRITE_OBJ_MODE_AFFINE_DOUBLE (0b11)

#define SPRITE_GFX_MODE_REGULAR       (0b00)
#define SPRITE_GFX_MODE_ALPHA         (0b01)
#define SPRITE_GFX_MODE_WINDOW        (0b10)
#define SPRITE_GFX_MODE_DONT_USE      (0b11)

typedef struct SPRITE_REGULAR_ATTR1 {
    int x          : 9;
    int dummy      : 3;
    int hflip      : 1;
    int vflip      : 1;
    int size       : 2;
} PACKED SPRITE_REGULAR_ATTR1;

typedef struct SPRITE_AFFINE_ATTR1 {
    int x          : 9;
    int affine_idx : 5;
    int size       : 2;
} PACKED SPRITE_AFFINE_ATTR1;

typedef struct SPRITE_ATTR2 {
    int tile_idx   : 10;
    int priority   : 2;
    int palbank    : 4;
} PACKED SPRITE_ATTR2;

typedef struct SPRITE {
    SPRITE_ATTR0 attr0;
    union {
        SPRITE_AFFINE_ATTR1  attr1_affine;
        SPRITE_REGULAR_ATTR1 attr1_regular;
    };
    SPRITE_ATTR2 attr2;
    u16 dummy;
} ALIGN(4) PACKED SPRITE;



//
// Tile structs
//

// tile 8x8@4bpp: 32bytes; 8 ints
typedef struct { u32 data[8];  } TILE, TILE4;
// d-tile: double-sized tile (8bpp)
typedef struct { u32 data[16]; } TILE8;
// tile block: 32x16 tiles, 16x16 d-tiles
typedef TILE  CHARBLOCK[512];
typedef TILE8 CHARBLOCK8[256];

#define tile_mem  ( (CHARBLOCK*)VRAM)
#define tile8_mem ((CHARBLOCK8*)VRAM)

// In code somewhere
//TILE *ptr= &tile_mem[4][12];   // block 4 (== lower object block), tile 12

// Copy a tile from data to sprite-mem, tile 12
//tile_mem[4][12] = *(TILE*)spriteData;


//
// Key states (not really hardware but everyone needs them...)
//

typedef struct KEYS {
    int pressed;
    int released;
} KEYS;

#endif
