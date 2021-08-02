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
    //setupEnemies(&enemies[0], 170, 112);
    init();

    while(1){
        SHOW_WIN;
        if(checkCollision(&player, &enemies[0])){
            lives--;
            // Since windowmap is an array of characters, we transform the value we want to change to a integer
            swap = (int)windowmap[2] - 1;
            windowmap[2] = (char) swap;
            turnOffSound();
            fadeOut();
            if(lives < 1){
                gameOverScreen();
                lives = 3;
                //Changes the characters on the window layer to its default
                windowmap[2] = 0x04;
            }
            waitpad(J_A | J_B | J_START); // Press any of this buttons to continue
            fadeIn();
            turnOnSound();
            set_win_tiles(0, 0, 20, 1, windowmap);
            setupBackground(); // Restart background
            //Restart sprites
            SHOW_SPRITES;
            setupPlayer();
            //setupEnemies(&enemies[0], 170, 112);
        } else {
            if(joypad()& J_START){
                pauseScreen();
                setupBackground();
            }
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
                //attacking = 1;
                attackAnimation();
            }
            if((joypad() & J_A) && jumping == 0){
                // Jumping
                jumping = 1;
                player.y -= 32;
                // Jump sfx
                NR11_REG = 0x1F;
                NR12_REG = 0xF1;     
                NR13_REG = 0x30;
                NR14_REG = 0xC4;
            }
            /*checkFloor(player.x, player.y + 16);
            if(onFloor < 1){
                // Falling because of the gravity
                set_sprite_tile(1, 6);
                set_sprite_tile(3, 5);
                player.vel_y = gravity;
            }
            if(jumping == 1 && (player.y + player.height) >= floor_height ){
                jumping = 0;
                player.vel_y = 0;
                player.y = floor_height - player.height + 8;
            }
            player.y += player.vel_y;
            */
            performDelay(3);
            if(walking > 2){
                walking = 0;
            }
            defaultSprite();
            movePlayerCharacter(&player, player.x, player.y);
            /*
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
            */
            //moveCharacter(&enemies[0], enemies[0].x, enemies[0].y );
        }
        wait_vbl_done();
    }
}