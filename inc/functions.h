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
#include "BG1.h"

#define camera_max_x ((BG1Width - 20) * 8)

#define MIN(A,B) ((A)<(B)?(A):(B))

extern bool game_on;
extern uint8_t floor_height;
extern bool reached_end;
extern uint8_t bkg_position_offset;
extern uint8_t bkg_colscroll_counter;
extern uint8_t bkg_columns_scrolled;
extern const uint8_t stage_width;
extern uint8_t next_vram_location;
extern const unsigned char floorTiles[3];
extern uint16_t camera_x, camera_y, old_camera_x, old_camera_y;
extern uint8_t map_pos_x, map_pos_y, old_map_pos_x, old_map_pos_y;
extern bool redraw;

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
extern void set_camera();

extern void jump_sfx();
extern void coinSound();
extern void gameOverScreen();

#endif