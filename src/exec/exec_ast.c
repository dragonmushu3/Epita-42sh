#include "../ast/ast.h"
#include <err.h>
#include <unistd.h>
#include <sys/wait.h>

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
            return 1;
    }
    return 1;
}
/*need to check for builtins*/

int exec_ast(struct ast *ast)
{
    if (ast == NULL)
        return 1;

    if (ast->type == AST_SIMPLE_COMM)
    {
        /*this could be put in a sub_function called exec_ast_simple_comm*/
        /*check if it's echo or other built-in or it won't work*/
        /*fix me*/
        return execute_in_child(ast->data);
    }
    else if (ast->type == AST_LIST)
    {
        /*this could be put in a sub_function called exec_ast_comm */
        int res = 0;
        if (!ast->children)
        {
            warn("This list has no simple_commands attached to it! This shouldn't happen...");
        }
        else
        {
            size_t i = 0;
            while (ast->children[i])
            {
                res = exec_ast(ast->children[i]);
                i++;
            }
        }
        return res;
    }
    else if (ast->type == AST_IF)
    {
        if (exec_ast(ast->children[0]))
        {
            if (ast->children[2])
            {
                exec_ast(ast->children[2]);
            }
            return exec_ast(ast->children[1]);
        }
        return 1;
    }
    else
    {
        /*other node types when implemented*/
        return 1;
    }
}
