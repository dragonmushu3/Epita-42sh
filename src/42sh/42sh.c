#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../ast/ast.h"
#include "../exec/exec_ast.h"

static int parse_and_exec(char *line, int pretty_print, int no_exec)
{
    struct lexer *lexer = lexer_new(line);

    struct ast *ast;
    enum parser_status status = parse(&ast, lexer);
    if (status != PARSER_OK)
    {
        lexer_free(lexer);
        return 1;
    }

    if (pretty_print)
    {
        print_ast(ast);
        printf("\n");
    }

    if (!no_exec)
    {
        int res_exec = exec_ast(ast);
        if (res_exec == 666)
            return 666;
    }

    ast_free(ast);
    lexer_free(lexer);

    return 0;
}

int main(int argc, char *argv[])
{
    int pretty_print_flag = 0;
    int no_exec_flag = 0;
    int read_string_flag = 0;

    int option_index = 0;
    int opt = 0;
    static struct option long_options[] =
    {
        {"pretty-print", no_argument, 0, 2},
        {"no-exec", no_argument, 0, 3},
        {0, 0, 0, 0}
    };

    while (opt != -1)
    {
        opt = getopt_long(argc, argv, "c", long_options, &option_index);
        if (opt == 2)
            pretty_print_flag = 1;
        if (opt == 'c')
            read_string_flag = 1;
        if (opt == 3)
            no_exec_flag = 1;
    }

    if (!read_string_flag && argv[optind])
    {
        //parse and execute text from file in argv[optind] FIX ME
        char *line = NULL;
        size_t n = 0;
        int nread = 0;

        FILE *file = fopen(argv[optind], "r");
        if (!file)
            err(1, "0");
        while ((nread = getline(&line, &n, file)) != -1)
        {
            if (parse_and_exec(line, pretty_print_flag, no_exec_flag) == -1)
                errx(1, "parsing failed: %.*s", nread, line);
        }
        free(line);
        fclose(file);
        return 0;
    }
    else if (read_string_flag)
    {
        //parse and execute the string argument in argv[2]
        if (parse_and_exec(argv[optind], pretty_print_flag, no_exec_flag) == -1)
            errx(1, "parsing failed: %s", argv[optind]);
        return 0;
    }
    else
    {
        //read stdin and parse the lines
        printf("42sh$ ");
        char *line = NULL;
        size_t n = 0;
        int nread = 0;
        while ((nread = getline(&line, &n, stdin)) != -1)
        {
            /*parse and execute lines FIX ME*/
            if (parse_and_exec(line, pretty_print_flag, no_exec_flag) == -1)
                errx(1, "parsing failed: %.*s", nread, line);
            printf("42sh$ ");
        }
        free(line);
        return 0;
    }
}
