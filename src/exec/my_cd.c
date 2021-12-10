#define _DEFAULT_SOURCE
#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int match_dir(char *name, struct dirent *dir)
{
    while (dir)
    {
        if (!strcmp(name, dir->d_name))
            return 1;
        dir++;
    }
    return 0;
}

int my_cd(char *new_path)
{
    if (!new_path)
        return 0;
    char *temp_name;
    //int i = 0;
    DIR *dirp = opendir(getenv("PWD"));
    struct dirent *ldir = readdir(dirp);
    char *split_path;
    split_path = strcpy(split_path, strtok_r(new_path, "/", &temp_name));
    for (size_t i = 0; i < strlen(split_path); i++)
    {
        new_path++;
    }
    while (ldir)
    {
        temp_name = ldir->d_name;
        if (!strcmp(temp_name, split_path))
        {
            int oldpwd = setenv("OLDPWD", "PWD", 1);
            int newpwd = setenv("PWD", split_path, 1);
            if (oldpwd == -1 || newpwd == -1)
            {
                warn("failed to set environment variable");
                return 1;
            }
            my_cd(new_path);
        }
        ldir = readdir(dirp);
    }
    warn("invalid path name");
    return 1;
}
