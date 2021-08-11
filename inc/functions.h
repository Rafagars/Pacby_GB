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
extern uint8_t floor_height;
extern bool reached_end;
extern uint8_t bkg_position_offset;
extern uint8_t bkg_colscroll_counter;
extern uint8_t bkg_columns_scrolled;
extern const uint8_t stage_width;
extern uint8_t next_vram_location;
extern const unsigned char floorTiles[3];

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

extern void checkFloor(uint8_t newPlayerX, uint8_t newPlayerY);
extern void updateCamera();

extern void jump_sfx();
extern void coinSound();
extern void gameOverScreen();

#endif