CPPFLAGS = -I src/lexer -I src/parser -I src/ast -I src/utils -MMD
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -D_DEFAULT_SOURCE -fsanitize=address -g
LDFLAGS = -fsanitize=address
LDLIBS =

PRG = 42sh

VPATH = src/ast src/lexer src/parser tests src/42sh

SRC = lexer.c ast.c parser.c token.c 42sh.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

42sh: $(OBJ)

ast_print: $(OBJ)






.PHONY: clean

clean:
	$(RM) ${OBJ} ${DEP} ${PRG} main ast_print
	$(RM) tests/*.o tests/*.d
