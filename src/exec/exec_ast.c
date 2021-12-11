#include "ast/ast.h"
#include "builtins/builtins.h"
#include <err.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

static int execute_in_child(char **args)
{
    int pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
            _exit(127);
    }
    else
    {
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            free(args);
            if (WEXITSTATUS(status) == 127)
            {
                warnx("%s: command not found", args[0]);
                return WEXITSTATUS(status);
            }
            else
            {
                return WEXITSTATUS(status);
            }
        }
        else
        {
            free(args);
            return 1;
        }
    }
    return 1;
}
/*need to check for builtins*/

int exec_ast(struct ast *ast)
{
    if (ast == NULL)
        return 0;

    if (ast->type == AST_SIMPLE_COMM)
    {
        if (ast->children)
        {
            char **data = malloc (sizeof(char *));
            size_t data_size = sizeof(char *);
            size_t data_index = 0;

            size_t i = 0;
            while (ast->children[i])
            {
                if (ast->children[i]->type == AST_WORD)
                {
                    data_size += sizeof(char *);
                    data = realloc(data, data_size);
                    data[data_index] = ast->children[i]->data[0];
                    data_index++;
                    i++;
                }
                else
                {
                    i++;
                }
            }
            data_size += sizeof(char *);
            data = realloc(data, data_size);
            data[data_index] = NULL;

            enum builtin_type built;
            if ((built = check_builtins(data)))
            {
                return run_builtin(built, data);
            }
            else
            {
                return execute_in_child(data);
            }
        }
        else
        {
            return 1;
        }
    }
    else if (ast->type == AST_LIST)
    {
        /*this could be put in a sub_function called exec_ast_list */
        int res_exec = 1;
        if (!ast->children)
        {
            warn("This list has no simple_commands attached to it! This shouldn't happen...");
        }
        else
        {
            size_t i = 0;
            while (ast->children[i])
            {
                res_exec = exec_ast(ast->children[i]);
                i++;
            }
        }
        return res_exec;
    }
    else if (ast->type == AST_IF)
    {
        if (exec_ast(ast->children[0]) == 0)
        {
            return exec_ast(ast->children[1]);
        }
        else 
        {
            return exec_ast(ast->children[2]);
        }
    }
    else
    {
        /*other node types when implemented*/
        return 1;
    }
}
