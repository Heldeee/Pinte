# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image`

all: main

main: filtre.o 

filtre.o: filtre.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} Results/*.bmp

# END
