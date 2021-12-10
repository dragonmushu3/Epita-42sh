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

char *add_suffix(char *dest, char *suffix)
{
    size_t len1 = strlen(dest);
    size_t len2 = strlen(suffix);
    int has_backslash = 0;
    int mem_to_add = 2;

    if (dest[len1 - 1] == '/')
        has_backslash = 1;
    if (!has_backslash)
        mem_to_add = 3;
    char *res = malloc(sizeof(char) * (len1 + len2) + mem_to_add);

    size_t i = 0;
    while (i < len1)
    {
        res[i] = dest[i];
        i++;
    }

    if (!has_backslash)
    {
        res[i] = '/';
        i++;
    }

    size_t j = 0;
    while (j < len2)
    {
        res[i] = suffix[j];
        i++;
        j++;
    }
    res[i] = '\0';
    return res;
}

int my_cd(char *new_path)
{
    if (!new_path)
    {
        return 0;
    }

    if (!strcmp(new_path, "-"))
    {
        char *temp = getenv("OLDPWD");
        printf("%s\n", temp);
        if (setenv("OLDPWD", getenv("PWD"), 1) == -1)
        {
            warn("failed to set oldpwd variable");
            return 1;
        }

        chdir(temp);
        if (setenv("PWD", temp, 1) == -1)
        {
            warn("failed to set pwd variable");
            return 1;
        }
        printf("%s\n", getenv("PWD"));
        return 0;
    }

    char *temp_name;
    DIR *dirp = opendir(getenv("PWD"));
    struct dirent *ldir = readdir(dirp);
    char *split_path = strtok_r(new_path, "/", &temp_name);
    for (size_t i = 0; i < strlen(split_path); i++)
    {
        new_path++;
    }
    while (ldir)
    {
        if (!strcmp(ldir->d_name, split_path))
        {
            if (setenv("OLDPWD", getenv("PWD"), 1) == -1)
            {
                warn("failed to set oldpwd variable");
                return 1;
            }

            char *new_pwd = add_suffix(getenv("PWD"),split_path);
            chdir(new_pwd); 
            if (setenv("PWD", new_pwd, 1) == -1)
            {
                warn("failed to set pwd variable");
                return 1;
            }
            free(new_pwd);
            closedir(dirp);
            printf("%s\n", getenv("PWD"));
            //my_cd(new_path);
            return 0;
        }
        else
        {
            ldir = readdir(dirp);
        }
    }
    free(new_path);
    closedir(dirp);
    warn("invalid path name");
    return 1;
}
