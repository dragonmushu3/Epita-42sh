#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>

#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
        errx(-1, "not enough arguments");

    if (strcmp(argv[1], "-c") != 0)
    {
        //read stdin
        char *line = NULL;
        size_t n = 0;
        int nread = 0;
        while ((nread = getline(&line, &n, stdin)) != -1)
        {
            printf("%.*s \n",nread, line);
        }
        free(line);
        argv++;
        return 0;
    }
    else
    {
        struct lexer *lexer = lexer_new(argv[2]);

        struct ast *ast;
        enum parser_status status = parse(&ast, lexer);
        if (status != PARSER_OK)
        {
            lexer_free(lexer);
            return 1;
        }
        printf("%d \n",ast->type);
        printf("%s \n",ast->data[0]);
        printf("\n");
        ast_free(ast);
        lexer_free(lexer);

        return 0;
    }
}
