#include "../ast/ast.h"

#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <stddef.h>
#include <string.h>

static void wrapper(char *str, int e, int n)
{    //function qui verifie les '\n'

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

//function qui verifie si string est parametre
/*static int is_param(char *str, int *e, int *n)
{
    if (!strcmp(str, "-n"))
    {
           (*n)++;
           return 1;
    }
    if (!strcmp(str, "-e"))
    {
           (*e)++;
           return 1;
    }
    return 0;
} */

//function qui verifie si on a deja passe le param et le skip et apres printf

void my_echo_simple_comm(struct ast *ast, size_t i)
{
    int already_met_e = 0;
    int already_met_n = 0;
    int finish = 1;
    //Check for option -n
    while (finish != 0)
    {
        if (!strcmp(ast->data[i], "-n"))
        {
            already_met_n = 1;
            i++;
        }
        if (!strcmp(ast->data[i], "-e"))
        {
            already_met_e = 1;
            i++;
        }
        finish = 0;
    }
    while (ast->data[i + 1] != NULL)
    {
        wrapper(ast->data[i], already_met_e, already_met_n);
        i++;
    }
}
