OBJS = main.o common.o geometry.o vector2d.o surface.o\
       datamanager.o resourcemanager.o timer.o camera.o\
       system.o singletonmanager.o input.o\
       level.o actor.o actorfactory.o manager.o color.o\
       bulletshape.o bullet.o item.o font.o levelreader.o\
       player.o playerinterface.o weapongrid.o\
       background.o levelselector.o\
       menu.o\
       enemies/enemies.o enemies/dark.o\
       enemies/simple.o enemies/strong.o enemies/bounce.o\
       enemies/attractor.o enemies/smart.o enemies/quick.o\
       enemies/group.o enemies/cloud.o\
       enemies/darksimple.o enemies/backandforth.o\
       enemies/darkattractor.o
FLAGS = -O0 -g -Wall -Wextra
#FLAGS = -O3 -Wall -Wextra
LINKS = -lm -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer
#LINKS = -lm -lSDL -lSDL_image -lSDL_mixer
NAME = ixthil

${NAME}.exe: ${OBJS}
	g++ -o ${NAME} ${OBJS} ${FLAGS} ${LINKS}

.cpp.o: $<
	-g++ ${FLAGS} -c -o $@ $<