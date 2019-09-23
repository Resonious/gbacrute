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
// Sprites state
//
typedef struct ct_sprites {
    // Shadow of the OAM
    OBJATTR obj_buffer[OAM_SIZE];
    OBJAFFINE *obj_aff_buffer;
} ct_sprites;

//
// Initialize sprites
//
void ctInitSprites(ct_sprites *spr) {
    // Set up shadows
    spr->obj_aff_buffer = (OBJAFFINE*)spr->obj_buffer;

    // Hide each object
    u32 nn   = OAM_SIZE;
    u32 *dst = (u32*)spr->obj_buffer;
    while(nn--)
    {
        // TODO this is very wack. Make a struct for attr0
        *dst++= ATTR0_DISABLED;
        *dst++= 0;
    }

    // Copy shadow into real OAM (will probably want a function at some point)
    u32 count           = 128;
    OBJATTR *real_oam   = OAM;
    OBJATTR *shadow_oam = spr->obj_buffer;
    while (count--)
		*real_oam++ = *shadow_oam++;
}


//////////////////////////////////////////////
//                  Sound
//////////////////////////////////////////////

//
// Sound state
//
typedef struct ct_sound {
    mm_sound_effect crows;
    mm_sound_effect invwhistle;
    mm_sfxhand crows_handle; // for cancelling sound-in-progress
} ct_sound;

//
// Initialize sound
//
void ctInitSound(ct_sound *s) {
    // Maxmod requires the vblank interrupt to reset sound DMA.
    // Link the VBlank interrupt to mmVBlank, and enable it.
    irqSet( IRQ_VBLANK, mmVBlank );
    irqEnable(IRQ_VBLANK);

    // initialise maxmod with soundbank and 8 channels
    mmInitDefault( (mm_addr)soundbank_bin, 8 );

    // Start playing module
    mmStart( MOD_HAPPYHUMBLE, MM_PLAY_LOOP );

    s->crows = (mm_sound_effect){
        { SFX_CROWS } ,            // id
        (int)(1.0f * (1<<10)),    // rate
        0,        // handle
        255,    // volume
        0,        // panning
    };

    s->invwhistle = (mm_sound_effect){
        { SFX_INVERSEWHISTLE } ,// id
        (int)(1.0f * (1<<10)),    // rate
        0,        // handle
        255,    // volume
        255,    // panning
    };

    s->crows_handle = 0;
}

//
// Update sound
//
void ctUpdateSound(ct_sound *s, int keys_pressed, int keys_released) {
    // Play looping crows sound effect out of left speaker if A button is pressed
    if ( keys_pressed & KEY_A ) {
        s->crows_handle = mmEffectEx(&s->crows);
    }

    // stop crows sound when A button is released
    if ( keys_released & KEY_A ) {
        mmEffectCancel(s->crows_handle);
    }

    // Play explosion sound effect out of right speaker if B button is pressed
    if ( keys_pressed & KEY_B ) {
        mmEffectEx(&s->invwhistle);
    }
}


int main() {
    irqInit();

    ct_sound sound;
    ct_sprites sprites;

    ctInitSound(&sound);
    ctInitSprites(&sprites);

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
        ctUpdateSound(&sound, keys_pressed, keys_released);
    } while( 1 );
}
