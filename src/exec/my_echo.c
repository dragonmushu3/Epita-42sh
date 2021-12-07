#include "../ast/ast.h"

#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <stddef.h>
#include <string.h>

static void wrapper(char *str, int e, int n,  int nb)
{    //function qui verifie les '\n'
    if (nb != 0)
        putchar(' ');
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '\n' && n)
        {
            i++;
            continue;
        }
        if (str[i] == '\\' && e)
        {
            switch(str[i + 1])
            {
            case 'n':
                    putchar('\n');
                    i += 2;
                    break;
            case 't':
                    putchar('\t');
                    i += 2;
                    break;
            default:
                    i++;
            }
        }
        else
        {
            putchar(str[i]);
            i++;
        }
    }
}

void my_echo(struct ast *ast, size_t i)
{
    int met_e = 0;
    int met_n = 0;
    int finish = 1;
    int nb_child = 0;
    //Check for option -n
    while (finish != 0)
    {
        if (!strcmp(ast->data[i], "-n"))
        {
            met_n = 1;
            i++;
        }
        if (!strcmp(ast->data[i], "-e"))
        {
            met_e = 1;
            i++;
        }
        finish = 0;
    }
    while (ast->data[i] != NULL)
    {
        wrapper(ast->data[i], met_e, met_n, nb_child);
        nb_child++;
        i++;
    }
    if (!met_n)
        putchar('\n');
}
