#include "../inc/characters.h"

uint8_t i;
const uint8_t spritesize = 8;
uint8_t frame = 0;
uint8_t id;
uint8_t timer = 5;

bool step = FALSE;
bool jumping = FALSE;

UBYTE checkCollision(Entity* one, Entity* two){
    return(one->x >= two->x && one->x <= two->x + two->w) && (one->y >= two->y && one->y <= two->y + two->h) || (two->x >= one->x && two->x <= one->x + one->w) && (two->y >= one->y && two->y <= one->y + one->h);
}

void moveCharacter(Entity* character, uint8_t x, uint8_t y){
    if (!character->flip){
        move_sprite(character->spriteID[0], x, y);
        move_sprite(character->spriteID[1], x + spritesize, y);
        move_sprite(character->spriteID[2], x, y + spritesize);
        move_sprite(character->spriteID[3], x + spritesize, y + spritesize);
        
    } else {
        move_sprite(character->spriteID[0], x + spritesize, y);
        move_sprite(character->spriteID[1], x, y);
        move_sprite(character->spriteID[2], x + spritesize, y + spritesize);
        move_sprite(character->spriteID[3], x, y + spritesize);
    }
}

void setupPlayer(){
    player.x = 24;
    player.y = 105;
    player.h = 16;
    player.w = 16;
    player.health = 3;
    player.flip = FALSE;

    //load player's sprites
    set_sprite_tile(0, 0);
    player.spriteID[0] = 0;
    set_sprite_tile(2, 0);
    player.spriteID[1] = 2;
    set_sprite_tile(1, 1);
    player.spriteID[2] = 1;
    set_sprite_tile(3, 3);
    player.spriteID[3] = 3;

    moveCharacter(&player, player.x, player.y);
}

void flipSprite(Entity* character){
    set_sprite_prop(character->spriteID[0], S_FLIPX);
    set_sprite_prop(character->spriteID[1], S_FLIPX);
    set_sprite_prop(character->spriteID[2], S_FLIPX);
    set_sprite_prop(character->spriteID[3], S_FLIPX);
}

void animations(){
    uint8_t n = 3*frame + frame;
    
    performDelay(3);
    if(player.flip){
        flipSprite(&player);
    } else {
        set_sprite_tile(0, 0);
        set_sprite_tile(2, 2);
        set_sprite_tile(1, 1);
        set_sprite_tile(3, 3);
        set_sprite_prop(0, 0);
        set_sprite_prop(2, 0);
        set_sprite_prop(1, 0);
        set_sprite_prop(3, 0);
    }
    
    if(jumping){
        set_sprite_tile(0, 8);
        set_sprite_tile(1, 9);
        set_sprite_tile(2, 10);
        set_sprite_tile(3, 11);
    } else {
        set_sprite_tile(0, 0+n);
        set_sprite_tile(1, 1+n);
        set_sprite_tile(2, 2+n);
        set_sprite_tile(3, 3+n);
    }
}

void setupBackground(){
    set_bkg_data(37, 20, Tiles);
    set_bkg_tiles(0, 0, Background1Width, Background1Height, Background1);
}