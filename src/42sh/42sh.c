#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../ast/ast.h"
#include "../exec/exec_ast.h"

static int parse_and_exec(char *line)
{
    struct lexer *lexer = lexer_new(line);

    struct ast *ast;
    enum parser_status status = parse(&ast, lexer);
    if (status != PARSER_OK)
    {
        lexer_free(lexer);
        return 1;
    }

    //print_ast(ast);
    exec_ast(ast);

    ast_free(ast);
    lexer_free(lexer);

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        //read stdin and parse the lines
        printf("42sh$ ");
        char *line = NULL;
        size_t n = 0;
        int nread = 0;
        while ((nread = getline(&line, &n, stdin)) != -1)
        {
            /*parse and execute lines FIX ME*/
            if (parse_and_exec(line) == -1)
                errx(1, "parsing failed: %.*s", nread, line);
            printf("42sh$ ");
        }
        free(line);
        return 0;
    }

    if (strcmp(argv[1], "-c") != 0)
    {
        //parse and execute text from file in argv[1] FIX ME
        char *line = NULL;
        size_t n = 0;
        int nread = 0;

        FILE *file = fopen(argv[1], "r");
        if (!file)
            err(1, "0");
        while ((nread = getline(&line, &n, file)) != -1)
        {
            if (parse_and_exec(line) == -1)
                errx(1, "parsing failed: %.*s", nread, line);
        }
        free(line);
        fclose(file);
        return 0;
    }
    else
    {
        //parse and execute the string argument in argv[2]
        if (parse_and_exec(argv[2]) == -1)
            errx(1, "parsing failed: %s", argv[2]);
        return 0;
    }
}
