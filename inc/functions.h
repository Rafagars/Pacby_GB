#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>
#include "entity.h"
#include "characters.h"
#include "windowLayer.h"
#include "Pacby_sprites.h"
#include "Tiles.h"
#include "Background1.h"
#include "floormap_full_segments.h"

extern bool game_on;

extern void interruptLCD();
extern void turnOnSound();
extern void turnOffSound();
extern void init();
extern void performDelay(uint8_t numloops);

extern void joyHandler();

extern void updateLives();
extern void fadeOut();
extern void fadeIn();
extern void resetBackground();

extern void updateCamera();

extern void hitSound();
extern void coinSound();
extern void gameOverScreen();

#endif