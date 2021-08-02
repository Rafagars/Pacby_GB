#include "../inc/functions.h"

UINT8 lives = 3;
UINT8 swap;
UBYTE reached_end = 0;
const UBYTE spritesize = 8;
UINT8 walking = 0;
UBYTE flip = 0;
UBYTE attacking = 0;
UBYTE jumping = 0;
UBYTE onFloor = 1;
UINT8 floor_height = 112;
const UINT8 gravity = 4;
const UINT8 max_speed = 24;
UINT8 bkg_position_offset = 0;
UINT8 bkg_colscroll_counter = 0;
UINT8 bkg_columns_scrolled = 0;
const UINT8 stage_width = 152;
UINT8 next_vram_location = 0;
const unsigned char floorTile[1] = {0x28};
const UBYTE max_enemies = 2;

unsigned char windowmap[] = 
{
    0x1A,0x22,0x04,0x00,0x00,0x0D,0x19,0x13,0x18,0x1D,0x22,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01
};

void interruptLCD(){
    HIDE_WIN;
}

void turnOnSound(){
    NR52_REG = 0x8F; //Turn on the sound
    NR51_REG = 0x11; //Enable the sound channel
    NR50_REG = 0x77; //Increase the volume to its max
}

void turnOffSound(){
    NR52_REG = 0x00;
}

//Initialize all the default variables needed to start the game
void init(){
    SHOW_WIN;
    SHOW_BKG; 
    SHOW_SPRITES;
    DISPLAY_ON;
    turnOnSound();

    add_LCD(interruptLCD);
    enable_interrupts();
    set_interrupts(VBL_IFLAG | LCD_IFLAG);
}

void performDelay(UINT8 numloops){
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }
}

UBYTE checkCollision(GameCharacter* one, GameCharacter* two){
    return(one->x >= two->x && one->x <= two->x + two->width) && (one->y >= two->y && one->y <= two->y + two->height) || (two->x >= one->x && two->x <= one->x + one->width) && (two->y >= one->y && two->y <= one->y + one->height);
}

void movePlayerCharacter(GameCharacter* character, UINT8 x, UINT8 y){
    if (flip == 0){
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

void moveCharacter(GameCharacter* character, UINT8 x, UINT8 y){
        move_sprite(character->spriteID[0], x, y);
        move_sprite(character->spriteID[1], x + spritesize, y);
        move_sprite(character->spriteID[2], x, y + spritesize);
        move_sprite(character->spriteID[3], x + spritesize, y + spritesize);
}

void defaultSprite(){
        set_sprite_tile(0, 0);
        set_sprite_tile(2, 2);
        set_sprite_tile(1, 1);
        set_sprite_tile(3, 3);
}

void flipSprite(GameCharacter* character){
    set_sprite_prop(character->spriteID[0], S_FLIPX);
    set_sprite_prop(character->spriteID[1], S_FLIPX);
    set_sprite_prop(character->spriteID[2], S_FLIPX);
    set_sprite_prop(character->spriteID[3], S_FLIPX);
}

// Walking animation 
void playerAnimation(){       
        if(jumping == 0){
            if(walking == 0 || walking == 2){
                movePlayerCharacter(&player, player.x, player.y);
                defaultSprite();
            } else if (walking == 1){
                movePlayerCharacter(&player, player.x, player.y - 1);
                set_sprite_tile(1, 4);
                set_sprite_tile(3, 5);
            } else {
                movePlayerCharacter(&player, player.x, player.y - 1);
                set_sprite_tile(1, 6);
                set_sprite_tile(3, 7);
            }
        }
}

void attackAnimation(){
        set_sprite_tile(0, 12);
        set_sprite_tile(2, 14);
        set_sprite_tile(1, 13);
        set_sprite_tile(3, 15);
        performDelay(2);
        set_sprite_tile(0, 8);
        set_sprite_tile(2, 10);
        set_sprite_tile(1, 9);
        set_sprite_tile(3, 11);
        performDelay(2);
}

void setupPlayer(){
    player.x = 40;
    player.vel_x = 0;
    player.vel_y = 0;
    player.y = 104;
    player.width = 14;
    player.height = 16;

    //load player's sprites
    set_sprite_tile(0, 0);
    player.spriteID[0] = 0;
    set_sprite_tile(2, 2);
    player.spriteID[1] = 2;
    set_sprite_tile(1, 1);
    player.spriteID[2] = 1;
    set_sprite_tile(3, 3);
    player.spriteID[3] = 3;

    movePlayerCharacter(&player, player.x, player.y);

}

void setupEnemies(GameCharacter* enemy, UINT8 x, UINT8 y){
    enemy->x = x;
    enemy->y = y;
    enemy->width = 14;
    enemy->height = 14;

    //Load Sprite for the enemy
    set_sprite_tile(24, 24);
    enemy->spriteID[0] = 24;
    set_sprite_tile(26, 26);
    enemy->spriteID[1] = 26;
    set_sprite_tile(25, 25);
    enemy->spriteID[2] = 25;
    set_sprite_tile(27, 27);
    enemy->spriteID[3] = 27;

    moveCharacter(enemy, x, y);
}

void setupBackground(){
    set_bkg_data(37, 18, Tiles);
    set_bkg_tiles(0, 0, Background1Width, Background1Height, Background1);
}

// Check collision with the floor
void checkFloor(UINT8 newPlayerX, UINT8 newPlayerY){
    UINT16 indexTLx, indexTLy, tileIndexTL;

    indexTLx = (newPlayerX - 8 + bkg_position_offset)/ 8;
    indexTLy = (newPlayerY - 16)/ 8;
    tileIndexTL = Background1Width * indexTLy + indexTLx;

    if (Background1[tileIndexTL] == floorTile[0]){
        onFloor = 1;
        floor_height = (indexTLy) * 8 + 8;
        //player.y = floor_height - player.height + 8;
    } else {
        onFloor = 0;
    }
}
UINT8 i;
//Update the tiles while moving
void updateCamera(){
    bkg_position_offset += 2;
    bkg_colscroll_counter += 2;
    scroll_bkg(2, 0);
    if(bkg_colscroll_counter == spritesize){
        bkg_colscroll_counter = 0;
        if(bkg_columns_scrolled < stage_width){
            // Only continue to render stage if there is stage left to render
            set_bkg_tiles(next_vram_location, 0, 1, 18, floormap_full[bkg_columns_scrolled]);
            for(i = 0; i < 18; i++){
                Background1[(i * 32) + next_vram_location] = floormap_full[bkg_columns_scrolled][i]; 
            }
            bkg_columns_scrolled += 1;
            if(bkg_columns_scrolled + 12 == stage_width){
                reached_end = 1;
            }
            next_vram_location++;
            if(next_vram_location == 32){ // Screen width limit
                next_vram_location = 0;
            }
        }
    }
}

//Creates a fade out effect
void fadeOut(){
    for(i = 0; i < 4; i++){
        switch (i)
        {
        case 0:
            BGP_REG = 0xE4;
            break;
        case 1:
            BGP_REG = 0xF9;
            break;
        case 2:
            BGP_REG = 0xFE;
            break;
        case 3:
            BGP_REG = 0xFF;
            break;
        }
        performDelay(10);
    }
}

//Creates a fade in effect
void fadeIn(){
    for(i = 0; i < 3; i++){
        switch (i)
        {
        case 0:
            BGP_REG = 0xFE;
            break;
        case 1:
            BGP_REG = 0xF9;
            break;
        case 2:
            BGP_REG = 0xE4;
            break;
        }
        performDelay(10);
    }
}

void resetBackground(){
    HIDE_WIN;
    move_bkg(0, 0); //Moves background to its default position
    HIDE_SPRITES;
}

void pauseScreen(){
    resetBackground();
    turnOffSound();
    set_bkg_tiles(0, 0, PauseWidth, PauseHeight, Pause);
    performDelay(10);
    waitpad(J_START);
    performDelay(10);
    turnOnSound();
    SHOW_SPRITES;
}

void gameOverScreen(){
    resetBackground();
    fadeIn();
    set_bkg_data(37, 12, Tiles);
    set_bkg_tiles(0, 0, GameOverWidth, GameOverHeight, GameOver);
}