CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -D_DEFAULT_SOURCE -fsanitize=address -g
LDFLAGS = -fsanitize=address
LDLIBS =

PRG = 42sh

VPATH = src tests

SRC = lexer.c ast.c parser.c token.c ast_print.c
OBJ = $(addprefix src/,${SRC:.c=.o})
DEP = $(addprefix src/,${SRC:.c=.d})


ast_print: $(OBJ)








.PHONY: clean

clean:
	$(RM) ${OBJ} ${DEP} ${PRG} main lexer_test_main
	$(RM) tests/*.o tests/*.d
