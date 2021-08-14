#include "../inc/characters.h"

uint8_t i;
const uint8_t spritesize = 8;
uint8_t frame = 0;
uint8_t id;
uint8_t timer = 5;

bool step = FALSE;
bool jumping = FALSE;
bool attack = FALSE;

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

void setupApples(){
    Entity* apple = apples;
    for(i = 0; i < 3; i++){
        apple->x = 84 + 12*i;
        apple->y = 92;
        apple->h = 8;
        apple->w = 8;
        apple->health = 1;

        // Load apple's sprite
        set_sprite_tile(4 + i, 13);
        apple->spriteID[0] = 4+i;

        move_sprite(apple->spriteID[0], apple->x, apple->y);
        apple++;
    }
}

void updateApples(uint8_t pos){
    Entity* apple = apples;
    for(i = 0; i < 3; i++){
        switch(pos){
            case 1:
                apple->x += 2;
                break;
            case 2:
                apple->x -= 2;
                break;
            default:
                apple->x += 0;
        }
        move_sprite(apple->spriteID[0], apple->x, apple->y);
        if(apple->health > 0 && checkCollision(&player, apple)){
            apple->health = 0;
            apple->x = 0;
            apple->y = 160;
        }
        apple++;
    }
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
    set_bkg_data(37, 21, Tiles);
    map_pos_x = map_pos_y = 0;
    old_map_pos_x = old_map_pos_y = 255;
    set_bkg_submap(map_pos_x, 0, 20, 18, BG1, BG1Width);

    camera_x = camera_y = 0;
    old_camera_x = camera_x;
    old_camera_y = camera_y;

    redraw = FALSE;

    SCX_REG = camera_x;
    SCY_REG = camera_y;
}