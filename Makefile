CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -D_DEFAULT_SOURCE 
LDFLAGS =
LDLIBS =

PRG = 42sh

VPATH = src

SRC =
OBJ = $(addprefix src/,${SRC:.c=.o})
DEP = $(addprefix src/,${SRC:.c=.d})


.PHONY: clean

clean:
	$(RM) ${OBJ} ${DEP} ${PRG}
