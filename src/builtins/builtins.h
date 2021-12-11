#ifndef BUILTINS_H
#define BUILTINS_H

enum builtin_type
{
    NO_BUILTIN = 0,
    BUILTIN_CD = 1,
    BUILTIN_ECHO = 2,
    BUILTIN_EXIT
};

enum builtin_type check_builtins(char **argv);

int run_builtin(enum builtin_type built, char **argv);

#endif /* !BUILTINS_H */
