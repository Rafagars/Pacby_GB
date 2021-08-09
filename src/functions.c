#include "../inc/functions.h"

bool game_on = TRUE;
bool reached_end = FALSE;
uint8_t bkg_position_offset = 0;
uint8_t bkg_colscroll_counter = 0;
uint8_t bkg_columns_scrolled = 0;
const uint8_t stage_width = 152;
uint8_t next_vram_location = 0;


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
    switch(joypad())
    {
        case J_LEFT:
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
            break;
        case J_RIGHT:
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
            break;
        case J_UP:
            break;
        case J_DOWN:
            break;
        case J_A:
            break;
        case J_B:
            break;
        case J_START:
            break;
        default:
            player.x += 0;
            frame = 0;
            break;    
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
                Background1[(i * 32 )  + next_vram_location] =  floormap_full[bkg_columns_scrolled][i];
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

void resetBackgrounds(){
    HIDE_WIN;
    move_bkg(0,0); // Moves background to its default position
    HIDE_SPRITES;
}

