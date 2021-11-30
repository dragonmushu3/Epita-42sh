CPPFLAGS = -I src/lexer -I src/parser -I src/ast -I src/utils -I src/exec -MMD
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -D_DEFAULT_SOURCE -fsanitize=address -g
LDFLAGS = -fsanitize=address
LDLIBS =

PRG = 42sh

VPATH = src/ast src/lexer src/parser tests src/42sh src/exec

SRC = lexer.c ast.c parser.c token.c 42sh.c exec_ast.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

42sh: $(OBJ)






.PHONY: clean

clean:
	$(RM) ${OBJ} ${DEP} ${PRG}
	$(RM) tests/*.o tests/*.d
