#include "../inc/functions.h"

bool game_on = TRUE;
uint8_t floor_height = 105;
const uint8_t stage_width = 152;
const unsigned char floorTiles[3] = {0x28, 0x29, 0x2A};

uint16_t camera_x, camera_y, old_camera_x, old_camera_y;
uint8_t map_pos_x, map_pos_y, old_map_pos_x, old_map_pos_y;
bool redraw;

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
    // Check the frame for the walking animation
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
    //
    if(joypad() & J_LEFT){
        if(player.x > 40 || !camera_x){
            player.x -= 2;
        } else {
            if(camera_x){
            camera_x -= 2;
            redraw  = TRUE;
            }
        }
        player.flip = TRUE;

    } else if(joypad() & J_RIGHT){
        if(player.x < 80 || camera_x > camera_max_x){
            player.x += 2;
        } else {
            camera_x+=2;
            if(camera_x < camera_max_x){
                redraw = TRUE;
            }
        }
        player.flip = FALSE;
    } else {
        frame = 0; //Default. Idle sprite
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

    indexTLx = (newPlayerX + camera_x)/8;
    indexTLy = (newPlayerY)/8;
    tileIndexTL = BG1Width * indexTLy + indexTLx;

    if(BG1[tileIndexTL] == floorTiles[0] || BG1[tileIndexTL] == floorTiles[1] || BG1[tileIndexTL] == floorTiles[2]){
        floor_height = (indexTLy * 8) + 1;
        jumping = FALSE;
    } else {
        jumping = TRUE;
        floor_height = 105;
    }
}

void set_camera(){
    SCY_REG = camera_y; SCX_REG = camera_x;
    map_pos_x = (uint8_t)(camera_x >> 3u);
    if(map_pos_x != old_map_pos_x){
        if(camera_x < old_camera_x){
            set_bkg_submap(map_pos_x, map_pos_y, 1, MIN(19u, BG1Height - map_pos_y), BG1, BG1Width);
        } else {
            if((BG1Width - 20u) > map_pos_x){
                set_bkg_submap(map_pos_x + 20u, map_pos_y, 1, MIN(19u, BG1Height - map_pos_y), BG1, BG1Width);
            }
        }
        old_map_pos_x = map_pos_x;
    }
    old_camera_x = camera_x;
    old_camera_y = camera_y;
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

