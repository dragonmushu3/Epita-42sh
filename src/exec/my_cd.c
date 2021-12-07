#define _DEFAULT_SOURCE
#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void my_cd(char *new_path)
{
    if (!new_path)
        return;
    char *temp_name;
    //int i = 0;
    DIR *dirp = opendir(getenv("PWD"));
    struct dirent *ldir = readdir(dirp);
    char *split_path = strtok_r(new_path, "/", &temp_name);
    for (size_t i = 0; i < strlen(split_path); i++)
    {
        new_path++;
    }
    while (ldir)
    {
        if (!strcmp(ldir++->d_name, split_path))
        {
            int oldpwd = setenv("OLDPWD", "PWD", 1);
            int newpwd = setenv("PWD", split_path, 1);
            if (oldpwd == -1 || newpwd == -1)
                errx(1, "cd failed");
            my_cd(new_path);
        }
    }
    errx(1, "invalid path name");
}
