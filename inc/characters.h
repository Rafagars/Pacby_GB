#ifndef _CHARACTERS_H
#define _CHARACTERS_H

#include "functions.h"

extern const UWORD spritePalette[];
extern const UWORD BG1Pallete[];

extern uint8_t i;
extern const uint8_t spritesize;
extern uint8_t frame;
extern uint8_t id;
extern uint8_t timer;

extern bool step;
extern bool jumping;
extern bool attack;

extern UBYTE checkCollision(Entity* one, Entity* two);
extern void moveCharacter(Entity* character, uint8_t x, uint8_t y);
extern void setupPlayer();
extern void setupApples();
extern void updateApples(uint8_t pos);
extern void setupBackground();
extern void flipSprite(Entity* character);
extern void animations();

#endif