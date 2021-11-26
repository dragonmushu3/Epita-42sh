CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -D_DEFAULT_SOURCE 
LDFLAGS =
LDLIBS =

PRG = 42sh

VPATH = src tests

SRC = main.c lexer.c
OBJ = $(addprefix src/,${SRC:.c=.o})
DEP = $(addprefix src/,${SRC:.c=.d})

main: $(OBJ)

ast_test_main: tests/ast_test_main.o src/lexer.o







.PHONY: clean

clean:
	$(RM) ${OBJ} ${DEP} ${PRG} main lexer_test_main
	$(RM) tests/*.o tests/*.d
