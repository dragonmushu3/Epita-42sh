#define _DEFAULT_SOURCE
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

size_t my_strlen(const char *s)
{
    size_t res = 0;
    while (*s != '\0')
    {
        s++;
        res++;
    }
    return res;
}

char *concat_space(char *dest, char *suffix)
{
    size_t len1 = my_strlen(dest);
    size_t len2 = my_strlen(suffix);
    char *res = malloc(sizeof(char) * (len1 + len2 + 2));

    size_t i = 0;
    while (i < len1)
    {
        res[i] = dest[i];
        i++;
    }
    res[i] = 32;
    i++;
    while (i < len1 + len2 + 1)
    {
        res[i] = suffix[i - len1 - 1];
        i++;
    }
    res[i] = '\0';
    return res;
}

int main(int argc, char *argv[])
{
    if (argc < 3 || !argv[2])
        exit(2);

    int output_fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (output_fd == -1)
        err(1, "Couldn't open %s", argv[1]);

    /*save stdout */
    int stdout_dup = dup(1);

    if (dup2(output_fd, 1) == -1)
    {
        err(1, "Couldn't dup");
    }
    int pid = fork();

    if (pid == 0)
    {
        /*I am child */
        char *command = argv[2];
        size_t i = 3;
        while (argv[i])
        {
            command = concat_space(command, argv[i]);
            i++;
        }

        char *argv_child[] = { "sh", "-c", command, NULL };
        if (execve("/bin/sh", argv_child, NULL) == -1)
            exit(127);
    }
    else
    {
        /*I am parent*/
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            fflush(stdout);
            dup2(stdout_dup, 1);
            close(stdout_dup);
            if (WEXITSTATUS(status) == 127)
                exit(1);
            printf("%s ", argv[2]);
            printf("exited with %d!\n", WEXITSTATUS(status));
            close(output_fd);
            exit(0);
        }
        else
            exit(1);
    }
}
