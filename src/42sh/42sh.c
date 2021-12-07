#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>

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

    print_ast(ast);
    printf("\n");
    int res_exec = exec_ast(ast);
    if (res_exec == 666)
        return 666;

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

    int pretty_print = 0;
    int command = 0;
    int option_index = 0;
    int opt = 0;
    static struct option long_options[] =
    {
        {"pretty_print", no_argument, &pretty_print, 1},
        {0, 0, 0, 0}
    };

    while (opt = getopt_long(argc, argv, "c", long_options, &option_index) != -1)
    {

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
