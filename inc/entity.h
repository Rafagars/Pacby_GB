#ifndef _ENTITY_H
#define _ENTITY_H

#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NUMBER_OF_APPLES 10

typedef struct Entity
{
    uint8_t spriteID[4];
    uint8_t x;
    uint8_t y;
    uint8_t vel_x;
    uint8_t vel_y;
    uint8_t w;
    uint8_t h;
    uint8_t health;
    bool flip;
}Entity;

extern Entity player;

extern uint8_t number_of_apples;

extern Entity apples[MAX_NUMBER_OF_APPLES];

#endif