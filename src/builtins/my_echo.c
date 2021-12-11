#include "../ast/ast.h"

#include <unistd.h>
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

int my_echo(char **argv)
{
    int met_e = 0;
    int met_n = 0;
    int nb_child = 0;
    int opt = 0;
    optind = 1;
    //Check for option -n

    size_t nb_arg = 1;
    while (argv[nb_arg])
    {
        nb_arg++;
    }

    while (opt != -1)
    {
        opt = getopt(nb_arg, argv, "en");
        if (opt == 'n')
            met_n = 1;
        if (opt == 'e')
            met_e = 1;
    }

    size_t i = optind;
    while (argv[i] != NULL)
    {
        wrapper(argv[i], met_e, met_n, nb_child);
        nb_child++;
        i++;
    }
    if (!met_n)
        putchar('\n');
    free(argv);
    return 0;
}
