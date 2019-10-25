#ifndef TESTGUY_H
#define TESTGUY_H

#include "hardware.h"
#include "sprites.h"

typedef struct ka_test_guy {
    int x, y;
    SPRITE *spr;
} ka_test_guy;

void kaInitTestGuy(ka_test_guy *, ka_sprites *);
void kaUpdateTestGuy(ka_test_guy *);
#endif
