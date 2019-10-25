#include <gba.h>
#include <maxmod.h>

#include <stdio.h>
#include <stdlib.h>

#include "soundbank.h"
#include "soundbank_bin.h"

#include "hardware.h"
#include "sprites.h"
#include "testguy.h"


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
    mmStart( MOD_INTERNALMECHANICS, MM_PLAY_LOOP );

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
    ka_test_guy test;

    kaInitSound(&sound);
    kaInitSprites(&sprites);
    kaInitTestGuy(&test, &sprites);

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
        kaUpdateTestGuy(&test);
    } while( 1 );
}
