#
#Makefile

CC = gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS = `pkg-config --cflags gtk+-3.0` -O3
LDLIBS = `pkg-config --libs gtk+-3.0` `pkg-config --libs sdl SDL_image` -lSDL_image -lSDL_gfx -lm -lpng

EXE = main

all: $(EXE)

main: main.o app_ds.o savepng.o filtre.o

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} *.o
	${RM} *.d
	${RM} $(EXE)

# END
