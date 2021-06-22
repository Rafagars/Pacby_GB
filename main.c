#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/font.h>
#include "inc/functions.h"
#include "src/Pacby_tittleScreen_data.c"
#include "src/Pacby_tittleScreen_map.c"

UBYTE number_of_enemies = 0;
UBYTE absorbing = 0;

void main(){

    //Choose font
    font_t min_font;

    //Set Up the title screen
    set_bkg_data(0, 122, Pacby_tittleScreen_data);
    set_bkg_tiles(0, 0, 20, 18, Pacby_tittleScreen_map);

    SHOW_BKG;
    DISPLAY_ON;

    waitpad(J_START);

    fadeOut();

    fadeIn();

    //STAT_REG = 0x45;
    //LYC_REG = 0x08;  //Fire LCD Interrupt on the 8th scan line (just first row)

    disable_interrupts();
    
    //Set font
    font_init();
    min_font = font_load(font_min); //36 tile
    font_set(min_font);
    
    //Initialize all the defaults variables needed to start the game
    setupBackground();
    set_win_tiles(0, 0, 20, 1, windowmap);
    move_win(7, 128);
    set_sprite_data(0, 36, Pacby);
    setupPlayer();
    init();

    while(1){
        SHOW_WIN;
        if((joypad() & J_LEFT)){
            walking++;
            if (player.x < 9){
                player.x = 8;
            } else {
                player.x -= 2;
                if(player.vel_x > 0){
                    player.vel_x--;
                }
            }
            if (flip == 0) {
                //Flip the character to the left
                flipSprite(&player);
            }           
            flip = 1;
            //Walking Animation
            playerAnimation();
            
        } else if ((joypad() & J_RIGHT)){
            walking++;
            if(reached_end == 0){
                if(player.vel_x < max_speed ){
                    player.x += 2;
                    player.vel_x++;
                }
            } else {
                player.x += 2;
                if(player.x > 152){
                    player.x = 152;
                }
            }

            if (flip == 1) {
                //Restore the default sprite
                set_sprite_prop(0, 0);
                set_sprite_prop(1, 0);
                set_sprite_prop(2, 0);
                set_sprite_prop(3, 0);
            }
            if(!reached_end){
                //update the tiles while moving
                updateCamera();
            }

            flip = 0;
            //Walking Animation
            playerAnimation();   
        }
        if((joypad() & J_B)){
            attacking = 1;
            attackAnimation();
        } 
        if((joypad() & J_A) && jumping == 0){
            onFloor = 0;
            // Jumping
            jumping = 1;
            player.y -= 36;
            // Jump sfx
            NR11_REG = 0x1F;
            NR12_REG = 0xF1;     
            NR13_REG = 0x30;
            NR14_REG = 0xC4;
        }
        if(onFloor == 0){
            // Falling because of the gravity
            set_sprite_tile(1, 6);
            set_sprite_tile(3, 5);
            player.y += gravity;
        }
        performDelay(3);
        if(walking > 2 || walking < 0){
            walking = 0;
        }
        defaultSprite();
        movePlayerCharacter(&player, player.x, player.y);
        checkFloor(player.x, player.y + 8);
        if(number_of_enemies < max_enemies){
            setupEnemies(&enemies[number_of_enemies], 160, 112);
            number_of_enemies++;
        }
        for(INT8 i = 0; i < 2; i++){
            if(walking > 0 || attacking > 0){
                enemies[i].x -= 3;
            } else {
                enemies[i].x -= 1;
            }
            moveCharacter(&enemies[i], enemies[i].x, enemies[i].y );
            if(attacking > 0){
                if(flip < 1){
                    absorbing = player.x + 8 >= enemies[i].x;
                } else {
                    absorbing = player.x - 8 <= enemies[i].x;
                }
            }
        }
        wait_vbl_done();
    }
}