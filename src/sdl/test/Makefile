# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD 
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS= 
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

all: display

display: display.o
grayscale: pixel_operations.o grayscale.o


clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} grayscale

# END
