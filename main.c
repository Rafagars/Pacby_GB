#include <gb/gb.h>
#include <gb/cgb.h>
#include "inc/functions.h"

UBYTE number_of_enemies = 0;

void main(){
    
    //Initialize all the defaults variables needed to start the game
    init();
    setupBackground();
    set_sprite_data(0, 36, Pacby);
    setupPlayer();

    while(1){
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
                updateCamera();
            }

            flip = 0;
            //Walking Animation
            playerAnimation();   
        }
        if((joypad() & J_B)){
            attackAnimation();
        } 
        if((joypad() & J_A) && jumping == 0){
            onFloor = 0;
            // Jumping
            jumping = 1;
            player.y -= 28;
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
        moveCharacter(&player, player.x, player.y);
        checkFloor();
        
        wait_vbl_done();
    }
}