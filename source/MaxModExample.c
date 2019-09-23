#include <gba.h>
#include <maxmod.h>

#include <stdio.h>
#include <stdlib.h>

#include "soundbank.h"
#include "soundbank_bin.h"


//////////////////////////////////////////////
//                  Sprites
//////////////////////////////////////////////
void ctInitSprites() {
    // Shadow of the OAM TODO put in struct or what
    OBJATTR obj_buffer[128];
    OBJAFFINE *const obj_aff_buffer = (OBJAFFINE*)obj_buffer;
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

    s->crows_handle = NULL;
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

    ctInitSound(&sound);

    // Main loop
    do {
        // Variables
        int keys_pressed, keys_released;

        // Frame stuff
        VBlankIntrWait();
        mmFrame();

        // Grab input
        scanKeys();

        keys_pressed = keysDown();
        keys_released = keysUp();

        // Run update functions
        ctUpdateSound(&sound, keys_pressed, keys_released);
    } while( 1 );
}
