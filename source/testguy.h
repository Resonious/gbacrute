#ifndef TESTGUY_H
#define TESTGUY_H

#include "hardware.h"
#include "sprites.h"

#define GUY_SPEED (20)
#define GUY_ACCEL (4)
#define GUY_SPEED_FACTOR (5)

typedef struct ka_test_guy {
    int dx, dy;   // velocity
    int ddx, ddy; // acceleration
    int mx, my;   // "moving"
    SPRITE *spr;
} ka_test_guy;

void kaInitTestGuy(ka_test_guy *, ka_sprites *);
void kaUpdateTestGuy(ka_test_guy *, ka_sprites *, KEYS keys);

#endif
