/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o src/Pacby_sprites.o src/Pacby_sprites.c 
/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o src/Tiles.o src/Tiles.c 
/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o src/entity.o src/entity.c 
/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o src/BG1.o src/BG1.c
/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o src/functions.o src/functions.c
/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o src/characters.o src/characters.c
/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o src/windowLayer.o src/windowLayer.c
/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o main.o main.c
/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -Wm-yn"PACBY" -o pacby.gb src/Pacby_sprites.o main.o src/functions.o src/characters.o src/Tiles.o src/entity.o src/BG1.o src/windowLayer.o

rm *.o *.lst *.asm *.sym

cd src

rm *.o *.lst *.asm *.sym


