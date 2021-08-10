#include <gb/gb.h>
#include <gb/font.h>
#include "inc/functions.h"

void main(){

    //Choose font
    font_t ui_font;

    font_init();
    ui_font = font_load(font_min);
    font_set(ui_font);

    set_win_tiles(0, 0, 20, 1, windowmap);
    move_win(7, 136);
    set_sprite_data(0, 12, Pacby_sprite);
    setupPlayer();
    setupBackground();
    init();

    while(1){
        joyHandler();
        animations();
        moveCharacter(&player, player.x, player.y);
        if(player.x < 8){
            player.x = 8;
        }else if(player.x > 152){
            player.x = 152;
        }
        checkFloor(player.x, player.y);
        if(jumping && player.y != floor_height){
            player.y += 4;
        } else {
            player.y += 0; 
            jumping = FALSE;
        }
        wait_vbl_done();
    }
}