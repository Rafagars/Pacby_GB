#include "../inc/functions.h"

bool game_on = TRUE;
uint8_t floor_height = 105;
bool reached_end = FALSE;
uint8_t bkg_position_offset = 0;
uint8_t bkg_colscroll_counter = 0;
uint8_t bkg_columns_scrolled = 0;
const uint8_t stage_width = 152;
uint8_t next_vram_location = 0;
const unsigned char floorTiles[3] = {0x28, 0x29, 0x2A};


void interruptLCD(){
    HIDE_WIN;
}

void turnOnSound(){
    NR52_REG = 0x80; // Turn on the sound
    NR51_REG = 0x11; // Enable the sound channels
    NR50_REG = 0x77; //Increase the volume to its max
}

void turnOffSound(){
    NR52_REG = 0x00;
}

// Initilize all the default variable to start the game
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

void performDelay(uint8_t numloops){
    for(uint8_t i = 0; i < numloops; i++){
        wait_vbl_done();
    }
}

void joyHandler(){

    if(joypad() & J_LEFT){
        player.x -= 2;
        player.flip = TRUE;
        if(frame == 2){
            step = TRUE;
        } else if(frame == 0){
            step = FALSE;
        }
        if(step){
            frame--;
        } else {
            frame++;
        }
    } else if(joypad() & J_RIGHT){
        if(player.x < 80 || (reached_end)){
            player.x += 2;
        } else {
            updateCamera();
        }
        player.flip = FALSE;
        if(frame == 2){
            step = TRUE;
        } else if(frame == 0){
            step = FALSE;
        }
        if(step){
            frame--;
        } else {
            frame++;
        }
    } else {
        frame = 0;
        player.x += 0;
    }
    if ((joypad() & J_A) && !jumping){
        //jumping = TRUE;
        player.y -= 24;
        jump_sfx();
    }
    if(joypad() & J_B){
        attack = TRUE;
        set_sprite_tile(0, 4);
        set_sprite_tile(2, 12);
    }
}

// Check collision with the floor
void checkFloor(uint8_t newPlayerX, uint8_t newPlayerY){
    uint16_t indexTLx, indexTLy, tileIndexTL;

    indexTLx = (newPlayerX + bkg_position_offset)/8;
    indexTLy = (newPlayerY)/8;
    tileIndexTL = Background1Width * indexTLy + indexTLx;

    if(Background1[tileIndexTL] == floorTiles[0] || Background1[tileIndexTL] == floorTiles[1] || Background1[tileIndexTL] == floorTiles[2]){
        floor_height = (indexTLy * 8) + 1;
        jumping = FALSE;
    } else {
        jumping = TRUE;
        floor_height = 105;
    }
}

// Update the tiles while moving
void updateCamera(){
    bkg_position_offset += 2;
    bkg_colscroll_counter += 2;
    scroll_bkg(2, 0);
    if(bkg_colscroll_counter == spritesize){
        bkg_colscroll_counter = 0;
        if(bkg_columns_scrolled < stage_width){
            // Only continue to render the stage if there any stage left to render
            set_bkg_tiles(next_vram_location, 0, 1, 18, floormap_full[bkg_columns_scrolled]);
            for(i = 0; i < 18; i++){
                uint8_t tileIndex = (i * 32)  + next_vram_location; // It would be helpful for the apple placement
                Background1[tileIndex] =  floormap_full[bkg_columns_scrolled][i];
            }
            bkg_columns_scrolled += 1;
            if(bkg_columns_scrolled + 12 == stage_width){
                reached_end = TRUE;
            }
            next_vram_location++;
            if(next_vram_location == 32){ // Screen width limit
                next_vram_location = 0;
            }
        }
    }
}

void fadeOut(){
    for(uint8_t i = 0; i < 4; i++){
        switch(i)
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

void jump_sfx(){
    // Jump sfx
    NR11_REG = 0x1F;
    NR12_REG = 0xF1;     
    NR13_REG = 0x30;
    NR14_REG = 0xC4;
}

void resetBackgrounds(){
    HIDE_WIN;
    move_bkg(0,0); // Moves background to its default position
    HIDE_SPRITES;
}

