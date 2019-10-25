#include <gba.h>
#include <maxmod.h>

#include <stdio.h>
#include <stdlib.h>

#include "soundbank.h"
#include "soundbank_bin.h"


#define OAM_SIZE 128


//////////////////////////////////////////////
//                  Sprites
//////////////////////////////////////////////

//
// Sprite structs
//

// SPRITE is a struct-based alternative to OBJATTR.
typedef struct SPRITE_ATTR0 {
    int y          : 8;
    int obj_mode   : 2;
    int gfx_mode   : 2;
    int mosaic     : 1;
    int color_mode : 1;
    int shape      : 2;
} SPRITE_ATTR0;

const int SPRITE_OBJ_MODE_REGULAR       = 0b00;
const int SPRITE_OBJ_MODE_AFFINE        = 0b01;
const int SPRITE_OBJ_MODE_HIDE          = 0b10;
const int SPRITE_OBJ_MODE_AFFINE_DOUBLE = 0b11;

const int SPRITE_GFX_MODE_REGULAR       = 0b00;
const int SPRITE_GFX_MODE_ALPHA         = 0b01;
const int SPRITE_GFX_MODE_WINDOW        = 0b10;
const int SPRITE_GFX_MODE_DONT_USE      = 0b11;

typedef struct SPRITE_REGULAR_ATTR1 {
    int x          : 9;
    int dummy      : 3;
    int hflip      : 1;
    int vflip      : 1;
    int size       : 2;
} SPRITE_REGULAR_ATTR1;

typedef struct SPRITE_AFFINE_ATTR1 {
    int x          : 9;
    int affine_idx : 5;
    int size       : 2;
} SPRITE_AFFINE_ATTR1;

typedef struct SPRITE_ATTR2 {
    int tile_idx   : 10;
    int priority   : 2;
    int palbank    : 4;
} SPRITE_ATTR2;

typedef struct SPRITE {
    SPRITE_ATTR0 attr0;
    union {
        SPRITE_AFFINE_ATTR1  attr1_affine;
        SPRITE_REGULAR_ATTR1 attr1_regular;
    };
    SPRITE_ATTR2 attr2;
    u16 dummy;
} ALIGN(4) SPRITE;


//
// Sprites state
//
typedef struct ka_sprites {
    // Shadow of the OAM
    OBJATTR obj_buffer[OAM_SIZE];
    OBJAFFINE *obj_aff_buffer;
} ka_sprites;

//
// Initialize sprites
//
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
}


//////////////////////////////////////////////
//                  Sound
//////////////////////////////////////////////

//
// Sound state
//
typedef struct ka_sound {
    // mm_sound_effect crows;
    mm_sound_effect invwhistle;
    mm_sfxhand crows_handle; // for cancelling sound-in-progress
} ka_sound;

//
// Initialize sound
//
void kaInitSound(ka_sound *s) {
    // Maxmod requires the vblank interrupt to reset sound DMA.
    // Link the VBlank interrupt to mmVBlank, and enable it.
    irqSet( IRQ_VBLANK, mmVBlank );
    irqEnable(IRQ_VBLANK);

    // initialise maxmod with soundbank and 8 channels
    mmInitDefault( (mm_addr)soundbank_bin, 8 );

    // Start playing module
    mmStart( MOD_HAPPYHUMBLE, MM_PLAY_LOOP );

    s->invwhistle = (mm_sound_effect){
        { SFX_INVERSEWHISTLE } ,// id
        (int)(1.0f * (1<<10)),    // rate
        0,        // handle
        255,    // volume
        255,    // panning
    };

    // s->crows_handle = 0;
}

//
// Update sound
//
void kaUpdateSound(ka_sound *s, int keys_pressed, int keys_released) {
    // Play looping crows sound effect out of left speaker if A button is pressed
    if ( keys_pressed & KEY_A ) {
        // s->crows_handle = mmEffectEx(&s->crows);
    }

    // stop crows sound when A button is released
    if ( keys_released & KEY_A ) {
        // mmEffectCancel(s->crows_handle);
    }

    // Play explosion sound effect out of right speaker if B button is pressed
    if ( keys_pressed & KEY_B ) {
        mmEffectEx(&s->invwhistle);
    }
}


int main() {
    irqInit();

    ka_sound sound;
    ka_sprites sprites;

    kaInitSound(&sound);
    kaInitSprites(&sprites);

    // Main loop
    do {
        // Frame stuff
        VBlankIntrWait();
        mmFrame();

        // Grab input
        int keys_pressed, keys_released;
        scanKeys();

        keys_pressed = keysDown();
        keys_released = keysUp();

        // Run update functions
        kaUpdateSound(&sound, keys_pressed, keys_released);
    } while( 1 );
}
