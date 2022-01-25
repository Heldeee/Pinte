CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -Werror -std=c99 
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lSDL_gfx -lm
EXEC=filters

SRC= filters.c main.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: ${EXEC}

-include ${DEP}

${EXEC}: ${OBJ}
	${CC} ${LDLIBS} ${LDFLAGS} -o $@ $^


clean:
	rm -f ${OBJ} ${EXEC}
