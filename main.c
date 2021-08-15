#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/font.h>
#include "inc/functions.h"

void main(){
    
    //Choose font
    font_t ui_font;

    font_init();
    ui_font = font_load(font_min);
    font_set(ui_font);


    if(_cpu == CGB_TYPE){ //Check if it's a Game Boy Color
        //Set colors palletes
        VBK_REG = 1;
        set_bkg_palette(0, 4, &BG1Pallete[0]);
        set_bkg_tiles(0, 0, BG1Height, BG1Width, BG1PLN1);
        set_win_tiles(0, 0, 20, 1, windowpalette); 
        set_sprite_palette(0, 3, &spritePalette[0]);
        VBK_REG = 0;
    }
    setupBackground();
    set_win_tiles(0, 0, 20, 1, windowmap);
    move_win(7, 136);
    set_sprite_data(0, 14, Pacby_sprite);
    setupPlayer();
    setupApples();
    init();

    while(1){
        joyHandler();
        animations();
        moveCharacter(&player, player.x, player.y);
        checkFloor(player.x, player.y);
        if(player.x < 8){
            player.x = 8;
        }else if(player.x > 152){
            player.x = 152;
        }
        if(player.y != floor_height || (joypad() & J_DOWN) && player.y != 105){
            jumping = TRUE;
            player.y += 4;
        } else {
            player.y += 0; 
            jumping = FALSE;
        }
        if(redraw){
            wait_vbl_done();
            set_camera();
            redraw = FALSE;
        } else {
            wait_vbl_done();
        }
    }
}