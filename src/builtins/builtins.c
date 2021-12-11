#include "my_cd.h"
#include "my_echo.h"
#include "builtins.h"

#include <string.h>

enum builtin_type check_builtins(char **argv)
{
    if (!strcmp("echo", argv[0]))
        return BUILTIN_ECHO;
    else if (!strcmp("cd", argv[0]))
        return BUILTIN_CD;
    else
        return NO_BUILTIN;
}

int run_builtin(enum builtin_type built, char **argv)
{
    switch (built)
    {
        case BUILTIN_ECHO:
            return my_echo(argv);
            break;
        case BUILTIN_CD:
            return my_cd(argv[1]);
            break;
        default:
            return 1;
            break;
    }
}
