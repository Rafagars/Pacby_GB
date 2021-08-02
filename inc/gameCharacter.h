#ifndef _GAMECHARACTER_H
#define _GAMECHARACTER_H

#include <gb/gb.h>

typedef struct GameCharacter{
    UBYTE spriteID[4]; //All characters use max 4 sprites
    UINT8 x;
    UINT8 vel_x;
    INT8 vel_y;
    UINT8 y;
    UINT8 width;
    UINT8 height;
} GameCharacter;

extern GameCharacter player;

extern GameCharacter enemies[2];

#endif