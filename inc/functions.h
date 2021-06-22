#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <gb/gb.h>
#include <stdio.h>
#include "gameCharacter.h"
#include "Pacby.h"
#include "Tiles.h"
#include "Background1.h"
#include "floormap_full_segments.h"

extern UBYTE reached_end;
extern const UBYTE spritesize;
extern int walking;
extern UBYTE flip;
extern UBYTE attacking;
extern UBYTE jumping;
extern UBYTE onFloor;
extern const UINT8 gravity;
extern const UINT8 max_speed;
extern UINT8 bkg_position_offset;
extern UINT8 bkg_colscroll_counter;
extern UINT8 bkg_columns_scrolled;
extern const UINT8 stage_width;
extern UINT8 next_vram_location; 
extern const unsigned char floorTile[1];
extern const UBYTE max_enemies;
extern int xOrder;

extern unsigned char windowmap[];

extern void interruptLCD();

extern void turnOnSound();

extern void turnOffSound();

extern void init();

extern void performDelay(UINT8 numloops);

extern UBYTE checkCollision(GameCharacter* one, GameCharacter* two);

extern void movePlayerCharacter(GameCharacter* character, UINT8 x, UINT8 y);

extern void moveCharacter(GameCharacter* character, UINT8 x, UINT8 y);

extern void defaultSprite();

extern void flipSprite(GameCharacter* character);

extern void playerAnimation();

extern void attackAnimation();

extern void setupPlayer();

extern void setupEnemies(GameCharacter* enemy, UINT8 x, UINT8 y);

extern void setupBackground();

extern void checkFloor(UINT8 newPlayerX, UINT8 newPlayerY);

extern void updateCamera();

extern void fadeOut();

extern void fadeIn();

extern void resetBackground();

extern void pauseScreen();

extern void gameOverScreen();

#endif