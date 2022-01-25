# Makefile

CC = gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -O3
LDLIBS = `pkg-config --libs gtk+-3.0` `pkg-config --libs sdl` -lSDL_image -lSDL_gfx -lm

EXE = main

all: $(EXE)

main: display.o rotations.o otsu.o pixel_operations.o

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} *.o
	${RM} *.d
	${RM} $(EXE)

# END
